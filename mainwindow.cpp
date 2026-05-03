#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"

#include "basictripwindow.h"
#include "customtripwindow.h"
#include "specialalgorithms.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QHeaderView>
#include <QComboBox>
#include <QTableView>
#include <QRect>
#include <QAbstractItemView>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ensureDbOpen();
    setupDistanceUi();
    populateSouvenirTeamCombo();
    populateStartingStadiumCombo();

    m_distanceModel->setQuery("SELECT '' AS col1, '' AS col2 WHERE 1=0", m_db);
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "");

    resizeDistanceTableColumns();
}

MainWindow::~MainWindow()
{
    const QString connName = m_db.connectionName();

    if (m_db.isValid() && m_db.isOpen())
        m_db.close();

    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connName);

    delete ui;
}

void MainWindow::resizeDistanceTableColumns()
{
    if (!m_distanceTable || !m_distanceTable->model())
        return;

    const int totalWidth = m_distanceTable->viewport()->width();
    if (totalWidth <= 0)
        return;

    // const int firstColWidth  = static_cast<int>(totalWidth * 0.5);
    // const int secondColWidth = totalWidth - firstColWidth;

    // m_distanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    // m_distanceTable->setColumnWidth(0, firstColWidth);
    // m_distanceTable->setColumnWidth(1, secondColWidth);
    m_distanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    resizeDistanceTableColumns();
}

// ------------------------------------------------------------
// Opening Database
// ------------------------------------------------------------

bool MainWindow::ensureDbOpen()
{
    const QString connName = QString("main_conn_%1")
    .arg(reinterpret_cast<quintptr>(this));

    if (QSqlDatabase::contains(connName))
        m_db = QSqlDatabase::database(connName);
    else
        m_db = QSqlDatabase::addDatabase("QSQLITE", connName);

    if (!m_db.isValid())
    {
        QMessageBox::critical(this, "Database Error",
                              "QSQLITE driver is not available.");
        return false;
    }

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
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not find BaseballDatabase.sqlite.");
        return false;
    }

    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open SQLite database:\n" +
                                  m_db.lastError().text() +
                                  "\n\nPath tried:\n" + dbPath);
        return false;
    }

    return true;
}

// ------------------------------------------------------------
// Distance UI
// ------------------------------------------------------------

void MainWindow::setupDistanceUi()
{
    if (!ensureDbOpen())
        return;

    m_fromStadiumCombo = ui->dropdownDistances;

    QRect tableRect(260, 30, 541, 410);
    if (ui->distanceListView)
    {
        tableRect = ui->distanceListView->geometry();
        ui->distanceListView->hide();
    }

    m_distanceTable = new QTableView(ui->centralwidget);
    m_distanceTable->setGeometry(tableRect);
    m_distanceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_distanceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_distanceTable->horizontalHeader()->setStretchLastSection(false);

    m_distanceModel = new QSqlQueryModel(m_distanceTable);
    m_distanceTable->setModel(m_distanceModel);

    populateStartingStadiumCombo();

    if (m_fromStadiumCombo && m_fromStadiumCombo->count() > 1)
    {
        QString start = m_fromStadiumCombo->currentData().toString();
        if (start.isEmpty())
            start = m_fromStadiumCombo->currentText();

        if (!start.isEmpty() &&
            start.compare("Select an MLB Stadium", Qt::CaseInsensitive) != 0)
        {
            loadDistancesForTeam(start);
        }
    }
}

// ------------------------------------------------------------
// Select Starting team box
// ------------------------------------------------------------

void MainWindow::populateStartingStadiumCombo()
{
    if (!m_fromStadiumCombo)
        return;

    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    m_fromStadiumCombo->clear();
    m_fromStadiumCombo->addItem("Select an MLB Stadium");

    QSqlQuery q(m_db);

    const QString sql = R"(
        SELECT DISTINCT "Beginning Stadium"
        FROM MLBDistances
        ORDER BY "Beginning Stadium" ASC
    )";

    if (!q.exec(sql))
    {
        QMessageBox::critical(this, "Query Error",
                              "Team list query failed:\n" +
                                  q.lastError().text() +
                                  "\n\nDB: " + m_db.databaseName() +
                                  "\nSQL:\n" + sql);
        return;
    }

    while (q.next())
    {
        const QString c = q.value(0).toString().trimmed();
        if (!c.isEmpty())
            m_fromStadiumCombo->addItem(c, c);
    }

    const int idx = m_fromStadiumCombo->findData("Select an MLB Stadium");
    if (idx >= 0)
        m_fromStadiumCombo->setCurrentIndex(idx);
}

