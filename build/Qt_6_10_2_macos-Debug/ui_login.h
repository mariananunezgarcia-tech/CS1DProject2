/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QLineEdit *logPass;
    QLabel *labelUsername;
    QLabel *labelPassword;
    QPushButton *LogButton;
    QPushButton *CancelButt;
    QPushButton *AdminButton;
    QLineEdit *logUser;
    QLabel *lblWelcome;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(400, 300);
        logPass = new QLineEdit(Login);
        logPass->setObjectName("logPass");
        logPass->setGeometry(QRect(100, 120, 151, 31));
        logPass->setMouseTracking(true);
        logPass->setEchoMode(QLineEdit::EchoMode::Password);
        labelUsername = new QLabel(Login);
        labelUsername->setObjectName("labelUsername");
        labelUsername->setGeometry(QRect(20, 80, 101, 21));
        labelPassword = new QLabel(Login);
        labelPassword->setObjectName("labelPassword");
        labelPassword->setGeometry(QRect(20, 120, 63, 20));
        LogButton = new QPushButton(Login);
        LogButton->setObjectName("LogButton");
        LogButton->setGeometry(QRect(100, 170, 61, 21));
        CancelButt = new QPushButton(Login);
        CancelButt->setObjectName("CancelButt");
        CancelButt->setGeometry(QRect(180, 170, 61, 21));
        AdminButton = new QPushButton(Login);
        AdminButton->setObjectName("AdminButton");
        AdminButton->setGeometry(QRect(140, 240, 75, 24));
        logUser = new QLineEdit(Login);
        logUser->setObjectName("logUser");
        logUser->setGeometry(QRect(100, 80, 151, 31));
        logUser->setMouseTracking(true);
        logUser->setEchoMode(QLineEdit::EchoMode::Password);
        lblWelcome = new QLabel(Login);
        lblWelcome->setObjectName("lblWelcome");
        lblWelcome->setGeometry(QRect(120, 40, 101, 21));
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        font.setItalic(true);
        lblWelcome->setFont(font);
        lblWelcome->setAlignment(Qt::AlignmentFlag::AlignCenter);

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Dialog", nullptr));
        labelUsername->setText(QCoreApplication::translate("Login", "Username:", nullptr));
        labelPassword->setText(QCoreApplication::translate("Login", "Password:", nullptr));
        LogButton->setText(QCoreApplication::translate("Login", "Login", nullptr));
        CancelButt->setText(QCoreApplication::translate("Login", "Clear", nullptr));
        AdminButton->setText(QCoreApplication::translate("Login", "Admin", nullptr));
        lblWelcome->setText(QCoreApplication::translate("Login", "Welcome!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
