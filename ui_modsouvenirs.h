/********************************************************************************
** Form generated from reading UI file 'modsouvenirs.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODSOUVENIRS_H
#define UI_MODSOUVENIRS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ModSouvenirs
{
public:
    QLabel *labelTitle;
    QLabel *labelSubtitle;
    QPushButton *SuvModifybutton;
    QTableWidget *SuvModTabel;
    QLabel *labelModify;
    QPushButton *pushButton;
    QPushButton *SubmitButt;
    QPushButton *cancelButt;
    QComboBox *comboBox;
    QLabel *labelCombobox;
    QPushButton *changePrButt;
    QPushButton *addSouvButt;
    QLabel *labelMods;
    QPushButton *delSouvButt;

    void setupUi(QDialog *ModSouvenirs)
    {
        if (ModSouvenirs->objectName().isEmpty())
            ModSouvenirs->setObjectName("ModSouvenirs");
        ModSouvenirs->resize(678, 557);
        labelTitle = new QLabel(ModSouvenirs);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setGeometry(QRect(140, -10, 191, 71));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe Print")});
        font.setPointSize(14);
        font.setBold(true);
        font.setUnderline(true);
        labelTitle->setFont(font);
        labelSubtitle = new QLabel(ModSouvenirs);
        labelSubtitle->setObjectName("labelSubtitle");
        labelSubtitle->setGeometry(QRect(60, 40, 361, 31));
        SuvModifybutton = new QPushButton(ModSouvenirs);
        SuvModifybutton->setObjectName("SuvModifybutton");
        SuvModifybutton->setGeometry(QRect(440, 360, 71, 24));
        SuvModTabel = new QTableWidget(ModSouvenirs);
        if (SuvModTabel->columnCount() < 3)
            SuvModTabel->setColumnCount(3);
        SuvModTabel->setObjectName("SuvModTabel");
        SuvModTabel->setGeometry(QRect(20, 80, 401, 311));
        SuvModTabel->setColumnCount(3);
        labelModify = new QLabel(ModSouvenirs);
        labelModify->setObjectName("labelModify");
        labelModify->setGeometry(QRect(430, 340, 191, 16));
        pushButton = new QPushButton(ModSouvenirs);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(20, 400, 75, 24));
        SubmitButt = new QPushButton(ModSouvenirs);
        SubmitButt->setObjectName("SubmitButt");
        SubmitButt->setGeometry(QRect(440, 130, 61, 21));
        cancelButt = new QPushButton(ModSouvenirs);
        cancelButt->setObjectName("cancelButt");
        cancelButt->setGeometry(QRect(530, 130, 61, 21));
        comboBox = new QComboBox(ModSouvenirs);
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(440, 90, 151, 31));
        labelCombobox = new QLabel(ModSouvenirs);
        labelCombobox->setObjectName("labelCombobox");
        labelCombobox->setGeometry(QRect(440, 70, 131, 21));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe Print")});
        font1.setPointSize(10);
        font1.setBold(true);
        labelCombobox->setFont(font1);
        changePrButt = new QPushButton(ModSouvenirs);
        changePrButt->setObjectName("changePrButt");
        changePrButt->setGeometry(QRect(440, 280, 101, 24));
        addSouvButt = new QPushButton(ModSouvenirs);
        addSouvButt->setObjectName("addSouvButt");
        addSouvButt->setGeometry(QRect(440, 220, 101, 24));
        labelMods = new QLabel(ModSouvenirs);
        labelMods->setObjectName("labelMods");
        labelMods->setGeometry(QRect(440, 190, 221, 21));
        labelMods->setFont(font1);
        delSouvButt = new QPushButton(ModSouvenirs);
        delSouvButt->setObjectName("delSouvButt");
        delSouvButt->setGeometry(QRect(440, 250, 101, 24));

        retranslateUi(ModSouvenirs);

        QMetaObject::connectSlotsByName(ModSouvenirs);
    } // setupUi

    void retranslateUi(QDialog *ModSouvenirs)
    {
        ModSouvenirs->setWindowTitle(QCoreApplication::translate("ModSouvenirs", "Dialog", nullptr));
        labelTitle->setText(QCoreApplication::translate("ModSouvenirs", "Modify Souvenirs", nullptr));
        labelSubtitle->setText(QCoreApplication::translate("ModSouvenirs", "Check all the souvenirs you want students to be able to purchase:", nullptr));
        SuvModifybutton->setText(QCoreApplication::translate("ModSouvenirs", "Modify", nullptr));
        labelModify->setText(QCoreApplication::translate("ModSouvenirs", "Click Modify to save changes:", nullptr));
        pushButton->setText(QCoreApplication::translate("ModSouvenirs", "Back", nullptr));
        SubmitButt->setText(QCoreApplication::translate("ModSouvenirs", "Submit", nullptr));
        cancelButt->setText(QCoreApplication::translate("ModSouvenirs", "Cancel", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("ModSouvenirs", "Select a College", nullptr));

        labelCombobox->setText(QCoreApplication::translate("ModSouvenirs", "Select a College:", nullptr));
        changePrButt->setText(QCoreApplication::translate("ModSouvenirs", "Change Price", nullptr));
        addSouvButt->setText(QCoreApplication::translate("ModSouvenirs", "Add Souvenir", nullptr));
        labelMods->setText(QCoreApplication::translate("ModSouvenirs", "Options for Editing Souvenirs:", nullptr));
        delSouvButt->setText(QCoreApplication::translate("ModSouvenirs", "Delete Souvenir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModSouvenirs: public Ui_ModSouvenirs {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODSOUVENIRS_H
