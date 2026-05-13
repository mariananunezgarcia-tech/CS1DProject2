#include "login.h"
#include "admin.h"
#include "ui_login.h"
#include "mainwindow.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_CancelButt_clicked()
{
    ui->logUser->clear();
    ui->logPass->clear();
}

void Login::on_LogButton_clicked()
{
    const QString username = ui->logUser->text().trimmed();
    const QString passwordText = ui->logPass->text().trimmed();

    // Admin login data
    const QString adminUsernames[5] =
    {
        "Christian",
        "Ryan",
        "Lila",
        "Dean",
        "Kevin"
    };

    const int adminPasswords[5] =
    {
        111,
        222,
        333,
        444,
        555
    };

    bool passwordIsNumber = false;
    const int password = passwordText.toInt(&passwordIsNumber);

    bool adminLogin = false;

    if (passwordIsNumber)
    {
        for (int i = 0; i < 5; i++)
        {
            if (username == adminUsernames[i] &&
                password == adminPasswords[i])
            {
                adminLogin = true;
                break;
            }
        }
    }

    if (adminLogin)
    {
        auto *win = new Admin(nullptr);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->show();

        this->close();
        return;
    }

    auto *win = new MainWindow(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}
