/********************************************************************************
** Form generated from reading UI file 'admindeletesouvenir.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINDELETESOUVENIR_H
#define UI_ADMINDELETESOUVENIR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminDeleteSouvenir
{
public:
    QLabel *labelDelTitle;
    QLabel *labelDelSubtitle;
    QComboBox *deleteSouvCombo;
    QPushButton *delSouvConfirm;
    QLabel *labelWarningMsg;
    QLabel *labelWarning;
    QPushButton *delSouvBack;

    void setupUi(QWidget *AdminDeleteSouvenir)
    {
        if (AdminDeleteSouvenir->objectName().isEmpty())
            AdminDeleteSouvenir->setObjectName("AdminDeleteSouvenir");
        AdminDeleteSouvenir->resize(240, 320);
        labelDelTitle = new QLabel(AdminDeleteSouvenir);
        labelDelTitle->setObjectName("labelDelTitle");
        labelDelTitle->setGeometry(QRect(20, 30, 201, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(15);
        font.setBold(true);
        font.setItalic(true);
        labelDelTitle->setFont(font);
        labelDelTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelDelSubtitle = new QLabel(AdminDeleteSouvenir);
        labelDelSubtitle->setObjectName("labelDelSubtitle");
        labelDelSubtitle->setGeometry(QRect(20, 60, 221, 20));
        deleteSouvCombo = new QComboBox(AdminDeleteSouvenir);
        deleteSouvCombo->setObjectName("deleteSouvCombo");
        deleteSouvCombo->setGeometry(QRect(60, 90, 121, 24));
        delSouvConfirm = new QPushButton(AdminDeleteSouvenir);
        delSouvConfirm->setObjectName("delSouvConfirm");
        delSouvConfirm->setGeometry(QRect(70, 200, 80, 24));
        labelWarningMsg = new QLabel(AdminDeleteSouvenir);
        labelWarningMsg->setObjectName("labelWarningMsg");
        labelWarningMsg->setGeometry(QRect(30, 170, 181, 21));
        labelWarning = new QLabel(AdminDeleteSouvenir);
        labelWarning->setObjectName("labelWarning");
        labelWarning->setGeometry(QRect(90, 150, 61, 21));
        delSouvBack = new QPushButton(AdminDeleteSouvenir);
        delSouvBack->setObjectName("delSouvBack");
        delSouvBack->setGeometry(QRect(70, 250, 80, 24));

        retranslateUi(AdminDeleteSouvenir);

        QMetaObject::connectSlotsByName(AdminDeleteSouvenir);
    } // setupUi

    void retranslateUi(QWidget *AdminDeleteSouvenir)
    {
        AdminDeleteSouvenir->setWindowTitle(QCoreApplication::translate("AdminDeleteSouvenir", "Form", nullptr));
        labelDelTitle->setText(QCoreApplication::translate("AdminDeleteSouvenir", "Delete Souvenir", nullptr));
        labelDelSubtitle->setText(QCoreApplication::translate("AdminDeleteSouvenir", "Select the souvenir you want to delete:", nullptr));
        delSouvConfirm->setText(QCoreApplication::translate("AdminDeleteSouvenir", "Confirm", nullptr));
        labelWarningMsg->setText(QCoreApplication::translate("AdminDeleteSouvenir", "Deleting a souvenir is permanent!", nullptr));
        labelWarning->setText(QCoreApplication::translate("AdminDeleteSouvenir", "Warning!", nullptr));
        delSouvBack->setText(QCoreApplication::translate("AdminDeleteSouvenir", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminDeleteSouvenir: public Ui_AdminDeleteSouvenir {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINDELETESOUVENIR_H
