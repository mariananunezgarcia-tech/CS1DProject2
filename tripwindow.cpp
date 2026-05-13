#include "tripwindow.h"
#include "ui_tripwindow.h"


//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QSqlQuery>

static constexpr double BIG = 1e18;

tripWindow::tripWindow(const QString &startStadium,
                       const QStringList &stadiums,
                       int maxStops,
                       bool forceExact,
                       QWidget *parent, QString type)
    : QDialog(parent)
    , ui(new Ui::tripWindow)
    , m_start(norm(startStadium))
    , m_maxStops(maxStops + 1)   // includes starting stadium
    , m_forceExact(forceExact)
{
    this->type = type;
    ui->setupUi(this);
    setWindowTitle("Trip Planner");

    m_goNextBtn = ui->buttNextSum;
    if (m_goNextBtn)
    {
        m_goNextBtn->setText("Go to Next Stadium");
        connect(m_goNextBtn, &QPushButton::clicked, this, &tripWindow::onGoNextClicked);
    }

    m_buyBtn = ui->buyButt;
    if (m_buyBtn)
        connect(m_buyBtn, &QPushButton::clicked, this, &tripWindow::onBuyClicked);

    // Build unique candidate list
    QSet<QString> seen;
    for (const QString &c : stadiums)
    {
        const QString n = norm(c);
        if (!n.isEmpty() && !seen.contains(n))
        {
            seen.insert(n);
            m_candidates.push_back(n);
        }
    }

    if (!m_start.isEmpty() && !seen.contains(m_start))
        m_candidates.insert(m_candidates.begin(), m_start);

    if (m_candidates.empty() && !m_start.isEmpty())
        m_candidates.push_back(m_start);

    if (m_maxStops < 1)
        m_maxStops = 1;
    if (m_maxStops > static_cast<int>(m_candidates.size()))
        m_maxStops = static_cast<int>(m_candidates.size());

    buildRuntimeUi();

    if (!ensureDbOpen())
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not open BaseballDatabase.sqlite.\n"
                             "Distances and souvenirs may be unavailable.");
    }

    loadVisibleStadiums();
    loadAllDistances();

    /*
    if (m_forceExact || m_maxStops == static_cast<int>(m_candidates.size()))
        planExactDp();
    else
        planNearestNeighbor();
    */

    if (m_forceExact)
        planExactDp();
    else
        planNearestNeighbor();

    buildStepQueue();
    populateRouteList();

    refreshUi();
}

tripWindow::~tripWindow()
{
    delete ui;
}

QString tripWindow::norm(const QString &s)
{
    return s.trimmed();
}

void tripWindow::setReadOnly(QLineEdit *edit)
{
    if (!edit)
        return;

    edit->setReadOnly(true);
    edit->setFocusPolicy(Qt::NoFocus);
}

bool tripWindow::ensureDbOpen()
{
    const QString connName = "tripwindow_connection";

    if (QSqlDatabase::contains(connName))
    {
        QSqlDatabase db = QSqlDatabase::database(connName);
        if (db.isOpen())
            return true;
    }

    QSqlDatabase db;
    if (QSqlDatabase::contains(connName))
        db = QSqlDatabase::database(connName);
    else
        db = QSqlDatabase::addDatabase("QSQLITE", connName);

    const QString exeDir = QCoreApplication::applicationDirPath();
    QStringList candidates;

    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("BaseballDatabase.sqlite");
        if (!d.cdUp())
            break;
    }

    candidates << QDir::current().filePath("BaseballDatabase.sqlite");

    QString dbPath;
    for (const QString &p : candidates)
    {
        if (QFileInfo::exists(p))
        {
            dbPath = p;
            break;
        }
    }

    if (dbPath.isEmpty())
        return false;

    db.setDatabaseName(dbPath);
    qDebug() << "tripWindow DB path =" << dbPath;

    if (!db.open())
    {
        qDebug() << "tripWindow DB open failed:" << db.lastError().text();
        return false;
    }

    return true;
}