// ------------------------------------------------------------
// Distance summary list
// ------------------------------------------------------------

void MainWindow::loadDistancesForTeam(const QString &fromStadium)
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    const QString from = fromStadium.trimmed();
    if (from.isEmpty() ||
        from.compare("Select an MLB Stadium", Qt::CaseInsensitive) == 0)
        return;

    QSqlQuery query(m_db);
    query.prepare(R"(
    SELECT
        "Ending Stadium" AS Stadium,
        "Distance" AS Distance
    FROM MLBDistances
    WHERE "Beginning Stadium" = :stadium
      AND "Ending Stadium" <> :stadium
    ORDER BY "Distance" ASC, "Ending Stadium" ASC
    )");
    query.bindValue(":stadium", from);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Distance query failed:\n" + query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              "Distance model error:\n" + m_distanceModel->lastError().text());
        return;
    }

    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Stadium Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Distance (Miles)");

    resizeDistanceTableColumns();
}

// ------------------------------------------------------------
// Souvenir list for each team
// ------------------------------------------------------------

void MainWindow::loadSouvenirsForTeam(const QString &Team)
{
    // if (!m_distanceTable || !m_distanceModel)
    //     return;
    // if (!m_db.isOpen() && !ensureDbOpen())
    //     return;

    // const QString selectedTeam = Team.trimmed();
    // if (selectedTeam.isEmpty() ||
    //     selectedTeam.compare("Select an MLB Team", Qt::CaseInsensitive) == 0)
    //     return;

    // QSqlQuery query(m_db);
    // query.prepare(R"(
    //     SELECT
    //         TRIM(s.item) AS souvenir,
    //         s.price      AS price
    //     FROM souvenirs s
    //     LEFT JOIN souvenir_access sa
    //       ON TRIM(sa.Team) = TRIM(s.Team)
    //      AND TRIM(sa.item)   = TRIM(s.item)
    //     WHERE TRIM(s.Team) = :Team
    //       AND (sa.enabled = 1 OR sa.enabled IS NULL)
    //       AND TRIM(s.item) <> ''
    //     ORDER BY TRIM(s.item) ASC
    // )");
    // query.bindValue(":Team", selectedTeam);

    // if (!query.exec())
    // {
    //     QMessageBox::critical(this, "Query Error",
    //                           "Souvenir query failed:\n" + query.lastError().text());
    //     return;
    // }

    // m_distanceModel->setQuery(query);

    // if (m_distanceModel->lastError().isValid())
    // {
    //     QMessageBox::critical(this, "Model Error",
    //                           "Souvenir model error:\n" + m_distanceModel->lastError().text());
    //     return;
    // }

    // m_distanceModel->setHeaderData(0, Qt::Horizontal, "Souvenir");
    // m_distanceModel->setHeaderData(1, Qt::Horizontal, "Price");

    // resizeDistanceTableColumns();
}

// ------------------------------------------------------------
// Souvenir summary list
// ------------------------------------------------------------

void MainWindow::previewSouvenirButtonClick()
{
    const QString Team = ui->dropdownSouvenirPreview->currentText().trimmed();

    if (Team.isEmpty() ||
        Team.compare("Select an MLB Team", Qt::CaseInsensitive) == 0)
    {
        m_distanceModel->setQuery("SELECT '' AS col1, '' AS col2 WHERE 1=0", m_db);
        m_distanceModel->setHeaderData(0, Qt::Horizontal, "");
        m_distanceModel->setHeaderData(1, Qt::Horizontal, "");
        return;
    }

    loadSouvenirsForTeam(Team);
}

// ------------------------------------------------------------
// Quit Program Button
// ------------------------------------------------------------

