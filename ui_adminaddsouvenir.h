/********************************************************************************
** Form generated from reading UI file 'adminaddsouvenir.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINADDSOUVENIR_H
#define UI_ADMINADDSOUVENIR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_adminAddSouvenir
{
public:
    QPushButton *addSouvConfirm;
    QLineEdit *souvName;
    QLabel *souvNameLbl;
    QLineEdit *souvName_2;
    QLabel *souvPriceLbl;
    QLabel *changePrLbl;
    QPushButton *addSouvBack;

    void setupUi(QWidget *adminAddSouvenir)
    {
        if (adminAddSouvenir->objectName().isEmpty())
            adminAddSouvenir->setObjectName("adminAddSouvenir");
        adminAddSouvenir->resize(240, 320);
        addSouvConfirm = new QPushButton(adminAddSouvenir);
        addSouvConfirm->setObjectName("addSouvConfirm");
        addSouvConfirm->setGeometry(QRect(70, 230, 80, 24));
        souvName = new QLineEdit(adminAddSouvenir);
        souvName->setObjectName("souvName");
        souvName->setGeometry(QRect(60, 90, 113, 24));
        souvNameLbl = new QLabel(adminAddSouvenir);
        souvNameLbl->setObjectName("souvNameLbl");
        souvNameLbl->setGeometry(QRect(20, 60, 201, 20));
        souvName_2 = new QLineEdit(adminAddSouvenir);
        souvName_2->setObjectName("souvName_2");
        souvName_2->setGeometry(QRect(60, 170, 113, 24));
        souvPriceLbl = new QLabel(adminAddSouvenir);
        souvPriceLbl->setObjectName("souvPriceLbl");
        souvPriceLbl->setGeometry(QRect(20, 140, 201, 20));
        changePrLbl = new QLabel(adminAddSouvenir);
        changePrLbl->setObjectName("changePrLbl");
        changePrLbl->setGeometry(QRect(20, 20, 201, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(15);
        font.setBold(true);
        font.setItalic(true);
        changePrLbl->setFont(font);
        changePrLbl->setAlignment(Qt::AlignmentFlag::AlignCenter);
        addSouvBack = new QPushButton(adminAddSouvenir);
        addSouvBack->setObjectName("addSouvBack");
        addSouvBack->setGeometry(QRect(70, 280, 80, 24));

        retranslateUi(adminAddSouvenir);

        QMetaObject::connectSlotsByName(adminAddSouvenir);
    } // setupUi

    void retranslateUi(QWidget *adminAddSouvenir)
    {
        adminAddSouvenir->setWindowTitle(QCoreApplication::translate("adminAddSouvenir", "Form", nullptr));
        addSouvConfirm->setText(QCoreApplication::translate("adminAddSouvenir", "Confirm", nullptr));
        souvNameLbl->setText(QCoreApplication::translate("adminAddSouvenir", "Enter the name of the new souvenir:", nullptr));
        souvPriceLbl->setText(QCoreApplication::translate("adminAddSouvenir", "Enter the price of the new souvenir:", nullptr));
        changePrLbl->setText(QCoreApplication::translate("adminAddSouvenir", "Add Souvenir", nullptr));
        addSouvBack->setText(QCoreApplication::translate("adminAddSouvenir", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class adminAddSouvenir: public Ui_adminAddSouvenir {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINADDSOUVENIR_H
