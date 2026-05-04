/********************************************************************************
** Form generated from reading UI file 'summarywindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUMMARYWINDOW_H
#define UI_SUMMARYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_summaryWindow
{
public:
    QListView *listColleges;
    QLabel *labelDistTrav;
    QLabel *labelSouvPurc;
    QLabel *labelSouvCost;
    QLineEdit *lineDistTrav;
    QLineEdit *lineSouvPurc;
    QLineEdit *lineSouvCost;
    QListWidget *listSouvenirs;
    QLabel *labelTitle1;
    QLabel *labelTitle2;
    QPushButton *backButtonSum;

    void setupUi(QDialog *summaryWindow)
    {
        if (summaryWindow->objectName().isEmpty())
            summaryWindow->setObjectName("summaryWindow");
        summaryWindow->resize(1114, 658);
        listColleges = new QListView(summaryWindow);
        listColleges->setObjectName("listColleges");
        listColleges->setGeometry(QRect(290, 50, 651, 251));
        labelDistTrav = new QLabel(summaryWindow);
        labelDistTrav->setObjectName("labelDistTrav");
        labelDistTrav->setGeometry(QRect(30, 110, 171, 20));
        QFont font;
        font.setBold(true);
        labelDistTrav->setFont(font);
        labelSouvPurc = new QLabel(summaryWindow);
        labelSouvPurc->setObjectName("labelSouvPurc");
        labelSouvPurc->setGeometry(QRect(30, 250, 171, 20));
        labelSouvPurc->setFont(font);
        labelSouvCost = new QLabel(summaryWindow);
        labelSouvCost->setObjectName("labelSouvCost");
        labelSouvCost->setGeometry(QRect(30, 360, 171, 20));
        labelSouvCost->setFont(font);
        lineDistTrav = new QLineEdit(summaryWindow);
        lineDistTrav->setObjectName("lineDistTrav");
        lineDistTrav->setGeometry(QRect(30, 130, 141, 21));
        lineSouvPurc = new QLineEdit(summaryWindow);
        lineSouvPurc->setObjectName("lineSouvPurc");
        lineSouvPurc->setGeometry(QRect(30, 270, 141, 21));
        lineSouvCost = new QLineEdit(summaryWindow);
        lineSouvCost->setObjectName("lineSouvCost");
        lineSouvCost->setGeometry(QRect(30, 380, 141, 21));
        listSouvenirs = new QListWidget(summaryWindow);
        listSouvenirs->setObjectName("listSouvenirs");
        listSouvenirs->setGeometry(QRect(290, 350, 651, 301));
        labelTitle1 = new QLabel(summaryWindow);
        labelTitle1->setObjectName("labelTitle1");
        labelTitle1->setGeometry(QRect(300, 20, 641, 21));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Rockwell")});
        font1.setPointSize(12);
        font1.setBold(true);
        labelTitle1->setFont(font1);
        labelTitle2 = new QLabel(summaryWindow);
        labelTitle2->setObjectName("labelTitle2");
        labelTitle2->setGeometry(QRect(300, 320, 641, 21));
        labelTitle2->setFont(font1);
        backButtonSum = new QPushButton(summaryWindow);
        backButtonSum->setObjectName("backButtonSum");
        backButtonSum->setGeometry(QRect(30, 600, 80, 24));

        retranslateUi(summaryWindow);

        QMetaObject::connectSlotsByName(summaryWindow);
    } // setupUi

    void retranslateUi(QDialog *summaryWindow)
    {
        summaryWindow->setWindowTitle(QCoreApplication::translate("summaryWindow", "Dialog", nullptr));
        labelDistTrav->setText(QCoreApplication::translate("summaryWindow", "Distance Traveled: ", nullptr));
        labelSouvPurc->setText(QCoreApplication::translate("summaryWindow", "Souvenirs Purchased:", nullptr));
        labelSouvCost->setText(QCoreApplication::translate("summaryWindow", "Souvenir Costs:", nullptr));
        labelTitle1->setText(QCoreApplication::translate("summaryWindow", "These are all the Colleges you Visited:", nullptr));
        labelTitle2->setText(QCoreApplication::translate("summaryWindow", "These are all the Souvenirs that you Purchased:", nullptr));
        backButtonSum->setText(QCoreApplication::translate("summaryWindow", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class summaryWindow: public Ui_summaryWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUMMARYWINDOW_H
