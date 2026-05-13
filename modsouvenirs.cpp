#include "modsouvenirs.h"
#include "ui_modsouvenirs.h"
#include "admin.h"
#include "adminaddsouvenir.h"
#include "adminchangeprice.h"
#include "admindeletesouvenir.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QTableWidgetItem>
#include <QAbstractItemView>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

ModSouvenirs::ModSouvenirs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModSouvenirs)
{
    ui->setupUi(this);
    setWindowTitle("Modify Souvenirs");

    ui->SuvModTabel->setColumnCount(3);
    ui->SuvModTabel->setHorizontalHeaderLabels(QStringList() << "Enabled" << "Souvenir" << "Price");
    ui->SuvModTabel->horizontalHeader()->setStretchLastSection(true);
    ui->SuvModTabel->verticalHeader()->setVisible(false);
    ui->SuvModTabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->SuvModTabel->setSelectionMode(QAbstractItemView::NoSelection);
    ui->SuvModTabel->setRowCount(0);

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return;
    }

    ensureSouvenirAccessTable();
    syncSouvenirsIntoAccessTable();
    loadStadiumDropdown();
    resetToDefaultState();
}

ModSouvenirs::~ModSouvenirs()
{
    delete ui;
}

bool ModSouvenirs::ensureDbOpen()
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

void ModSouvenirs::ensureSouvenirAccessTable()
{
    QSqlDatabase db = QSqlDatabase::database();

    bool hasOldCampusColumn = false;
    bool hasNewStadiumColumn = false;

    QSqlQuery pragmaQ(db);

    if (pragmaQ.exec("PRAGMA table_info(souvenir_access)"))
    {
        while (pragmaQ.next())
        {
            const QString columnName = pragmaQ.value("name").toString();

            if (columnName.compare("campus", Qt::CaseInsensitive) == 0)
                hasOldCampusColumn = true;

            if (columnName.compare("stadium", Qt::CaseInsensitive) == 0)
                hasNewStadiumColumn = true;
        }
    }

    if (hasOldCampusColumn && !hasNewStadiumColumn)
    {
        QSqlQuery dropQ(db);

        if (!dropQ.exec("DROP TABLE souvenir_access"))
        {
            QMessageBox::warning(this,
                                 "Table Error",
                                 "Could not remove old souvenir_access table:\n" +
                                     dropQ.lastError().text());
            return;
        }
    }

    QSqlQuery q(db);

    q.prepare(R"(
        CREATE TABLE IF NOT EXISTS souvenir_access (
            stadium  TEXT NOT NULL,
            item     TEXT NOT NULL,
            enabled  INTEGER NOT NULL DEFAULT 1,
            PRIMARY KEY (stadium, item)
        )
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this,
                             "Table Error",
                             "Could not create/access souvenir_access table:\n" +
                                 q.lastError().text());
    }
}

void ModSouvenirs::syncSouvenirsIntoAccessTable()
{
    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        INSERT OR IGNORE INTO souvenir_access (stadium, item, enabled)
        SELECT TRIM("Stadium"), TRIM("Name"), 1
        FROM SouvenirList
        WHERE TRIM("Stadium") <> ''
          AND TRIM("Name") <> ''
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this,
                             "Sync Error",
                             "Could not sync souvenirs into souvenir_access:\n" +
                                 q.lastError().text());
    }
}

void ModSouvenirs::loadStadiumDropdown()
{
    QSignalBlocker blocker(ui->comboBox);

    ui->comboBox->clear();
    ui->comboBox->addItem("Select a Stadium");

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT DISTINCT TRIM(sa.stadium) AS stadium
        FROM stadium_access sa
        WHERE TRIM(sa.stadium) <> ''
        ORDER BY TRIM(sa.stadium) ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this,
                             "Query Error",
                             "Could not load stadium list:\n" +
                                 q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString stadium = q.value(0).toString().trimmed();

        if (!stadium.isEmpty())
            ui->comboBox->addItem(stadium);
    }

    ui->comboBox->setCurrentIndex(0);
}

void ModSouvenirs::clearSouvenirTable()
{
    ui->SuvModTabel->clearContents();
    ui->SuvModTabel->setRowCount(0);
}