void MainWindow::cancelButtonClick()
{
    QCoreApplication::quit();
}

// ------------------------------------------------------------
// Distance Table from Team Selected
// ------------------------------------------------------------

void MainWindow::on_buttonDistancesSubmit_clicked()
{
    const QString Team = ui->dropdownDistances->currentText().trimmed();

    if (Team.isEmpty() ||
        Team.compare("Select an MLB Team", Qt::CaseInsensitive) == 0)
    {
        m_distanceModel->setQuery("SELECT '' AS col1, '' AS col2 WHERE 1=0", m_db);
        m_distanceModel->setHeaderData(0, Qt::Horizontal, "");
        m_distanceModel->setHeaderData(1, Qt::Horizontal, "");
        return;
    }

    loadDistancesForTeam(Team);
}

// ------------------------------------------------------------
// Basic Trip Button
// ------------------------------------------------------------

void MainWindow::on_buttonBasicTrip_clicked()
{
    auto *win = new BasicTripWindow(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close();
}

// ------------------------------------------------------------
// Custom Trip Button
// ------------------------------------------------------------

void MainWindow::on_buttonCustomTrip_clicked()
{
    auto *win = new CustomTripWindow(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close();
}

// ------------------------------------------------------------
// IDK button
// ------------------------------------------------------------

void MainWindow::on_pushButton_clicked()
{
    auto *win = new Login(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}

// ------------------------------------------------------------
// Create the Dropdown List With all 30 teams
// ------------------------------------------------------------

void MainWindow::populateSouvenirTeamCombo()
{
    if (!ui->dropdownSouvenirPreview)
        return;

    ui->dropdownSouvenirPreview->clear();
    ui->dropdownSouvenirPreview->addItem("Select an MLB Team");

    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery q(m_db);

    const QString sql = R"(
        SELECT DISTINCT "Team name"
        FROM MLBInformation
        ORDER BY "Team name" ASC
    )";

    if (!q.exec(sql))
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load enabled colleges:\n" +
                                 q.lastError().text() +
                                 "\n\nDB: " + m_db.databaseName() +
                                 "\nSQL:\n" + sql);
        return;
    }

    while (q.next())
    {
        const QString Team = q.value(0).toString().trimmed();
        if (!Team.isEmpty())
            ui->dropdownSouvenirPreview->addItem(Team);
    }
}

// ------------------------------------------------------------
// Shortest to Center Field Button
// ------------------------------------------------------------

void MainWindow::loadClosestCenterField()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name", "Distance to center field"
    FROM MLBInformation
    WHERE "Distance to center field" = (
        SELECT MIN("Distance to center field")
        FROM MLBInformation
    )
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Distance to Center Field");

    resizeDistanceTableColumns();
}

void MainWindow::on_ClosestCenterField_clicked()
{
    loadClosestCenterField();
}

// ------------------------------------------------------------
// Sort By Name Button
// ------------------------------------------------------------

void MainWindow::sortByName()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name"
    FROM MLBInformation
    ORDER BY "Team name" ASC;
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");

    resizeDistanceTableColumns();
}

void MainWindow::on_SortByName_clicked()
{
    sortByName();
}

// ------------------------------------------------------------
// Sort By Stadium Button
// ------------------------------------------------------------

void MainWindow::sortByStadium()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name"
    FROM MLBInformation
    ORDER BY "Stadium name" ASC;
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");

    resizeDistanceTableColumns();
}

void MainWindow::on_SortByStadium_clicked()
{
    sortByStadium();
}

// ------------------------------------------------------------
// Display American League Button
// ------------------------------------------------------------

void MainWindow::displayALTeams()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name"
    FROM MLBInformation
    WHERE League = "American"
    ORDER BY "Team name" ASC;
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");

    resizeDistanceTableColumns();
}


void MainWindow::on_SortByAL_clicked()
{
    displayALTeams();
}

// ------------------------------------------------------------
// Display National League Button
// ------------------------------------------------------------

void MainWindow::displayNLTeams()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name"
    FROM MLBInformation
    WHERE League = "National"
    ORDER BY "Stadium name" ASC;
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");

    resizeDistanceTableColumns();
}

