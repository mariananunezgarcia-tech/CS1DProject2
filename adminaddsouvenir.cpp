#include "adminaddsouvenir.h"
#include "ui_adminaddsouvenir.h"
#include "modsouvenirs.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

adminAddSouvenir::adminAddSouvenir(const QString &stadium, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminAddSouvenir)
    , m_stadium(stadium.trimmed())
{
    ui->setupUi(this);
    setWindowTitle("Add Souvenir");
}

adminAddSouvenir::~adminAddSouvenir()
{
    delete ui;
}

bool adminAddSouvenir::ensureDbOpen()
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

void adminAddSouvenir::on_addSouvConfirm_clicked()
{
    const QString itemName = ui->souvName->text().trimmed();
    const QString priceText = ui->souvName_2->text().trimmed();

    if (itemName.isEmpty())
    {
        QMessageBox::warning(this,
                             "Invalid Name",
                             "Souvenir name cannot be blank.");
        return;
    }

    bool ok = false;
    const double price = priceText.toDouble(&ok);

    if (!ok)
    {
        QMessageBox::warning(this,
                             "Invalid Price",
                             "Enter a valid numeric price.");
        return;
    }

    if (price <= 0.0)
    {
        QMessageBox::warning(this,
                             "Invalid Price",
                             "Price must be greater than 0.00.");
        return;
    }

    if (m_stadium.isEmpty())
    {
        QMessageBox::warning(this,
                             "Missing Stadium",
                             "No stadium was selected.");
        return;
    }

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();

    // Enforce max of 7 souvenirs per stadium.
    QSqlQuery countQ(db);

    countQ.prepare(R"(
        SELECT COUNT(*)
        FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
    )");

    countQ.bindValue(":stadium", m_stadium);

    if (!countQ.exec() || !countQ.next())
    {
        QMessageBox::warning(this,
                             "Database Error",
                             "Could not verify how many souvenirs this stadium already has.");
        return;
    }

    if (countQ.value(0).toInt() >= 7)
    {
        QMessageBox::information(this,
                                 "Maximum Souvenirs Reached",
                                 "Stadiums cannot have more than 7 souvenirs at once.");
        return;
    }

    if (!db.transaction())
    {
        QMessageBox::warning(this,
                             "Database Error",
                             "Could not start database transaction.");
        return;
    }

    // Prevent duplicate souvenir names at the same stadium.
    QSqlQuery dupQ(db);

    dupQ.prepare(R"(
        SELECT COUNT(*)
        FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
          AND TRIM(Name) = TRIM(:item)
    )");

    dupQ.bindValue(":stadium", m_stadium);
    dupQ.bindValue(":item", itemName);

    if (!dupQ.exec() || !dupQ.next())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "Could not validate souvenir name.");
        return;
    }

    if (dupQ.value(0).toInt() > 0)
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Duplicate Souvenir",
                             "That stadium already has a souvenir with this name.");
        return;
    }

    // Look up the MLB team that belongs to this stadium.
    QString teamName;

    QSqlQuery teamQ(db);

    teamQ.prepare(R"(
        SELECT TRIM("Team name")
        FROM MLBInformation
        WHERE TRIM("Stadium name") = TRIM(:stadium)
        LIMIT 1
    )");

    teamQ.bindValue(":stadium", m_stadium);

    if (!teamQ.exec() || !teamQ.next())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "Could not find the team name for this stadium.");
        return;
    }

    teamName = teamQ.value(0).toString().trimmed();

    if (teamName.isEmpty())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "The selected stadium does not have a valid team name.");
        return;
    }

    // Insert into the baseball souvenir table.
    // Important: include Team so it does not become NULL.
    QSqlQuery insertSouvenir(db);

    insertSouvenir.prepare(R"(
        INSERT INTO SouvenirList (Team, Stadium, Name, Price)
        VALUES (:team, :stadium, :item, :price)
    )");

    insertSouvenir.bindValue(":team", teamName);
    insertSouvenir.bindValue(":stadium", m_stadium);
    insertSouvenir.bindValue(":item", itemName);
    insertSouvenir.bindValue(":price", price);

    if (!insertSouvenir.exec())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Insert Error",
                             "Could not add souvenir:\n" +
                                 insertSouvenir.lastError().text());
        return;
    }

    // Make sure the access table exists using baseball field names.
    QSqlQuery ensureAccess(db);

    ensureAccess.prepare(R"(
        CREATE TABLE IF NOT EXISTS souvenir_access (
            stadium  TEXT NOT NULL,
            item     TEXT NOT NULL,
            enabled  INTEGER NOT NULL DEFAULT 1,
            PRIMARY KEY (stadium, item)
        )
    )");

    if (!ensureAccess.exec())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Table Error",
                             "Could not access souvenir_access table:\n" +
                                 ensureAccess.lastError().text());
        return;
    }

    // New souvenirs should start enabled.
    QSqlQuery insertAccess(db);

    insertAccess.prepare(R"(
        INSERT OR REPLACE INTO souvenir_access (stadium, item, enabled)
        VALUES (:stadium, :item, 1)
    )");

    insertAccess.bindValue(":stadium", m_stadium);
    insertAccess.bindValue(":item", itemName);

    if (!insertAccess.exec())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Insert Error",
                             "Could not enable new souvenir:\n" +
                                 insertAccess.lastError().text());
        return;
    }

    if (!db.commit())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "Could not commit souvenir changes.");
        return;
    }

    QMessageBox::information(this,
                             "Souvenir Added",
                             "The new souvenir was added successfully.");

    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    win->setSelectedStadium(m_stadium);

    this->close();
}

void adminAddSouvenir::on_addSouvBack_clicked()
{
    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    win->setSelectedStadium(m_stadium);

    this->close();
}
