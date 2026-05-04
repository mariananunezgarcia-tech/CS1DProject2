/********************************************************************************
** Form generated from reading UI file 'tripwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIPWINDOW_H
#define UI_TRIPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_tripWindow
{
public:
    QPushButton *buttNextSum;
    QLabel *labelNextCol;
    QLabel *labelDistNext;
    QLabel *labelSouvPurc;
    QLabel *labelSouvCost;
    QLabel *labelCurrCol;
    QLabel *labelDistTrav;
    QTableWidget *souvDisplay;
    QLabel *labelCheckAll;
    QLabel *labelBuySouv;
    QPushButton *buyButt;
    QListView *collegeDisplay;
    QLineEdit *lineCurrColl;
    QLineEdit *lineDistTrav;
    QLineEdit *lineNextColl;
    QLineEdit *lineDistNext;
    QLineEdit *lineSouvPurc;
    QLineEdit *lineSouvCost;

    void setupUi(QDialog *tripWindow)
    {
        if (tripWindow->objectName().isEmpty())
            tripWindow->setObjectName("tripWindow");
        tripWindow->resize(939, 590);
        buttNextSum = new QPushButton(tripWindow);
        buttNextSum->setObjectName("buttNextSum");
        buttNextSum->setGeometry(QRect(30, 470, 161, 29));
        labelNextCol = new QLabel(tripWindow);
        labelNextCol->setObjectName("labelNextCol");
        labelNextCol->setGeometry(QRect(20, 340, 121, 20));
        labelDistNext = new QLabel(tripWindow);
        labelDistNext->setObjectName("labelDistNext");
        labelDistNext->setGeometry(QRect(20, 380, 181, 20));
        labelSouvPurc = new QLabel(tripWindow);
        labelSouvPurc->setObjectName("labelSouvPurc");
        labelSouvPurc->setGeometry(QRect(20, 230, 151, 20));
        labelSouvCost = new QLabel(tripWindow);
        labelSouvCost->setObjectName("labelSouvCost");
        labelSouvCost->setGeometry(QRect(20, 250, 121, 20));
        labelCurrCol = new QLabel(tripWindow);
        labelCurrCol->setObjectName("labelCurrCol");
        labelCurrCol->setGeometry(QRect(20, 40, 121, 20));
        labelDistTrav = new QLabel(tripWindow);
        labelDistTrav->setObjectName("labelDistTrav");
        labelDistTrav->setGeometry(QRect(20, 70, 131, 20));
        souvDisplay = new QTableWidget(tripWindow);
        souvDisplay->setObjectName("souvDisplay");
        souvDisplay->setGeometry(QRect(450, 320, 471, 211));
        labelCheckAll = new QLabel(tripWindow);
        labelCheckAll->setObjectName("labelCheckAll");
        labelCheckAll->setGeometry(QRect(450, 290, 461, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Perpetua Titling MT")});
        font.setPointSize(8);
        font.setBold(true);
        labelCheckAll->setFont(font);
        labelBuySouv = new QLabel(tripWindow);
        labelBuySouv->setObjectName("labelBuySouv");
        labelBuySouv->setGeometry(QRect(450, 540, 211, 31));
        labelBuySouv->setFont(font);
        buyButt = new QPushButton(tripWindow);
        buyButt->setObjectName("buyButt");
        buyButt->setGeometry(QRect(830, 540, 91, 31));
        collegeDisplay = new QListView(tripWindow);
        collegeDisplay->setObjectName("collegeDisplay");
        collegeDisplay->setGeometry(QRect(450, 20, 471, 271));
        lineCurrColl = new QLineEdit(tripWindow);
        lineCurrColl->setObjectName("lineCurrColl");
        lineCurrColl->setGeometry(QRect(160, 40, 231, 21));
        lineDistTrav = new QLineEdit(tripWindow);
        lineDistTrav->setObjectName("lineDistTrav");
        lineDistTrav->setGeometry(QRect(160, 70, 231, 21));
        lineNextColl = new QLineEdit(tripWindow);
        lineNextColl->setObjectName("lineNextColl");
        lineNextColl->setGeometry(QRect(160, 340, 231, 21));
        lineDistNext = new QLineEdit(tripWindow);
        lineDistNext->setObjectName("lineDistNext");
        lineDistNext->setGeometry(QRect(160, 380, 231, 21));
        lineSouvPurc = new QLineEdit(tripWindow);
        lineSouvPurc->setObjectName("lineSouvPurc");
        lineSouvPurc->setGeometry(QRect(150, 230, 231, 21));
        lineSouvCost = new QLineEdit(tripWindow);
        lineSouvCost->setObjectName("lineSouvCost");
        lineSouvCost->setGeometry(QRect(150, 260, 231, 21));

        retranslateUi(tripWindow);

        QMetaObject::connectSlotsByName(tripWindow);
    } // setupUi

    void retranslateUi(QDialog *tripWindow)
    {
        tripWindow->setWindowTitle(QCoreApplication::translate("tripWindow", "Dialog", nullptr));
        buttNextSum->setText(QCoreApplication::translate("tripWindow", "See Summary", nullptr));
        labelNextCol->setText(QCoreApplication::translate("tripWindow", "Next College:", nullptr));
        labelDistNext->setText(QCoreApplication::translate("tripWindow", "Distance to Next College:", nullptr));
        labelSouvPurc->setText(QCoreApplication::translate("tripWindow", "Souvenirs Purchased: ", nullptr));
        labelSouvCost->setText(QCoreApplication::translate("tripWindow", "Souvenirs Cost: ", nullptr));
        labelCurrCol->setText(QCoreApplication::translate("tripWindow", "Current College:", nullptr));
        labelDistTrav->setText(QCoreApplication::translate("tripWindow", "Distance Traveled: ", nullptr));
        labelCheckAll->setText(QCoreApplication::translate("tripWindow", "Check All the Souvenirs that you want: ", nullptr));
        labelBuySouv->setText(QCoreApplication::translate("tripWindow", "Hit Buy to purchase your souvenirs:", nullptr));
        buyButt->setText(QCoreApplication::translate("tripWindow", "Buy", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tripWindow: public Ui_tripWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIPWINDOW_H
