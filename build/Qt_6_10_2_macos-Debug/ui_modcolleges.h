/********************************************************************************
** Form generated from reading UI file 'modcolleges.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODCOLLEGES_H
#define UI_MODCOLLEGES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ModColleges
{
public:
    QLabel *label;
    QLabel *label_2;
    QTableWidget *CollegeModtable;
    QPushButton *ColModifybutton;
    QLabel *label_3;
    QPushButton *pushButton;

    void setupUi(QDialog *ModColleges)
    {
        if (ModColleges->objectName().isEmpty())
            ModColleges->setObjectName("ModColleges");
        ModColleges->resize(598, 470);
        label = new QLabel(ModColleges);
        label->setObjectName("label");
        label->setGeometry(QRect(130, -10, 151, 71));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe Print")});
        font.setPointSize(14);
        font.setBold(true);
        font.setUnderline(true);
        label->setFont(font);
        label_2 = new QLabel(ModColleges);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 20, 391, 81));
        CollegeModtable = new QTableWidget(ModColleges);
        if (CollegeModtable->columnCount() < 2)
            CollegeModtable->setColumnCount(2);
        CollegeModtable->setObjectName("CollegeModtable");
        CollegeModtable->setGeometry(QRect(10, 80, 411, 341));
        CollegeModtable->setColumnCount(2);
        CollegeModtable->horizontalHeader()->setVisible(true);
        ColModifybutton = new QPushButton(ModColleges);
        ColModifybutton->setObjectName("ColModifybutton");
        ColModifybutton->setGeometry(QRect(430, 390, 75, 24));
        label_3 = new QLabel(ModColleges);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(430, 360, 161, 31));
        pushButton = new QPushButton(ModColleges);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 430, 75, 24));

        retranslateUi(ModColleges);

        QMetaObject::connectSlotsByName(ModColleges);
    } // setupUi

    void retranslateUi(QDialog *ModColleges)
    {
        ModColleges->setWindowTitle(QCoreApplication::translate("ModColleges", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ModColleges", "Modify Colleges", nullptr));
        label_2->setText(QCoreApplication::translate("ModColleges", "Check all the colleges you want students to have access to during tours:", nullptr));
        ColModifybutton->setText(QCoreApplication::translate("ModColleges", "Modify", nullptr));
        label_3->setText(QCoreApplication::translate("ModColleges", "Click Modify to save changes:", nullptr));
        pushButton->setText(QCoreApplication::translate("ModColleges", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModColleges: public Ui_ModColleges {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODCOLLEGES_H
