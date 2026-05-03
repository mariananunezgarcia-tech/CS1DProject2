#include "specialalgorithms.h"
#include "ui_specialalgorithms.h"
#include "mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QAbstractItemView>

#include <queue>
#include <algorithm>

SearchAlgorithms::SearchAlgorithms(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SearchAlgorithms)
{
    ui->setupUi(this);
    setWindowTitle("Special Algorithms");

    m_resultsModel = new QStringListModel(this);
    ui->listViewResults->setModel(m_resultsModel);

    if (!ensureDbOpen())
    {
        showResultsInListView("Could not open BaseballDatabase.sqlite.");
        return;
    }

    loadStadiums();

    ui->listViewResults->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

SearchAlgorithms::~SearchAlgorithms()
{
    const QString connName = m_db.connectionName();

    if (m_db.isValid() && m_db.isOpen())
        m_db.close();

    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connName);

    delete ui;
}

bool SearchAlgorithms::ensureDbOpen()
{
    const QString connName = QString("special_algorithms_conn_%1")
    .arg(reinterpret_cast<quintptr>(this));

    if (QSqlDatabase::contains(connName))
        m_db = QSqlDatabase::database(connName);
    else
        m_db = QSqlDatabase::addDatabase("QSQLITE", connName);

    if (!m_db.isValid())
        return false;

    if (m_db.isOpen())
        return true;

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
            dbPath = QFileInfo(p).absoluteFilePath();
            break;
        }
    }

    if (dbPath.isEmpty())
        return false;

    m_db.setDatabaseName(dbPath);
    return m_db.open();
}

void SearchAlgorithms::loadStadiums()
{
    ui->comboSelStadium->clear();
    ui->comboSelStadium->addItem("Select a Stadium");

    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery q(m_db);
    const QString sql = R"(
        SELECT DISTINCT TRIM([Stadium name])
        FROM MLBInformation
        WHERE TRIM([Stadium name]) <> ''
        ORDER BY TRIM([Stadium name]) ASC
    )";

    if (!q.exec(sql))
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load stadiums:\n" + q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString stadium = q.value(0).toString().trimmed();
        if (!stadium.isEmpty())
            ui->comboSelStadium->addItem(stadium);
    }
}

