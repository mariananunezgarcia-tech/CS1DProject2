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
    void on_StadMod_clicked();   // Opens modstadiums window
    void on_SuvMod_clicked();    // Opens modsouvenirs window
    void on_BackButt_clicked();  // Returns to login window

private:
    Ui::Admin *ui;
};

#endif
