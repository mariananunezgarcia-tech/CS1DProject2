#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>

namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

private slots:
    void on_ColMod_clicked();

    void on_SuvMod_clicked();

    void on_BackButt_clicked();

private:
    Ui::Admin *ui;
};

#endif // ADMIN_H