void tripWindow::loadAllDistances()
{
    m_dist.clear();

    QSqlDatabase db = QSqlDatabase::database("tripwindow_connection");
    if (!db.isOpen())
        return;

    QSqlQuery q(db);
    if (!q.exec(R"(
        SELECT "Beginning Stadium", "Ending Stadium", "Distance"
        FROM MLBDistances
    )"))
    {
        QMessageBox::warning(this, "Distance Query Error",
                             "Could not load distances:\n" + q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString a = norm(q.value(0).toString());
        const QString b = norm(q.value(1).toString());
        const double miles = q.value(2).toDouble();

        if (a.isEmpty() || b.isEmpty())
            continue;

        m_dist[a][b] = miles;
        m_dist[b][a] = miles;
    }
}

double tripWindow::dist(const QString &a, const QString &b) const
{
    const QString na = norm(a);
    const QString nb = norm(b);

    if (na == nb)
        return 0.0;

    if (m_dist.contains(na) && m_dist[na].contains(nb))
        return m_dist[na][nb];

    return BIG;
}

void tripWindow::buildRuntimeUi()
{
    if (ui->stadiumDisplay)
        ui->stadiumDisplay->hide();

    const QRect listRect = ui->stadiumDisplay
                               ? ui->stadiumDisplay->geometry()
                               : QRect(430, 70, 471, 271);

    m_routeList = new QListWidget(this);
    m_routeList->setGeometry(listRect);

    m_currentEdit     = ui->lineCurrColl;
    m_totalEdit       = ui->lineDistTrav;
    m_souvenirCount   = ui->lineSouvPurc;
    m_souvenirCost    = ui->lineSouvCost;
    m_nextEdit        = ui->lineNextColl;
    m_nextDistEdit    = ui->lineDistNext;
    m_souvTable       = ui->souvDisplay;

    setReadOnly(m_currentEdit);
    setReadOnly(m_totalEdit);
    setReadOnly(m_souvenirCount);
    setReadOnly(m_souvenirCost);
    setReadOnly(m_nextEdit);
    setReadOnly(m_nextDistEdit);

    if (m_souvenirCount)
        m_souvenirCount->setText("0");
    if (m_souvenirCost)
        m_souvenirCost->setText("$0.00");

    if (m_souvTable)
    {
        m_souvTable->setColumnCount(4);
        m_souvTable->setHorizontalHeaderLabels(QStringList() << "" << "Souvenir" << "Price" << "Quantity");
        m_souvTable->setRowCount(0);
        m_souvTable->verticalHeader()->setVisible(false);
        m_souvTable->setSelectionMode(QAbstractItemView::NoSelection);
        m_souvTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_souvTable->horizontalHeader()->setStretchLastSection(false);
        m_souvTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        m_souvTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        m_souvTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        m_souvTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
        m_souvTable->setColumnWidth(0, 40);
        m_souvTable->setColumnWidth(3, 80);
    }
}

void tripWindow::populateRouteList()
{
    if (!m_routeList)
        return;

    m_routeList->clear();

    int visibleNumber = 0;

    for (size_t i = 0; i < m_route.size(); ++i)
    {
        const QString stadium = norm(m_route[i]);

        // For the Marlins/default trip, hide enabled = 0 stadiums from the visible route.
        if (type == "default" && !isVisibleStadium(stadium))
            continue;

        if (visibleNumber == 0)
        {
            m_routeList->addItem(QString("Start: %1").arg(stadium));
        }
        else
        {
            m_routeList->addItem(QString("%1) %2").arg(visibleNumber).arg(stadium));
        }

        ++visibleNumber;
    }
}

void tripWindow::buildStepQueue()
{
    while (!m_stepQueue.empty())
        m_stepQueue.pop();

    double hiddenDistanceTotal = 0.0;

    for (size_t i = 1; i < m_route.size(); ++i)
    {
        const QString nextStadium = norm(m_route[i]);
        const double leg = (i - 1 < m_legs.size()) ? m_legs[i - 1] : 0.0;

        hiddenDistanceTotal += leg;

        // For the Marlins/default trip, only visible stadiums become user-visible stops.
        if (type == "default" && !isVisibleStadium(nextStadium))
        {
            continue;
        }

        // The distance shown to this visible stadium includes any hidden pass-through stadiums.
        m_stepQueue.push(std::make_pair(nextStadium, hiddenDistanceTotal));

        hiddenDistanceTotal = 0.0;
    }
}

void tripWindow::planNearestNeighbor()
{
    m_route.clear();
    m_legs.clear();

    if (m_candidates.empty())
        return;

    QSet<QString> visited;
    m_route.push_back(m_start);
    visited.insert(m_start);

    if(type=="default")
    {
        nearestStepRecursive(m_start, visited);
    }
    else if(type == "custom")
    {

        QVector<vertex> allStadiums = loadStadiumsFromDb();
        customStep(m_start, visited, allStadiums);
    }
    else if(type == "inOrderCustom")
    {
        QVector<vertex> allStadiums = loadStadiumsFromDb();
        customOrderStep(m_start, visited, allStadiums);
    }
}

void tripWindow::nearestStepRecursive(const QString &current, QSet<QString> &visited)
{
    if (static_cast<int>(m_route.size()) >= m_maxStops)
        return;

    QString bestStadium;
    double bestDist = BIG;

    for (const QString &candidate : m_candidates)
    {
        if (visited.contains(candidate))
            continue;

        const double d = dist(current, candidate);
        if (d < bestDist)
        {
            bestDist = d;
            bestStadium = candidate;
        }
    }

    QSqlDatabase db = QSqlDatabase::database("tripwindow_connection");
    if (!db.isOpen())
        return;

    if ((bestStadium.isEmpty() || bestDist >= BIG) && visited.size() < m_maxStops)
    {
        QString currentStadium = current;
        QSqlQuery q(db);
        q.prepare(R"(
        SELECT
            "Beginning Stadium", "Ending Stadium", "Distance"
        FROM MLBDistances
        WHERE "Beginning Stadium" = :begin
        )");
        q.bindValue(":begin", currentStadium);

        if (!q.exec())
        {
            QMessageBox::warning(this, "Query Error",
                                 "Could not load stadium list:\n" + q.lastError().text());
            return;
        }

        while (q.next())
        {
            const QString s = q.value(0).toString().trimmed();
            const QString e = q.value(1).toString().trimmed();
            const int d = q.value(2).toInt();
            if (!s.isEmpty())
            {
                currentStadium = e;
                QSqlQuery query(db);
                query.prepare(R"(
                SELECT
                    "Beginning Stadium", "Ending Stadium", "Distance"
                FROM MLBDistances
                WHERE "Beginning Stadium" = :end
                )");
                query.bindValue(":end", currentStadium);
                if (!query.exec())
                {
                    QMessageBox::warning(this, "Query Error",
                                         "Could not load stadium list:\n" + query.lastError().text());
                    return;
                }
                while(query.next())
                {
                    const QString e2 = query.value(1).toString().trimmed();
                    const int d2 = query.value(2).toInt();
                    if (!visited.contains(e2))
                    {
                        const double d3 = d + d2;
                        if (d3 < bestDist)
                        {
                            bestDist = d3;
                            bestStadium = e2;
                        }
                    }
                }
            }
        }
    }

    if (bestStadium.isEmpty() || bestDist >= BIG)
        return;

    visited.insert(bestStadium);
    m_route.push_back(bestStadium);
    totalDist += bestDist;
    m_legs.push_back(bestDist);

    nearestStepRecursive(bestStadium, visited);
}

void tripWindow::customStep(const QString &current, QSet<QString> &visited, QVector<vertex> allStadiums)
{
    if (static_cast<int>(m_route.size()) >= m_maxStops)
    {
        return;
    }

    if (allStadiums.isEmpty())
    {
        return;
    }

    QString bestStadium;
    double bestDist = BIG;

    const int startIndex = findStadium(current, allStadiums);

    if (startIndex < 0 || startIndex >= allStadiums.size())
    {
        QMessageBox::warning(this, "Invalid Index", "Current stadium was not found.");
        return;
    }

    const int stadiumCount = allStadiums.size();

    QVector<int> smalls(stadiumCount, 5000);
    QVector<int> parents(stadiumCount, -1);

    int num = 0;

    smalls[startIndex] = 0;
    parents[startIndex] = -1;

    dijkstraAlgorithm(startIndex, parents[startIndex], num, allStadiums, smalls.data(), parents.data());

    for (const QString &candidate : m_candidates)
    {
        if (visited.contains(candidate))
        {
            continue;
        }

        const int candidateIndex = findStadium(candidate, allStadiums);

        if (candidateIndex < 0 || candidateIndex >= smalls.size())
        {
            continue;
        }

        const double d = smalls[candidateIndex];

        if (d < bestDist)
        {
            bestDist = d;
            bestStadium = candidate;
        }
    }

    if (bestStadium.isEmpty() || bestDist >= BIG)
    {
        return;
    }

    visited.insert(bestStadium);
    m_route.push_back(bestStadium);

    totalDist += bestDist;
    m_legs.push_back(bestDist);

    customStep(bestStadium, visited, allStadiums);
}

void tripWindow::customOrderStep(const QString &current, QSet<QString> &visited, QVector<vertex> allStadiums)
{
    if (static_cast<int>(m_route.size()) >= m_maxStops)
    {
        return;
    }

    if (allStadiums.isEmpty())
    {
        return;
    }

    QString bestStadium;
    double bestDist = BIG;

    const int startIndex = findStadium(current, allStadiums);

    if (startIndex < 0 || startIndex >= allStadiums.size())
    {
        QMessageBox::warning(this, "Invalid Index", "Current stadium was not found.");
        return;
    }

    const int stadiumCount = allStadiums.size();

    QVector<int> smalls(stadiumCount, 5000);
    QVector<int> parents(stadiumCount, -1);

    int num = 0;

    smalls[startIndex] = 0;
    parents[startIndex] = -1;

    dijkstraAlgorithm(startIndex, parents[startIndex], num, allStadiums, smalls.data(), parents.data());

    for (const QString &candidate : m_candidates)
    {
        if (visited.contains(candidate))
        {
            continue;
        }

        const int candidateIndex = findStadium(candidate, allStadiums);

        if (candidateIndex < 0 || candidateIndex >= smalls.size())
        {
            continue;
        }

        bestDist = smalls[candidateIndex];
        bestStadium = candidate;
        break;
    }

    if (bestStadium.isEmpty() || bestDist >= BIG)
    {
        return;
    }

    visited.insert(bestStadium);
    m_route.push_back(bestStadium);

    totalDist += bestDist;
    m_legs.push_back(bestDist);

    customOrderStep(bestStadium, visited, allStadiums);
}


double tripWindow::tspRecursive(int mask, int last)
{
    if (mask == m_allMask)
        return 0.0;

    double &memo = m_memo[mask][last];
    if (memo >= 0.0)
        return memo;

    double best = BIG;
    int bestNext = -1;

    for (int nxt = 0; nxt < static_cast<int>(m_nodes.size()); ++nxt)
    {
        if (mask & (1 << nxt))
            continue;

        double edge = 0.0;
        if (!m_adj[static_cast<size_t>(last)].find(nxt, edge))
            continue;

        const double cand = edge + tspRecursive(mask | (1 << nxt), nxt);
        if (cand < best)
        {
            best = cand;
            bestNext = nxt;
        }
    }

    m_next[mask][last] = bestNext;
    memo = best;
    return memo;
}

void tripWindow::planExactDp()
{
    m_route.clear();
    m_legs.clear();

    if (m_candidates.empty())
        return;

    m_nodes.clear();
    m_nodes.push_back(m_start);

    for (const QString &c : m_candidates)
    {
        if (norm(c) != m_start)
            m_nodes.push_back(norm(c));
    }

    if (static_cast<int>(m_nodes.size()) > m_maxStops)
        m_nodes.resize(static_cast<size_t>(m_maxStops));

    const int n = static_cast<int>(m_nodes.size());
    if (n == 0)
        return;
    if (n == 1)
    {
        m_route.push_back(m_nodes[0]);
        return;
    }

    m_adj.clear();
    m_adj.resize(static_cast<size_t>(n));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (i == j)
                continue;

            const double d = dist(m_nodes[static_cast<size_t>(i)], m_nodes[static_cast<size_t>(j)]);
            if (d < BIG)
                m_adj[static_cast<size_t>(i)].insert(j, d);
        }
    }

    m_allMask = (1 << n) - 1;
    m_memo.assign(static_cast<size_t>(1 << n), std::vector<double>(static_cast<size_t>(n), -1.0));
    m_next.assign(static_cast<size_t>(1 << n), std::vector<int>(static_cast<size_t>(n), -1));

    (void)tspRecursive(1, 0);

    m_route.push_back(m_nodes[0]);

    int mask = 1;
    int last = 0;

    while (true)
    {
        const int nxt = m_next[mask][last];
        if (nxt < 0)
            break;

        double leg = 0.0;
        if (!m_adj[static_cast<size_t>(last)].find(nxt, leg))
            break;

        m_route.push_back(m_nodes[static_cast<size_t>(nxt)]);
        m_legs.push_back(leg);

        mask |= (1 << nxt);
        last = nxt;
    }
}

