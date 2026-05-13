#include "customtripwindow.h"
#include "ui_customtripwindow.h"

CustomTripWindow::CustomTripWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CustomTripWindow)
{
    ui->setupUi(this);
    setWindowTitle("Custom Trip");

    ui->selectStartingStadiumDropdownCT->clear();

    if (ensureDbOpen())
        m_allStadiums = loadStadiums();

    for (const QString &c : m_allStadiums)
        ui->selectStartingStadiumDropdownCT->addItem(c);

    // No default stadium selected
    ui->selectStartingStadiumDropdownCT->setCurrentIndex(-1);

    // Prep table
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Stadiums");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setRowCount(0);
}

CustomTripWindow::~CustomTripWindow()
{
    delete ui;
}

bool CustomTripWindow::ensureDbOpen()
{
    QSqlDatabase db;

    if (QSqlDatabase::contains())
    {
        db = QSqlDatabase::database();
        if (db.isOpen())
            return true;
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    const QString exeDir = QCoreApplication::applicationDirPath();
    QStringList candidates;

    // Look in exe dir and walk upward a few levels (helps when you keep the DB in the project root)
    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("BaseballDatabase.sqlite");
        if (!d.cdUp())
            break;
    }

    // Also try current working directory (Qt Creator often sets this to the build folder)
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
    return db.open();
}

QStringList CustomTripWindow::loadStadiums()
{
    QStringList allStadiums;

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT DISTINCT TRIM(sa.stadium) AS stadium
        FROM stadium_access sa
        WHERE sa.enabled = 1
          AND TRIM(sa.stadium) <> ''
          AND EXISTS (
              SELECT 1
              FROM MLBDistances d
              WHERE TRIM(d."Beginning Stadium") = TRIM(sa.stadium)
                 OR TRIM(d."Ending Stadium") = TRIM(sa.stadium)
          )
        ORDER BY TRIM(sa.stadium) ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load enabled stadium list:\n" +
                                 q.lastError().text());
        return allStadiums;
    }

    while (q.next())
    {
        const QString stadium = q.value(0).toString().trimmed();

        if (!stadium.isEmpty())
            allStadiums << stadium;
    }

    return allStadiums;
}

void CustomTripWindow::on_confirmButtonCT_clicked()
{
    m_startLocked = ui->selectStartingStadiumDropdownCT->currentText().trimmed();
    if (m_startLocked.isEmpty())
    {
        QMessageBox::information(this, "Select Start",
                                 "Please select a starting stadium first.");
        return;
    }

    ui->selectStartingStadiumDropdownCT->setEnabled(false);
    ui->confirmButtonCT->setEnabled(false);

    QStringList remaining;
    for (const QString &c : m_allStadiums)
    {
        if (c.compare(m_startLocked, Qt::CaseInsensitive) != 0)
            remaining << c;
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(remaining.size());
    ui->tableWidget->setColumnCount(1);

    for (int r = 0; r < remaining.size(); ++r)
    {
        auto *item = new QTableWidgetItem(remaining[r]);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(r, 0, item);
    }
}

void CustomTripWindow::on_startTripButtonCT_clicked()
{
    if (m_startLocked.isEmpty())
    {
        QMessageBox::information(this, "Select Start",
                                 "Click Confirm to lock in your starting stadium first.");
        return;
    }

    QStringList selected;
    selected << m_startLocked;

    for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
    {
        QTableWidgetItem *item = ui->tableWidget->item(r, 0);
        if (item && item->checkState() == Qt::Checked)
            selected << item->text().trimmed();
    }

    if (selected.size() <= 1)
    {
        QMessageBox::information(this, "No Stadiums Selected",
                                 "Select at least one stadium to visit in your custom trip.");
        return;
    }

    // Hide this setup window so TripWindow is the only one open.
    this->hide();

    //tripWindow dlg(m_startLocked, selected, selected.size(), /*forceExact=*/true, nullptr);
    tripWindow dlg(m_startLocked, selected, selected.size(), /*forceExact=*/false, nullptr, "custom");
    dlg.setModal(true);
    dlg.exec();

    // Close the setup window once TripWindow is done
    this->close();
}

void CustomTripWindow::on_backButtCT_clicked()
{
    MainWindow *mainWin = new MainWindow(nullptr);
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    mainWin->show();

    this->close();
}