QString SearchAlgorithms::runBfsFrom(const QString &startStadium)
{
    if (!m_db.isOpen() && !ensureDbOpen())
        return "Database is not open.";

    // ------------------------------------------------------------
    // Step 1: Load all stadium names
    // ------------------------------------------------------------
    QStringList stadiums;
    QSqlQuery stadiumQuery(m_db);
    stadiumQuery.prepare(R"(
        SELECT DISTINCT TRIM([Stadium name])
        FROM MLBInformation
        WHERE TRIM([Stadium name]) <> ''
        ORDER BY TRIM([Stadium name]) ASC
    )");

    if (!stadiumQuery.exec())
        return "Could not load stadium list:\n" + stadiumQuery.lastError().text();

    while (stadiumQuery.next())
        stadiums << stadiumQuery.value(0).toString().trimmed();

    const int NUM_STADIUMS = stadiums.size();
    if (NUM_STADIUMS == 0)
        return "No stadiums were found in the database.";

    const int start = stadiums.indexOf(startStadium.trimmed());
    if (start < 0)
        return "Selected stadium was not found.";

    // ------------------------------------------------------------
    // Step 2: Build an undirected adjacency matrix
    // ------------------------------------------------------------
    std::vector<std::vector<int>> adjacencyMatrix(
        static_cast<size_t>(NUM_STADIUMS),
        std::vector<int>(static_cast<size_t>(NUM_STADIUMS), 0)
        );

    QSqlQuery distQuery(m_db);
    distQuery.prepare(R"(
        SELECT TRIM([Beginning Stadium]),
               TRIM([Ending Stadium]),
               [Distance]
        FROM MLBDistances
    )");

    if (!distQuery.exec())
        return "Could not load stadium distances:\n" + distQuery.lastError().text();

    while (distQuery.next())
    {
        const QString from = distQuery.value(0).toString().trimmed();
        const QString to   = distQuery.value(1).toString().trimmed();
        const int miles    = distQuery.value(2).toInt();

        const int i = stadiums.indexOf(from);
        const int j = stadiums.indexOf(to);

        if (i < 0 || j < 0 || i == j || miles <= 0)
            continue;

        // Treat as undirected
        adjacencyMatrix[static_cast<size_t>(i)][static_cast<size_t>(j)] = miles;
        adjacencyMatrix[static_cast<size_t>(j)][static_cast<size_t>(i)] = miles;
    }

    // ------------------------------------------------------------
    // Step 3: BFS setup
    // ------------------------------------------------------------
    std::vector<bool> visited(static_cast<size_t>(NUM_STADIUMS), false);
    std::vector<int> level(static_cast<size_t>(NUM_STADIUMS), -1);
    std::vector<std::vector<bool>> edgeLabeled(
        static_cast<size_t>(NUM_STADIUMS),
        std::vector<bool>(static_cast<size_t>(NUM_STADIUMS), false)
        );

    std::queue<int> bfsQueue;
    std::vector<std::vector<int>> levels;
    std::vector<EdgeRecord> discoveryEdges;
    std::vector<EdgeRecord> crossEdges;
    int discoveryDistanceTotal = 0;

    visited[static_cast<size_t>(start)] = true;
    level[static_cast<size_t>(start)] = 0;
    bfsQueue.push(start);
    levels.push_back({ start });

    // ------------------------------------------------------------
    // Step 4: Main BFS loop
    // ------------------------------------------------------------
    while (!bfsQueue.empty())
    {
        const int current = bfsQueue.front();
        bfsQueue.pop();

        std::vector<Neighbor> neighbors;

        for (int i = 0; i < NUM_STADIUMS; ++i)
        {
            if (adjacencyMatrix[static_cast<size_t>(current)][static_cast<size_t>(i)] != 0)
            {
                neighbors.push_back({
                    i,
                    adjacencyMatrix[static_cast<size_t>(current)][static_cast<size_t>(i)]
                });
            }
        }

        std::sort(neighbors.begin(), neighbors.end(),
                  [&](const Neighbor &a, const Neighbor &b)
                  {
                      if (a.mileage != b.mileage)
                          return a.mileage < b.mileage;

                      return stadiums[a.stadiumIndex] < stadiums[b.stadiumIndex];
                  });

        for (const Neighbor &neighbor : neighbors)
        {
            const int next = neighbor.stadiumIndex;
            const int mileage = neighbor.mileage;

            const int a = std::min(current, next);
            const int b = std::max(current, next);

            if (edgeLabeled[static_cast<size_t>(a)][static_cast<size_t>(b)])
                continue;

            edgeLabeled[static_cast<size_t>(a)][static_cast<size_t>(b)] = true;

            if (!visited[static_cast<size_t>(next)])
            {
                visited[static_cast<size_t>(next)] = true;
                level[static_cast<size_t>(next)] = level[static_cast<size_t>(current)] + 1;
                bfsQueue.push(next);

                if (static_cast<int>(levels.size()) <= level[static_cast<size_t>(next)])
                    levels.push_back({});

                levels[static_cast<size_t>(level[static_cast<size_t>(next)])].push_back(next);

                discoveryEdges.push_back({ current, next, mileage });
                discoveryDistanceTotal += mileage;
            }
            else
            {
                crossEdges.push_back({ current, next, mileage });
            }
        }
    }

    // ------------------------------------------------------------
    // Step 5: Format output
    // ------------------------------------------------------------
    QString output;
    output += "BFS starting at " + stadiums[start] + "\n\n";

    for (size_t i = 0; i < levels.size(); ++i)
    {
        output += "Level " + QString::number(static_cast<int>(i)) + ": ";
        for (size_t j = 0; j < levels[i].size(); ++j)
        {
            output += stadiums[levels[i][j]];
            if (j + 1 < levels[i].size())
                output += ", ";
        }
        output += "\n";
    }

    output += "\nDiscovery edges:\n";
    for (const EdgeRecord &edge : discoveryEdges)
    {
        output += stadiums[edge.from] + " -> " + stadiums[edge.to]
                  + " (" + QString::number(edge.mileage) + " miles)\n";
    }

    output += "\nCross edges:\n";
    for (const EdgeRecord &edge : crossEdges)
    {
        output += stadiums[edge.from] + " -> " + stadiums[edge.to]
                  + " (" + QString::number(edge.mileage) + " miles)\n";
    }

    output += "\nTotal distance on discovery edges: "
              + QString::number(discoveryDistanceTotal) + "\n";

    return output;
}

