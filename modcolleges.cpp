#include "modcolleges.h"
#include "ui_modcolleges.h"
#include "admin.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

ModColleges::ModColleges(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModColleges)
{
    ui->setupUi(this);
    setWindowTitle("Modify Colleges");

    // Table setup
    ui->CollegeModtable->setColumnCount(2);
    ui->CollegeModtable->setHorizontalHeaderLabels(QStringList() << "Enabled" << "College");
    ui->CollegeModtable->horizontalHeader()->setStretchLastSection(true);
    ui->CollegeModtable->verticalHeader()->setVisible(false);
    ui->CollegeModtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->CollegeModtable->setSelectionMode(QAbstractItemView::NoSelection);

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open college_tour.sqlite.");
        return;
    }

    ensureCampusAccessTable();
    syncCampusesIntoAccessTable();
    loadCampusChecklist();
}

ModColleges::~ModColleges()
{
    delete ui;
}

bool ModColleges::ensureDbOpen()
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

    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("college_tour.sqlite");
        if (!d.cdUp())
            break;
    }

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

void ModColleges::ensureCampusAccessTable()
{
    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        CREATE TABLE IF NOT EXISTS campus_access (
            campus   TEXT PRIMARY KEY,
            enabled  INTEGER NOT NULL DEFAULT 1
        )
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Table Error",
                             "Could not create/access campus_access table:\n" + q.lastError().text());
    }
}

void ModColleges::syncCampusesIntoAccessTable()
{
    // Pull every campus from distances
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT DISTINCT TRIM(name) AS campus
        FROM (
            SELECT from_campus AS name FROM distances
            UNION
            SELECT to_campus AS name FROM distances
        )
        WHERE TRIM(name) <> ''
        ORDER BY TRIM(name) ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load campuses:\n" + q.lastError().text());
        return;
    }

    QSqlQuery insertQ(QSqlDatabase::database());
    insertQ.prepare(R"(
        INSERT OR IGNORE INTO campus_access (campus, enabled)
        VALUES (:campus, 1)
    )");

    while (q.next())
    {
        const QString campus = q.value(0).toString().trimmed();
        if (campus.isEmpty())
            continue;

        insertQ.bindValue(":campus", campus);
        insertQ.exec();
    }
}

void ModColleges::loadCampusChecklist()
{
    ui->CollegeModtable->setRowCount(0);

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT campus, enabled
        FROM campus_access
        WHERE TRIM(campus) <> ''
        ORDER BY campus ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load campus access list:\n" + q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next())
    {
        const QString campus = q.value(0).toString().trimmed();
        const bool enabled = q.value(1).toInt() != 0;

        ui->CollegeModtable->insertRow(row);

        // Column 0: checkbox
        auto *checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        checkItem->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
        ui->CollegeModtable->setItem(row, 0, checkItem);

        // Column 1: college name
        auto *nameItem = new QTableWidgetItem(campus);
        nameItem->setFlags(Qt::ItemIsEnabled);
        ui->CollegeModtable->setItem(row, 1, nameItem);

        ++row;
    }

    ui->CollegeModtable->resizeColumnsToContents();
}

void ModColleges::on_ColModifybutton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction())
    {
        QMessageBox::warning(this, "Database Error", "Could not start transaction.");
        return;
    }

    QSqlQuery updateQ(db);
    updateQ.prepare(R"(
        UPDATE campus_access
        SET enabled = :enabled
        WHERE campus = :campus
    )");

    bool ok = true;

    for (int row = 0; row < ui->CollegeModtable->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = ui->CollegeModtable->item(row, 0);
        QTableWidgetItem *nameItem  = ui->CollegeModtable->item(row, 1);

        if (!checkItem || !nameItem)
            continue;

        const QString campus = nameItem->text().trimmed();
        const int enabled = (checkItem->checkState() == Qt::Checked) ? 1 : 0;

        updateQ.bindValue(":enabled", enabled);
        updateQ.bindValue(":campus", campus);

        if (!updateQ.exec())
        {
            ok = false;
            break;
        }
    }

    if (ok)
    {
        db.commit();
        QMessageBox::information(this, "Saved",
                                 "College access has been updated.");
    }
    else
    {
        db.rollback();
        QMessageBox::warning(this, "Save Error",
                             "Could not save changes.");
    }
}

void ModColleges::on_pushButton_clicked()
{
    auto *win = new Admin(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}
