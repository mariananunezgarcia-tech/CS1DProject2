#include "login.h"
#include "admin.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <string>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login(){}

void Login::on_CancelButt_clicked()
{
    ui->logUser->setText("");
    ui->logPass->setText("");
}

void Login::on_LogButton_clicked()
{
    // TODO: put real credential checking here if you want.
    // For now: always succeed and open MainWindow.

    if (!m_mainWindow)
    {
        m_mainWindow = new MainWindow();
        m_mainWindow->setAttribute(Qt::WA_DeleteOnClose);

        // If the main window gets closed, also close the login (or just exit app)
        connect(m_mainWindow, &QObject::destroyed, this, [this]() {
            this->close();
        });
    }

    m_mainWindow->show();
    this->close();   // FIXED (was this-close())
    /*if(ui->logUser == "Admin")
    {

    }
    else
    {

    }
   */
}


void Login::on_AdminButton_clicked()
{
    auto *win = new Admin(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();   // FIXED (was this-close())
}