void ModSouvenirs::resetToDefaultState()
{
    m_selectedStadium.clear();

    ui->comboBox->setEnabled(true);
    ui->SubmitButt->setEnabled(true);

    {
        QSignalBlocker blocker(ui->comboBox);
        ui->comboBox->setCurrentIndex(0);
    }

    clearSouvenirTable();

    ui->addSouvButt->hide();
    ui->delSouvButt->hide();
    ui->changePrButt->hide();
}

void ModSouvenirs::loadSouvenirsForStadium(const QString &stadium)
{
    clearSouvenirTable();

    const QString selectedStadium = stadium.trimmed();

    if (selectedStadium.isEmpty() ||
        selectedStadium.compare("Select a Stadium", Qt::CaseInsensitive) == 0)
    {
        return;
    }

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT sa.item, sa.enabled, s.Price
        FROM souvenir_access sa
        LEFT JOIN SouvenirList s
            ON TRIM(s.Stadium) = TRIM(sa.stadium)
           AND TRIM(s.Name)    = TRIM(sa.item)
        WHERE TRIM(sa.stadium) = :stadium
        ORDER BY sa.item ASC
    )");

    q.bindValue(":stadium", selectedStadium);

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load souvenirs for selected stadium:\n" +
                                 q.lastError().text());
        return;
    }

    int row = 0;

    while (q.next())
    {
        const QString item = q.value(0).toString().trimmed();
        const bool enabled = q.value(1).toInt() != 0;
        const double price = q.value(2).toDouble();

        ui->SuvModTabel->insertRow(row);

        auto *checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        checkItem->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
        ui->SuvModTabel->setItem(row, 0, checkItem);

        auto *nameItem = new QTableWidgetItem(item);
        nameItem->setFlags(Qt::ItemIsEnabled);
        ui->SuvModTabel->setItem(row, 1, nameItem);

        auto *priceItem = new QTableWidgetItem(QString::number(price, 'f', 2));
        priceItem->setFlags(Qt::ItemIsEnabled);
        ui->SuvModTabel->setItem(row, 2, priceItem);

        ++row;
    }

    ui->SuvModTabel->resizeColumnsToContents();
}

void ModSouvenirs::on_comboBox_currentIndexChanged(int)
{
    if (ui->comboBox->isEnabled())
    {
        clearSouvenirTable();
        ui->addSouvButt->hide();
        ui->delSouvButt->hide();
        ui->changePrButt->hide();
        m_selectedStadium.clear();
    }
}

void ModSouvenirs::on_SubmitButt_clicked()
{
    const QString stadium = ui->comboBox->currentText().trimmed();

    if (stadium.isEmpty() ||
        stadium.compare("Select a Stadium", Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium first.");
        return;
    }

    m_selectedStadium = stadium;

    loadSouvenirsForStadium(m_selectedStadium);

    ui->comboBox->setEnabled(false);
    ui->SubmitButt->setEnabled(false);

    ui->addSouvButt->show();
    ui->delSouvButt->show();
    ui->changePrButt->show();
}

void ModSouvenirs::on_cancelButt_clicked()
{
    resetToDefaultState();
}

void ModSouvenirs::on_SuvModifybutton_clicked()
{
    const QString stadium = m_selectedStadium.trimmed();

    if (stadium.isEmpty())
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium and click Submit first.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction())
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not start database transaction.");
        return;
    }

    QSqlQuery updateQ(db);

    updateQ.prepare(R"(
        UPDATE souvenir_access
        SET enabled = :enabled
        WHERE TRIM(stadium) = TRIM(:stadium)
          AND TRIM(item) = TRIM(:item)
    )");

    bool ok = true;

    for (int row = 0; row < ui->SuvModTabel->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = ui->SuvModTabel->item(row, 0);
        QTableWidgetItem *nameItem  = ui->SuvModTabel->item(row, 1);

        if (!checkItem || !nameItem)
            continue;

        const QString item = nameItem->text().trimmed();
        const int enabled = (checkItem->checkState() == Qt::Checked) ? 1 : 0;

        updateQ.bindValue(":enabled", enabled);
        updateQ.bindValue(":stadium", stadium);
        updateQ.bindValue(":item", item);

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
                                 "Souvenir access has been updated.");
    }
    else
    {
        db.rollback();

        QMessageBox::warning(this, "Save Error",
                             "Could not save souvenir settings:\n" +
                                 updateQ.lastError().text());
    }
}

