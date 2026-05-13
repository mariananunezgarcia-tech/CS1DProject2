#ifndef ADMINCHANGEPRICE_H
#define ADMINCHANGEPRICE_H

#include <QWidget>
#include <QString>

namespace Ui {
class adminChangePrice;
}

class adminChangePrice : public QWidget
{
    Q_OBJECT

public:
    explicit adminChangePrice(const QString &stadium, QWidget *parent = nullptr);
    ~adminChangePrice();

private slots:
    void on_changePrConfirm_clicked();
    void on_changePrBack_clicked();

private:
    Ui::adminChangePrice *ui;

    QString m_stadium;

    bool ensureDbOpen();
    void loadSouvenirsForStadium();
};

#endif // ADMINCHANGEPRICE_H
