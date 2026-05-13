#ifndef ADMINDELETESOUVENIR_H
#define ADMINDELETESOUVENIR_H

#include <QWidget>
#include <QString>

namespace Ui {
class AdminDeleteSouvenir;
}

class AdminDeleteSouvenir : public QWidget
{
    Q_OBJECT

public:
    explicit AdminDeleteSouvenir(const QString &stadium, QWidget *parent = nullptr);
    ~AdminDeleteSouvenir();

private slots:
    void on_delSouvConfirm_clicked();
    void on_delSouvBack_clicked();

private:
    Ui::AdminDeleteSouvenir *ui;

    QString m_stadium;

    bool ensureDbOpen();
    void loadSouvenirDropdown();
};

#endif // ADMINDELETESOUVENIR_H
