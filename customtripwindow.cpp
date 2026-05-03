#include "customtripwindow.h"
#include "ui_customtripwindow.h"

#include "tripwindow.h"
#include "mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QTableWidgetItem>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

CustomTripWindow::CustomTripWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CustomTripWindow)
{
    ui->setupUi(this);
    setWindowTitle("Custom Trip");

    ui->selectStartingCollegeDropdownCT->clear();

    if (ensureDbOpen())
        m_allCampuses = loadCampuses();

    for (const QString &c : m_allCampuses)
        ui->selectStartingCollegeDropdownCT->addItem(c);

    // No default college selected
    ui->selectStartingCollegeDropdownCT->setCurrentIndex(-1);

    // Prep table
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Campuses");
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
        candidates << d.filePath("college_tour.sqlite");
        if (!d.cdUp())
            break;
    }

    // Also try current working directory (Qt Creator often sets this to the build folder)
    candidates << QDir::current().filePath("college_tour.sqlite");

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

QStringList CustomTripWindow::loadCampuses()
{
    QStringList campuses;

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT campus
        FROM campus_access
        WHERE enabled = 1
            AND TRIM(campus) <> ''
        ORDER BY campus ASC
    )");

    if (!q.exec())
        return campuses;

    while (q.next())
    {
        const QString c = q.value(0).toString().trimmed();
        if (!c.isEmpty())
            campuses << c;
    }
    return campuses;
}

void CustomTripWindow::on_confirmButtonCT_clicked()
{
    m_startLocked = ui->selectStartingCollegeDropdownCT->currentText().trimmed();
    if (m_startLocked.isEmpty())
    {
        QMessageBox::information(this, "Select Start",
                                 "Please select a starting college first.");
        return;
    }

    ui->selectStartingCollegeDropdownCT->setEnabled(false);
    ui->confirmButtonCT->setEnabled(false);

    QStringList remaining;
    for (const QString &c : m_allCampuses)
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
                                 "Click Confirm to lock in your starting college first.");
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
        QMessageBox::information(this, "No Campuses Selected",
                                 "Select at least one campus to visit in your custom trip.");
        return;
    }

    // Close this setup window so TripWindow is the only one open.
    this->close();

    //tripWindow dlg(m_startLocked, selected, selected.size(), /*forceExact=*/true, nullptr);
    tripWindow dlg(m_startLocked, selected, selected.size(), /*forceExact=*/false, nullptr);
    dlg.setModal(true);
    dlg.exec();

    if (parentWidget())
        parentWidget()->show();
}

void CustomTripWindow::on_backButtCT_clicked()
{
    MainWindow *mainWin = new MainWindow(nullptr);
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    mainWin->show();

    this->close();
}