void SearchAlgorithms::on_buttonBFS_clicked()
{
    const QString startStadium = ui->comboSelStadium->currentText().trimmed();

    if (startStadium.isEmpty() || startStadium == "Select a Stadium")
    {
        showResultsInListView("Please select a starting stadium first.");
        return;
    }

    const QString result = runBfsFrom(startStadium);
    showResultsInListView(result);
}

void SearchAlgorithms::on_buttonBack_clicked()
{
    MainWindow *win = new MainWindow(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

void SearchAlgorithms::showResultsInListView(const QString &text)
{
    QStringList lines = text.split('\n', Qt::KeepEmptyParts);

    //Remove trailing empty lines
    while (!lines.isEmpty() && lines.last().trimmed().isEmpty())
        lines.removeLast();

    m_resultsModel->setStringList(lines);
}


QString SearchAlgorithms::runDfsFromOracle()
{
    QHash<QString, QVector<DfsEdge>> graph;

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT "Beginning Stadium", "Ending Stadium", "Distance"
        FROM MLBDistances
        WHERE TRIM("Beginning Stadium") <> ''
          AND TRIM("Ending Stadium") <> ''
    )");

    if (!query.exec())
        return "DFS query failed:\n" + query.lastError().text();

    while (query.next())
    {
        const QString from = query.value(0).toString().trimmed();
        const QString to   = query.value(1).toString().trimmed();
        const int miles    = query.value(2).toInt();

        if (from.isEmpty() || to.isEmpty())
            continue;

        graph[from].push_back({to, miles});
        graph[to].push_back({from, miles});
    }

    // sort neighbors by shortest distance
    for (auto it = graph.begin(); it != graph.end(); ++it)
    {
        std::sort(it.value().begin(), it.value().end(),
                  [](const DfsEdge &a, const DfsEdge &b)
                  {
                      if (a.miles != b.miles)
                          return a.miles < b.miles;
                      return a.dest < b.dest;
                  });
    }

    const QString start = "Oracle Park";

    if (!graph.contains(start))
        return "Oracle Park not found in graph.";

    QSet<QString> visited;
    QStringList order;
    int totalMiles = 0;

    QString output;

    output += "Discovery Edges: \n\n";

    std::function<void(const QString&)> dfs = [&](const QString &current)
    {
        visited.insert(current);
        order << current;

        for (const DfsEdge &edge : graph[current])
        {

            if (!visited.contains(edge.dest))
            {
                output += current + " -> " + edge.dest +
                          " (" + QString::number(edge.miles) + " miles)\n";

                totalMiles += edge.miles;
                dfs(edge.dest);
            }

        }
    };

    dfs(start);


    output += "\nTotal mileage: " + QString::number(totalMiles);

    return output;
}

void SearchAlgorithms::on_buttonDFS_clicked()
{
    QString result = runDfsFromOracle();
    showResultsInListView(result);
}
