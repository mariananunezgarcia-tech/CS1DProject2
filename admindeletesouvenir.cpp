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

AdminDeleteSouvenir::AdminDeleteSouvenir(const QString &stadium, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDeleteSouvenir)
    , m_stadium(stadium.trimmed())
{
    ui->setupUi(this);
    setWindowTitle("Delete Souvenir");

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
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
    return db.open();
}

void AdminDeleteSouvenir::loadSouvenirDropdown()
{
    ui->deleteSouvCombo->clear();
    ui->deleteSouvCombo->addItem("");

    if (m_stadium.isEmpty())
        return;

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT TRIM(Name)
        FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
          AND TRIM(Name) <> ''
        ORDER BY TRIM(Name) ASC
    )");

    q.bindValue(":stadium", m_stadium);

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load souvenirs for selected stadium:\n" +
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

    if (m_stadium.isEmpty())
    {
        QMessageBox::warning(this, "Missing Stadium",
                             "No stadium was selected.");
        return;
    }

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
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
        DELETE FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
          AND TRIM(Name) = TRIM(:item)
    )");

    deleteSouvenir.bindValue(":stadium", m_stadium);
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
        WHERE TRIM(stadium) = TRIM(:stadium)
          AND TRIM(item) = TRIM(:item)
    )");

    deleteAccess.bindValue(":stadium", m_stadium);
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
    win->setSelectedStadium(m_stadium);

    this->close();
}

void AdminDeleteSouvenir::on_delSouvBack_clicked()
{
    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    win->setSelectedStadium(m_stadium);

    this->close();
}
