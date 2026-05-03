#include "admindeletesouvenir.h"
#include "ui_admindeletesouvenir.h"
#include "modsouvenirs.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

AdminDeleteSouvenir::AdminDeleteSouvenir(const QString &campus, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDeleteSouvenir)
    , m_campus(campus.trimmed())
{
    ui->setupUi(this);
    setWindowTitle("Delete Souvenir");

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open college_tour.sqlite.");
        return;
    }

    loadSouvenirDropdown();
}

AdminDeleteSouvenir::~AdminDeleteSouvenir()
{
    delete ui;
}

bool AdminDeleteSouvenir::ensureDbOpen()
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

void AdminDeleteSouvenir::loadSouvenirDropdown()
{
    ui->deleteSouvCombo->clear();
    ui->deleteSouvCombo->addItem("");

    if (m_campus.isEmpty())
        return;

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT TRIM(item)
        FROM souvenirs
        WHERE TRIM(campus) = :campus
          AND TRIM(item) <> ''
        ORDER BY TRIM(item) ASC
    )");
    q.bindValue(":campus", m_campus);

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load souvenirs for selected college:\n" +
                                 q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString item = q.value(0).toString().trimmed();
        if (!item.isEmpty())
            ui->deleteSouvCombo->addItem(item);
    }

    ui->deleteSouvCombo->setCurrentIndex(0);
}

void AdminDeleteSouvenir::on_delSouvConfirm_clicked()
{
    const QString itemName = ui->deleteSouvCombo->currentText().trimmed();

    if (itemName.isEmpty())
    {
        QMessageBox::warning(this, "Missing Souvenir",
                             "Select the souvenir you want to delete.");
        return;
    }

    if (m_campus.isEmpty())
    {
        QMessageBox::warning(this, "Missing College",
                             "No college was selected.");
        return;
    }

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open college_tour.sqlite.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction())
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not start database transaction.");
        return;
    }

    QSqlQuery deleteSouvenir(db);
    deleteSouvenir.prepare(R"(
        DELETE FROM souvenirs
        WHERE TRIM(campus) = :campus
          AND TRIM(item) = :item
    )");
    deleteSouvenir.bindValue(":campus", m_campus);
    deleteSouvenir.bindValue(":item", itemName);

    if (!deleteSouvenir.exec())
    {
        db.rollback();
        QMessageBox::warning(this, "Delete Error",
                             "Could not delete souvenir:\n" +
                                 deleteSouvenir.lastError().text());
        return;
    }

    QSqlQuery deleteAccess(db);
    deleteAccess.prepare(R"(
        DELETE FROM souvenir_access
        WHERE TRIM(campus) = :campus
          AND TRIM(item) = :item
    )");
    deleteAccess.bindValue(":campus", m_campus);
    deleteAccess.bindValue(":item", itemName);

    if (!deleteAccess.exec())
    {
        db.rollback();
        QMessageBox::warning(this, "Delete Error",
                             "Could not delete souvenir access entry:\n" +
                                 deleteAccess.lastError().text());
        return;
    }

    if (!db.commit())
    {
        db.rollback();
        QMessageBox::warning(this, "Database Error",
                             "Could not commit souvenir deletion.");
        return;
    }

    QMessageBox::information(this, "Souvenir Deleted",
                             "The souvenir was deleted successfully.");

    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    win->setSelectedCampus(m_campus);
    this->close();
}

void AdminDeleteSouvenir::on_delSouvBack_clicked()
{
    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    win->setSelectedCampus(m_campus);
    this->close();
}