void MainWindow::on_SortByNL_clicked()
{
    displayNLTeams();
}

// ------------------------------------------------------------
// Sort By Topology Button
// ------------------------------------------------------------

void MainWindow::sortByTopology()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name", "Ballpark typology"
    FROM MLBInformation
    ORDER BY "Ballpark typology" ASC;
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");
    m_distanceModel->setHeaderData(2, Qt::Horizontal, "Ballpark Typology");

    resizeDistanceTableColumns();
}

void MainWindow::on_SortByTypology_clicked()
{
    sortByTopology();
}

// ------------------------------------------------------------
// Sort By Open Roof
// ------------------------------------------------------------

void MainWindow::sortByOpenRoof()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Roof Type"
    FROM MLBInformation
    WHERE "Roof Type" = "Open";
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");
    m_distanceModel->setHeaderData(2, Qt::Horizontal, "Ballpark Typology");

    resizeDistanceTableColumns();
}

void MainWindow::on_SortByOpenRoof_clicked()
{
    sortByOpenRoof();
}

// ------------------------------------------------------------
// Display All Info Button
// ------------------------------------------------------------

void MainWindow::displayAllInformation()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT *
    FROM MLBInformation;
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");
    m_distanceModel->setHeaderData(2, Qt::Horizontal, "Seating Capacity");
    m_distanceModel->setHeaderData(3, Qt::Horizontal, "Location");
    m_distanceModel->setHeaderData(4, Qt::Horizontal, "Playing Surface");
    m_distanceModel->setHeaderData(5, Qt::Horizontal, "League");
    m_distanceModel->setHeaderData(6, Qt::Horizontal, "Date Opened");
    m_distanceModel->setHeaderData(7, Qt::Horizontal, "Distance to Center Field");
    m_distanceModel->setHeaderData(8, Qt::Horizontal, "Ballpark Typology");
    m_distanceModel->setHeaderData(9, Qt::Horizontal, "Roof Type");

    resizeDistanceTableColumns();
}

void MainWindow::on_DisplayAll_clicked()
{
    displayAllInformation();
}

// ------------------------------------------------------------
// Sort By Date Opened Button
// ------------------------------------------------------------

void MainWindow::sortByNewestOpened()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name", "Date Opened"
    FROM MLBInformation
    ORDER BY "Date Opened" ASC;
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");
    m_distanceModel->setHeaderData(2, Qt::Horizontal, "Date Opened");

    resizeDistanceTableColumns();
}


void MainWindow::on_SortByNewest_clicked()
{
    sortByNewestOpened();
}

// ------------------------------------------------------------
// Sort By Smallest Capacity Button
// ------------------------------------------------------------

void MainWindow::sortBySmallestCapacity()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name", "Seating Capacity"
    FROM MLBInformation
    ORDER BY  "Seating Capacity"  ASC;
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");
    m_distanceModel->setHeaderData(2, Qt::Horizontal, "Date Opened");

    resizeDistanceTableColumns();
}

void MainWindow::on_SortByCapacity_clicked()
{
    sortBySmallestCapacity();
}

// ------------------------------------------------------------
// Load Furthest Center Field Button
// ------------------------------------------------------------

void MainWindow::loadFurthestCenterField()
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery query(m_db);

    query.prepare(R"(
    SELECT "Team name", "Stadium name", "Distance to center field"
    FROM MLBInformation
    WHERE "Distance to center field" = (SELECT MAX("Distance to center field")
    FROM MLBInformation);
)");

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Closest stadium query failed:\n" +
                                  query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              m_distanceModel->lastError().text());
        return;
    }

    // Set headers for UI table
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Team Name");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Stadium Name");
    m_distanceModel->setHeaderData(2, Qt::Horizontal, "Distance to Center Field");

    resizeDistanceTableColumns();
}

void MainWindow::on_FurthestCenterField_clicked()
{
    loadFurthestCenterField();
}

//Opens SpecialAlgorithms window
void MainWindow::on_buttonSpecAlgorithms_clicked()
{
    auto *win = new SearchAlgorithms(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close();
}