void tripWindow::loadSouvenirsForCurrentStadium()
{
    if (!m_souvTable)
        return;

    m_souvTable->clearContents();
    m_souvTable->setRowCount(0);

    if (m_index < 0 || m_index >= static_cast<int>(m_route.size()))
        return;

    QSqlDatabase db = QSqlDatabase::database("tripwindow_connection");
    if (!db.isOpen())
        return;

    const QString stadium = norm(m_route[static_cast<size_t>(m_index)]);

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT
            "Name" AS souvenir,
            "Price" AS price
        FROM SouvenirList
        WHERE "Stadium" = :Stadium
    )");
    q.bindValue(":Stadium", stadium);

    if (!q.exec())
        return;

    int row = 0;
    while (q.next())
    {
        const QString itemName = q.value(0).toString().trimmed();
        const double price = q.value(1).toDouble();

        m_souvTable->insertRow(row);

        QTableWidgetItem *checkItem = new QTableWidgetItem;
        checkItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        checkItem->setCheckState(Qt::Unchecked);
        checkItem->setText("");
        checkItem->setData(Qt::UserRole, itemName);
        checkItem->setData(Qt::UserRole + 1, price);

        QTableWidgetItem *nameItem = new QTableWidgetItem(itemName);
        nameItem->setFlags(Qt::ItemIsEnabled);

        QTableWidgetItem *priceItem =
            new QTableWidgetItem(QString("$%1").arg(price, 0, 'f', 2));
        priceItem->setFlags(Qt::ItemIsEnabled);

        QLineEdit *qtyEdit = new QLineEdit(this);
        qtyEdit->setText("1");
        qtyEdit->setAlignment(Qt::AlignCenter);
        qtyEdit->setValidator(new QIntValidator(1, 9999, qtyEdit));

        m_souvTable->setItem(row, 0, checkItem);
        m_souvTable->setItem(row, 1, nameItem);
        m_souvTable->setItem(row, 2, priceItem);
        m_souvTable->setCellWidget(row, 3, qtyEdit);

        ++row;
    }
}

