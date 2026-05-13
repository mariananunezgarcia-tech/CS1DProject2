#include "adminchangeprice.h"
#include "ui_adminchangeprice.h"
#include "modsouvenirs.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

adminChangePrice::adminChangePrice(const QString &stadium, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminChangePrice)
    , m_stadium(stadium.trimmed())
{
    ui->setupUi(this);
    setWindowTitle("Change Price");

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return;
    }

    loadSouvenirsForStadium();
}

adminChangePrice::~adminChangePrice()
{
    delete ui;
}

bool adminChangePrice::ensureDbOpen()
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

void adminChangePrice::loadSouvenirsForStadium()
{
    ui->changePrCombo->clear();

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
        QMessageBox::warning(this,
                             "Query Error",
                             "Could not load souvenirs for this stadium:\n" +
                                 q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString item = q.value(0).toString().trimmed();

        if (!item.isEmpty())
            ui->changePrCombo->addItem(item);
    }

    ui->changePrCombo->setCurrentIndex(-1);
}

void adminChangePrice::on_changePrConfirm_clicked()
{
    const QString item = ui->changePrCombo->currentText().trimmed();
    const QString priceText = ui->enterNewPrice->text().trimmed();

    if (m_stadium.isEmpty())
    {
        QMessageBox::warning(this,
                             "Missing Stadium",
                             "No stadium was selected.");
        return;
    }

    if (item.isEmpty())
    {
        QMessageBox::warning(this,
                             "Missing Souvenir",
                             "Select a souvenir first.");
        return;
    }

    bool ok = false;
    const double newPrice = priceText.toDouble(&ok);

    if (!ok)
    {
        QMessageBox::warning(this,
                             "Invalid Price",
                             "Enter a valid numeric price.");
        return;
    }

    if (newPrice <= 0.0)
    {
        QMessageBox::warning(this,
                             "Invalid Price",
                             "Price must be greater than 0.00.");
        return;
    }

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return;
    }

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        UPDATE SouvenirList
        SET Price = :price
        WHERE TRIM(Stadium) = TRIM(:stadium)
          AND TRIM(Name) = TRIM(:item)
    )");

    q.bindValue(":price", newPrice);
    q.bindValue(":stadium", m_stadium);
    q.bindValue(":item", item);

    if (!q.exec())
    {
        QMessageBox::warning(this,
                             "Update Error",
                             "Could not update souvenir price:\n" +
                                 q.lastError().text());
        return;
    }

    if (q.numRowsAffected() <= 0)
    {
        QMessageBox::warning(this,
                             "Update Error",
                             "No souvenir price was updated.");
        return;
    }

    QMessageBox::information(this,
                             "Price Updated",
                             "The souvenir price was updated successfully.");

    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    win->setSelectedStadium(m_stadium);

    this->close();
}

void adminChangePrice::on_changePrBack_clicked()
{
    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    win->setSelectedStadium(m_stadium);

    this->close();
}
