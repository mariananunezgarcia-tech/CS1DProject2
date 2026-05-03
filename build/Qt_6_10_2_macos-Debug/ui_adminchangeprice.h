/********************************************************************************
** Form generated from reading UI file 'adminchangeprice.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINCHANGEPRICE_H
#define UI_ADMINCHANGEPRICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_adminChangePrice
{
public:
    QLabel *changePrLbl;
    QComboBox *changePrCombo;
    QLabel *comboPriceLbl;
    QLabel *priceLbl;
    QPushButton *changePrConfirm;
    QLineEdit *enterNewPrice;
    QPushButton *changePrBack;

    void setupUi(QWidget *adminChangePrice)
    {
        if (adminChangePrice->objectName().isEmpty())
            adminChangePrice->setObjectName("adminChangePrice");
        adminChangePrice->resize(240, 320);
        changePrLbl = new QLabel(adminChangePrice);
        changePrLbl->setObjectName("changePrLbl");
        changePrLbl->setGeometry(QRect(20, 20, 201, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(15);
        font.setBold(true);
        font.setItalic(true);
        changePrLbl->setFont(font);
        changePrLbl->setAlignment(Qt::AlignmentFlag::AlignCenter);
        changePrCombo = new QComboBox(adminChangePrice);
        changePrCombo->setObjectName("changePrCombo");
        changePrCombo->setGeometry(QRect(60, 80, 121, 24));
        comboPriceLbl = new QLabel(adminChangePrice);
        comboPriceLbl->setObjectName("comboPriceLbl");
        comboPriceLbl->setGeometry(QRect(10, 60, 221, 20));
        priceLbl = new QLabel(adminChangePrice);
        priceLbl->setObjectName("priceLbl");
        priceLbl->setGeometry(QRect(70, 130, 101, 16));
        changePrConfirm = new QPushButton(adminChangePrice);
        changePrConfirm->setObjectName("changePrConfirm");
        changePrConfirm->setGeometry(QRect(70, 200, 80, 24));
        enterNewPrice = new QLineEdit(adminChangePrice);
        enterNewPrice->setObjectName("enterNewPrice");
        enterNewPrice->setGeometry(QRect(60, 150, 113, 24));
        changePrBack = new QPushButton(adminChangePrice);
        changePrBack->setObjectName("changePrBack");
        changePrBack->setGeometry(QRect(70, 260, 80, 24));

        retranslateUi(adminChangePrice);

        QMetaObject::connectSlotsByName(adminChangePrice);
    } // setupUi

    void retranslateUi(QWidget *adminChangePrice)
    {
        adminChangePrice->setWindowTitle(QCoreApplication::translate("adminChangePrice", "Form", nullptr));
        changePrLbl->setText(QCoreApplication::translate("adminChangePrice", "Change Price", nullptr));
        comboPriceLbl->setText(QCoreApplication::translate("adminChangePrice", "Select the souvenir you want to change:", nullptr));
        priceLbl->setText(QCoreApplication::translate("adminChangePrice", "Enter a new price:", nullptr));
        changePrConfirm->setText(QCoreApplication::translate("adminChangePrice", "Confirm", nullptr));
        changePrBack->setText(QCoreApplication::translate("adminChangePrice", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class adminChangePrice: public Ui_adminChangePrice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINCHANGEPRICE_H
