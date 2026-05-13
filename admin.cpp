#include "admin.h"
#include "ui_admin.h"
#include "modstadiums.h"
#include "modsouvenirs.h"
#include "login.h"

Admin::Admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Admin)
{
    ui->setupUi(this);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_StadMod_clicked()
{
    auto *win = new ModStadiums(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}



void Admin::on_SuvMod_clicked()
{
    auto *win = new ModSouvenirs(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}





void Admin::on_BackButt_clicked()
{

        auto *win = new Login(this);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->show();
        this->hide();
}

