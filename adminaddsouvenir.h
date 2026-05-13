#ifndef ADMINADDSOUVENIR_H
#define ADMINADDSOUVENIR_H

#include <QWidget>
#include <QString>

namespace Ui {
class adminAddSouvenir;
}

class adminAddSouvenir : public QWidget
{
    Q_OBJECT

public:
    explicit adminAddSouvenir(const QString &stadium, QWidget *parent = nullptr);
    ~adminAddSouvenir();

private slots:
    void on_addSouvConfirm_clicked();
    void on_addSouvBack_clicked();

private:
    Ui::adminAddSouvenir *ui;

    QString m_stadium;

    bool ensureDbOpen();
};

#endif // ADMINADDSOUVENIR_H
