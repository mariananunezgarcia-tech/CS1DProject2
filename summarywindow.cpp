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
    // Colleges visited list
    // ------------------------------------------------------------
    QStringList collegeLines;

    if (!route.empty())
        collegeLines << QString("Start: %1").arg(route[0]);

    for (size_t i = 1; i < route.size(); ++i)
    {
        const double d = (i - 1 < legs.size()) ? legs[i - 1] : 0.0;

        collegeLines << QString("%1) %2 (+%3 miles)")
                            .arg(static_cast<int>(i))
                            .arg(route[i])
                            .arg(d, 0, 'f', 1);
    }

    m_model = new QStringListModel(collegeLines, this);
    ui->listColleges->setModel(m_model);

    // ------------------------------------------------------------
    // Souvenir summary list
    // ------------------------------------------------------------
    ui->listSouvenirs->clear();

    // Group purchases by college, then by souvenir name+price
    // college -> (souvenirKey -> quantity)
    QMap<QString, QMap<QString, int>> quantityByCollege;
    QMap<QString, QMap<QString, double>> priceByCollege;

    for (const auto &p : purchases)
    {
        const QString college = p.college.trimmed();
        const QString souvenirKey =
            QString("%1|||%2")
                .arg(p.name.trimmed())
                .arg(QString::number(p.price, 'f', 2));

        quantityByCollege[college][souvenirKey] += 1;
        priceByCollege[college][souvenirKey] = p.price;
    }

    // Show every visited college, even if no souvenirs were bought there
    for (const QString &college : route)
    {
        ui->listSouvenirs->addItem(QString("College: %1").arg(college));

        int collegeCount = 0;
        double collegeCost = 0.0;

        const QMap<QString, int> souvenirMap = quantityByCollege.value(college);

        if (!souvenirMap.isEmpty())
        {
            for (auto it = souvenirMap.constBegin(); it != souvenirMap.constEnd(); ++it)
            {
                const QString key = it.key();
                const int quantity = it.value();

                const QStringList parts = key.split("|||");
                const QString souvenirName = parts.value(0);
                const double price = priceByCollege.value(college).value(key, 0.0);

                collegeCount += quantity;
                collegeCost += price * quantity;

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
                .arg(college)
                .arg(collegeCount)
                .arg(collegeCost, 0, 'f', 2)
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