void tripWindow::updateSouvenirTotals()
{
    if (m_souvenirCount)
        m_souvenirCount->setText(QString::number(m_totalSouvenirCount));

    if (m_souvenirCost)
        m_souvenirCost->setText(QString("$%1").arg(m_totalSouvenirCost, 0, 'f', 2));
}

void tripWindow::clearSouvenirChecks()
{
    if (!m_souvTable)
        return;

    for (int row = 0; row < m_souvTable->rowCount(); ++row)
    {
        QTableWidgetItem *item = m_souvTable->item(row, 0);
        if (item)
            item->setCheckState(Qt::Unchecked);

        QWidget *cell = m_souvTable->cellWidget(row, 3);
        QLineEdit *qtyEdit = qobject_cast<QLineEdit*>(cell);
        if (qtyEdit)
            qtyEdit->setText("1");
    }
}

void tripWindow::refreshUi()
{
    if (!m_currentEdit || !m_totalEdit || !m_nextEdit || !m_nextDistEdit)
        return;

    if (m_index < 0 || m_index >= static_cast<int>(m_route.size()))
        return;

    m_currentEdit->setText(m_route[static_cast<size_t>(m_index)]);
    m_totalEdit->setText(QString::number(m_traveled, 'f', 1));
    ui->lineDistTrav_2->setText(QString::number(totalDist, 'f', 1));

    updateSouvenirTotals();

    const QString currentStadium = norm(m_route[static_cast<size_t>(m_index)]);

    if (type == "default" && !isVisibleStadium(currentStadium))
    {
        if (m_souvTable)
            m_souvTable->setRowCount(0);

        if (m_buyBtn)
            m_buyBtn->setEnabled(false);
    }
    else
    {
        if (m_buyBtn)
            m_buyBtn->setEnabled(true);

        loadSouvenirsForCurrentStadium();
    }
}

