/********************************************************************************
** Form generated from reading UI file 'admin.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMIN_H
#define UI_ADMIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Admin
{
public:
    QPushButton *ColMod;
    QPushButton *SuvMod;
    QPushButton *BackButt;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QDialog *Admin)
    {
        if (Admin->objectName().isEmpty())
            Admin->setObjectName("Admin");
        Admin->resize(400, 300);
        ColMod = new QPushButton(Admin);
        ColMod->setObjectName("ColMod");
        ColMod->setGeometry(QRect(140, 160, 81, 24));
        SuvMod = new QPushButton(Admin);
        SuvMod->setObjectName("SuvMod");
        SuvMod->setGeometry(QRect(140, 190, 81, 24));
        BackButt = new QPushButton(Admin);
        BackButt->setObjectName("BackButt");
        BackButt->setGeometry(QRect(20, 260, 75, 24));
        label = new QLabel(Admin);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 150, 91, 31));
        label_2 = new QLabel(Admin);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 180, 101, 41));
        label_3 = new QLabel(Admin);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(100, 10, 201, 91));
        QFont font;
        font.setFamilies({QString::fromUtf8("Showcard Gothic")});
        font.setPointSize(14);
        label_3->setFont(font);
        label_3->setFrameShape(QFrame::Shape::NoFrame);
        label_3->setFrameShadow(QFrame::Shadow::Plain);
        label_4 = new QLabel(Admin);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 120, 211, 21));
        QFont font1;
        font1.setBold(true);
        font1.setItalic(true);
        label_4->setFont(font1);

        retranslateUi(Admin);

        QMetaObject::connectSlotsByName(Admin);
    } // setupUi

    void retranslateUi(QDialog *Admin)
    {
        Admin->setWindowTitle(QCoreApplication::translate("Admin", "Dialog", nullptr));
        ColMod->setText(QCoreApplication::translate("Admin", "Modify", nullptr));
        SuvMod->setText(QCoreApplication::translate("Admin", "Modify", nullptr));
        BackButt->setText(QCoreApplication::translate("Admin", "Back", nullptr));
        label->setText(QCoreApplication::translate("Admin", "Modify Colleges:", nullptr));
        label_2->setText(QCoreApplication::translate("Admin", "Modify Souvenirs:", nullptr));
        label_3->setText(QCoreApplication::translate("Admin", "   WELCOME ADMIN!", nullptr));
        label_4->setText(QCoreApplication::translate("Admin", "WHAT DO YOU WANT TO CHANGE??", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Admin: public Ui_Admin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMIN_H
