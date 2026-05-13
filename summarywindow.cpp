#include "summarywindow.h"
#include "ui_summarywindow.h"

#include "mainwindow.h"

#include <QLineEdit>
#include <QStringList>
#include <QStringListModel>
#include <QMap>

summaryWindow::summaryWindow(const std::vector<QString> &route,
                             const std::vector<double> &legs,
                             double totalMiles,
                             const std::vector<PurchasedSouvenir> &purchases,
                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::summaryWindow)
{
    ui->setupUi(this);
    setWindowTitle("Trip Summary");

    m_totalMilesEdit    = ui->lineDistTrav;
    m_souvenirCountEdit = ui->lineSouvPurc;
    m_souvenirCostEdit  = ui->lineSouvCost;

    setReadOnly(m_totalMilesEdit);
    setReadOnly(m_souvenirCountEdit);
    setReadOnly(m_souvenirCostEdit);

    // ------------------------------------------------------------
    // Total distance
    // ------------------------------------------------------------
    if (m_totalMilesEdit)
        m_totalMilesEdit->setText(QString::number(totalMiles, 'f', 1));

    // ------------------------------------------------------------
    // Overall souvenir totals
    // ------------------------------------------------------------
    int totalSouvenirsPurchased = 0;
    double totalSouvenirCost = 0.0;

    for (const auto &p : purchases)
    {
        ++totalSouvenirsPurchased;
        totalSouvenirCost += p.price;
    }

    if (m_souvenirCountEdit)
        m_souvenirCountEdit->setText(QString::number(totalSouvenirsPurchased));

    if (m_souvenirCostEdit)
        m_souvenirCostEdit->setText(QString("$%1").arg(totalSouvenirCost, 0, 'f', 2));

    // ------------------------------------------------------------
    // Stadiums visited list
    // ------------------------------------------------------------
    QStringList stadiumLines;

    if (!route.empty())
        stadiumLines << QString("Start: %1").arg(route[0]);

    for (size_t i = 1; i < route.size(); ++i)
    {
        const double d = (i - 1 < legs.size()) ? legs[i - 1] : 0.0;

        stadiumLines << QString("%1) %2 (+%3 miles)")
                            .arg(static_cast<int>(i))
                            .arg(route[i])
                            .arg(d, 0, 'f', 1);
    }

    m_model = new QStringListModel(stadiumLines, this);
    ui->listStadiums->setModel(m_model);

    // ------------------------------------------------------------
    // Souvenir summary list
    // ------------------------------------------------------------
    ui->listSouvenirs->clear();

    // Group purchases by stadium, then by souvenir name+price
    // stadium -> (souvenirKey -> quantity)
    QMap<QString, QMap<QString, int>> quantityByStadium;
    QMap<QString, QMap<QString, double>> priceByStadium;

    for (const auto &p : purchases)
    {
        const QString stadium = p.stadium.trimmed();
        const QString souvenirKey =
            QString("%1|||%2")
                .arg(p.name.trimmed())
                .arg(QString::number(p.price, 'f', 2));

        quantityByStadium[stadium][souvenirKey] += 1;
        priceByStadium[stadium][souvenirKey] = p.price;
    }

    // Show every visited stadium, even if no souvenirs were bought there
    for (const QString &stadium : route)
    {
        ui->listSouvenirs->addItem(QString("Stadium: %1").arg(stadium));

        int stadiumCount = 0;
        double stadiumCost = 0.0;

        const QMap<QString, int> souvenirMap = quantityByStadium.value(stadium);

        if (!souvenirMap.isEmpty())
        {
            for (auto it = souvenirMap.constBegin(); it != souvenirMap.constEnd(); ++it)
            {
                const QString key = it.key();
                const int quantity = it.value();

                const QStringList parts = key.split("|||");
                const QString souvenirName = parts.value(0);
                const double price = priceByStadium.value(stadium).value(key, 0.0);

                stadiumCount += quantity;
                stadiumCost += price * quantity;

                ui->listSouvenirs->addItem(
                    QString("   Souvenir: %1 | Price: $%2 | Quantity: %3")
                        .arg(souvenirName)
                        .arg(price, 0, 'f', 2)
                        .arg(quantity)
                    );
            }
        }

        ui->listSouvenirs->addItem(
            QString("   Total at %1: %2 souvenirs purchased | Cost: $%3")
                .arg(stadium)
                .arg(stadiumCount)
                .arg(stadiumCost, 0, 'f', 2)
            );

        ui->listSouvenirs->addItem("");
    }
}

summaryWindow::~summaryWindow()
{
    delete ui;
}

void summaryWindow::setReadOnly(QLineEdit *edit)
{
    if (!edit)
        return;

    edit->setReadOnly(true);
    edit->setFocusPolicy(Qt::NoFocus);
}

void summaryWindow::on_backButtonSum_clicked()
{
    MainWindow *mainWin = new MainWindow(nullptr);
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    mainWin->show();

    this->close();
}