void tripWindow::onBuyClicked()
{
    if (!m_souvTable)
        return;

    if (m_index < 0 || m_index >= static_cast<int>(m_route.size()))
        return;

    const QString currentStadium = norm(m_route[static_cast<size_t>(m_index)]);

    int boughtThisClick = 0;
    double costThisClick = 0.0;

    for (int row = 0; row < m_souvTable->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = m_souvTable->item(row, 0);
        if (!checkItem || checkItem->checkState() != Qt::Checked)
            continue;

        QLineEdit *qtyEdit =
            qobject_cast<QLineEdit*>(m_souvTable->cellWidget(row, 3));

        int quantity = 1;
        if (qtyEdit)
        {
            quantity = qtyEdit->text().trimmed().toInt();
        }

        if (quantity <= 0)
            continue;

        const QString souvenirName = checkItem->data(Qt::UserRole).toString().trimmed();
        const double price = checkItem->data(Qt::UserRole + 1).toDouble();

        for (int i = 0; i < quantity; ++i)
        {
            Souvenir s;
            s.stadium = currentStadium;
            s.name = souvenirName;
            s.price = price;
            s.quantity = 1;   // each queue entry still represents one purchased copy
            m_purchasedSouvenirs.push(s);
        }

        boughtThisClick += quantity;
        costThisClick += price * quantity;
    }

    if (boughtThisClick == 0)
    {
        QMessageBox::information(this, "No Purchase",
                                 "Please check at least one souvenir and enter a valid quantity.");
        return;
    }

    m_totalSouvenirCount += boughtThisClick;
    m_totalSouvenirCost += costThisClick;

    updateSouvenirTotals();
    clearSouvenirChecks();
}

