#include "basictripwindow.h"
#include "ui_basictripwindow.h"

#include "tripwindow.h"
#include "mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

BasicTripWindow::BasicTripWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BasicTripWindow)
{
    ui->setupUi(this);
    setWindowTitle("Basic Trip");

    if (ensureDbOpen())
        loadCampusesFromDb();

    // Number of visitable colleges after choosing a starting college
    rebuildNumDropdown(ui->selectStartingCollegeDropdownBT->count() - 1);

    // No default college selected
    ui->selectStartingCollegeDropdownBT->setCurrentIndex(-1);
}

BasicTripWindow::~BasicTripWindow()
{
    delete ui;
}

bool BasicTripWindow::ensureDbOpen()
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

void BasicTripWindow::loadCampusesFromDb()
{
    ui->selectStartingCollegeDropdownBT->clear();

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT campus
        FROM campus_access
        WHERE enabled = 1
            AND TRIM(campus) <> ''
        ORDER BY campus ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load campus list:\n" + q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString c = q.value(0).toString().trimmed();
        if (!c.isEmpty())
            ui->selectStartingCollegeDropdownBT->addItem(c);
    }
}

void BasicTripWindow::rebuildNumDropdown(int campusCount)
{
    ui->numCollegestoVisitDropdownBT->clear();

    // campusCount = how many OTHER colleges can still be visited
    if (campusCount < 0)
        campusCount = 0;

    ui->numCollegestoVisitDropdownBT->setEnabled(true);

    // Allow visiting zero additional colleges
    for (int i = 0; i <= campusCount; ++i)
        ui->numCollegestoVisitDropdownBT->addItem(QString::number(i));
}

void BasicTripWindow::on_startTripButtonBT_clicked()
{
    const QString start = ui->selectStartingCollegeDropdownBT->currentText().trimmed();
    const int maxStops = ui->numCollegestoVisitDropdownBT->currentText().toInt();

    if (start.isEmpty())
    {
        QMessageBox::information(this, "Select Start",
                                 "Please select a starting college.");
        return;
    }

    QStringList campuses;
    for (int i = 0; i < ui->selectStartingCollegeDropdownBT->count(); ++i)
        campuses << ui->selectStartingCollegeDropdownBT->itemText(i);

    this->close();

    tripWindow dlg(start, campuses, maxStops, /*forceExact=*/false, nullptr);
    dlg.setModal(true);
    dlg.exec();

    if (parentWidget())
        parentWidget()->show();
}

void BasicTripWindow::on_backButtonBT_clicked()
{
    MainWindow *mainWin = new MainWindow(nullptr);
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    mainWin->show();

    this->close();
}