int ModSouvenirs::souvenirCountForStadium(const QString &stadium)
{
    if (stadium.trimmed().isEmpty())
        return -1;

    if (!ensureDbOpen())
        return -1;

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery q(db);

    q.prepare(R"(
        SELECT COUNT(*)
        FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
    )");

    q.bindValue(":stadium", stadium.trimmed());

    if (!q.exec() || !q.next())
        return -1;

    return q.value(0).toInt();
}

void ModSouvenirs::on_addSouvButt_clicked()
{
    if (m_selectedStadium.isEmpty())
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium and click Submit first.");
        return;
    }

    const int souvenirCount = souvenirCountForStadium(m_selectedStadium);

    if (souvenirCount < 0)
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not verify the number of souvenirs for this stadium.");
        return;
    }

    if (souvenirCount >= 7)
    {
        QMessageBox::information(this, "Maximum Souvenirs Reached",
                                 "Stadiums cannot have more than 7 souvenirs at once.");
        return;
    }

    auto *win = new adminAddSouvenir(m_selectedStadium, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

void ModSouvenirs::on_pushButton_clicked()
{
    auto *win = new Admin(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->hide();
}

void ModSouvenirs::on_changePrButt_clicked()
{
    if (m_selectedStadium.isEmpty())
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium and click Submit first.");
        return;
    }

    auto *win = new adminChangePrice(m_selectedStadium, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

void ModSouvenirs::on_delSouvButt_clicked()
{
    if (m_selectedStadium.isEmpty())
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium and click Submit first.");
        return;
    }

    auto *win = new AdminDeleteSouvenir(m_selectedStadium, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

void ModSouvenirs::setSelectedStadium(const QString &stadium)
{
    const QString trimmedStadium = stadium.trimmed();

    if (trimmedStadium.isEmpty() ||
        trimmedStadium.compare("Select a Stadium", Qt::CaseInsensitive) == 0)
    {
        resetToDefaultState();
        return;
    }

    int index = ui->comboBox->findText(trimmedStadium, Qt::MatchFixedString);

    if (index < 0)
    {
        resetToDefaultState();
        return;
    }

    {
        QSignalBlocker blocker(ui->comboBox);
        ui->comboBox->setCurrentIndex(index);
    }

    m_selectedStadium = trimmedStadium;

    loadSouvenirsForStadium(m_selectedStadium);

    ui->comboBox->setEnabled(false);
    ui->SubmitButt->setEnabled(false);

    ui->addSouvButt->show();
    ui->delSouvButt->show();
    ui->changePrButt->show();
}

void ModSouvenirs::on_SuvResetButt_clicked()
{
    const QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Reset Souvenirs",
            "Are you sure you want to reset the list of souvenirs for all stadiums back to their original states?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

    if (reply == QMessageBox::No)
    {
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

    if (!db.transaction())
    {
        QMessageBox::warning(this,
                             "Database Error",
                             "Could not start database transaction.");
        return;
    }

    QSqlQuery q(db);

    // Clear current souvenir list.
    if (!q.exec(R"(
        DELETE FROM SouvenirList
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not clear SouvenirList:\n" +
                                 q.lastError().text());
        return;
    }

    // Restore original souvenirs from DefaultSouvenirList.
    if (!q.exec(R"(
        INSERT INTO SouvenirList (Team, Stadium, Name, Price)
        SELECT Team, Stadium, Name, Price
        FROM DefaultSouvenirList
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not restore souvenirs from DefaultSouvenirList:\n" +
                                 q.lastError().text());
        return;
    }

    /*
        Rebuild souvenir_access so it matches the restored souvenir list.

        This prevents souvenir_access from keeping old deleted items,
        missing restored default items, or referencing souvenirs that
        no longer exist after the reset.
    */
    if (!q.exec(R"(
        DELETE FROM souvenir_access
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not clear souvenir_access:\n" +
                                 q.lastError().text());
        return;
    }

    if (!q.exec(R"(
        INSERT INTO souvenir_access (stadium, item, enabled)
        SELECT Stadium, Name, 1
        FROM DefaultSouvenirList
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not rebuild souvenir_access:\n" +
                                 q.lastError().text());
        return;
    }

    if (!db.commit())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "Could not commit souvenir reset.");
        return;
    }

    QMessageBox::information(this,
                             "Souvenirs Reset",
                             "All souvenirs have been reset.");

    auto *win = new Admin(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}