void tripWindow::onGoNextClicked()
{
    if (!m_stepQueue.empty())
    {
        const auto nextStep = m_stepQueue.front();
        m_stepQueue.pop();

        const QString nextVisibleStadium = norm(nextStep.first);

        // Move m_index forward until it reaches the visible stadium.
        for (int i = m_index + 1; i < static_cast<int>(m_route.size()); ++i)
        {
            if (norm(m_route[static_cast<size_t>(i)]).compare(nextVisibleStadium, Qt::CaseInsensitive) == 0)
            {
                m_index = i;
                break;
            }
        }

        m_traveled += nextStep.second;

        refreshUi();
        return;
    }

    std::queue<Souvenir> copy = m_purchasedSouvenirs;
    std::vector<summaryWindow::PurchasedSouvenir> purchasesForSummary;

    while (!copy.empty())
    {
        const Souvenir s = copy.front();
        copy.pop();

        summaryWindow::PurchasedSouvenir out;
        out.stadium = s.stadium;
        out.name = s.name;
        out.price = s.price;
        purchasesForSummary.push_back(out);
    }

    summaryWindow dlg(m_route, m_legs, m_traveled, purchasesForSummary, this);
    dlg.setModal(true);
    dlg.exec();

    accept();
}

void tripWindow::loadVisibleStadiums()
{
    m_visibleStadiums.clear();

    QSqlDatabase db = QSqlDatabase::database("tripwindow_connection");

    if (!db.isOpen())
        return;

    QSqlQuery q(db);

    q.prepare(R"(
        SELECT TRIM(stadium)
        FROM stadium_access
        WHERE enabled = 1
          AND TRIM(stadium) <> ''
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load visible stadiums:\n" +
                                 q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString stadium = norm(q.value(0).toString());

        if (!stadium.isEmpty())
            m_visibleStadiums.insert(stadium);
    }
}

bool tripWindow::isVisibleStadium(const QString &stadium) const
{
    const QString s = norm(stadium);

    return m_visibleStadiums.contains(s);
}
