/********************************************************************************
** Form generated from reading UI file 'customtripwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMTRIPWINDOW_H
#define UI_CUSTOMTRIPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomTripWindow
{
public:
    QWidget *centralwidget;
    QComboBox *selectStartingCollegeDropdownCT;
    QPushButton *confirmButtonCT;
    QLabel *subtitle2CT;
    QPushButton *startTripButtonCT;
    QTableWidget *tableWidget;
    QPushButton *backButtCT;
    QLabel *subtitle1CT;
    QLabel *titleCT;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CustomTripWindow)
    {
        if (CustomTripWindow->objectName().isEmpty())
            CustomTripWindow->setObjectName("CustomTripWindow");
        CustomTripWindow->resize(343, 536);
        centralwidget = new QWidget(CustomTripWindow);
        centralwidget->setObjectName("centralwidget");
        selectStartingCollegeDropdownCT = new QComboBox(centralwidget);
        selectStartingCollegeDropdownCT->setObjectName("selectStartingCollegeDropdownCT");
        selectStartingCollegeDropdownCT->setGeometry(QRect(20, 80, 191, 25));
        confirmButtonCT = new QPushButton(centralwidget);
        confirmButtonCT->setObjectName("confirmButtonCT");
        confirmButtonCT->setGeometry(QRect(220, 80, 81, 25));
        subtitle2CT = new QLabel(centralwidget);
        subtitle2CT->setObjectName("subtitle2CT");
        subtitle2CT->setGeometry(QRect(70, 140, 171, 20));
        subtitle2CT->setAlignment(Qt::AlignmentFlag::AlignCenter);
        startTripButtonCT = new QPushButton(centralwidget);
        startTripButtonCT->setObjectName("startTripButtonCT");
        startTripButtonCT->setGeometry(QRect(120, 420, 80, 25));
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(10, 170, 311, 241));
        backButtCT = new QPushButton(centralwidget);
        backButtCT->setObjectName("backButtCT");
        backButtCT->setGeometry(QRect(120, 460, 80, 25));
        subtitle1CT = new QLabel(centralwidget);
        subtitle1CT->setObjectName("subtitle1CT");
        subtitle1CT->setGeometry(QRect(70, 50, 171, 20));
        subtitle1CT->setAlignment(Qt::AlignmentFlag::AlignCenter);
        titleCT = new QLabel(centralwidget);
        titleCT->setObjectName("titleCT");
        titleCT->setGeometry(QRect(50, 10, 211, 31));
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        font.setItalic(true);
        titleCT->setFont(font);
        titleCT->setAlignment(Qt::AlignmentFlag::AlignCenter);
        CustomTripWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CustomTripWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 343, 21));
        CustomTripWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CustomTripWindow);
        statusbar->setObjectName("statusbar");
        CustomTripWindow->setStatusBar(statusbar);

        retranslateUi(CustomTripWindow);
        QObject::connect(startTripButtonCT, SIGNAL(clicked()), CustomTripWindow, SLOT(startTripClickedCT()));

        QMetaObject::connectSlotsByName(CustomTripWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CustomTripWindow)
    {
        CustomTripWindow->setWindowTitle(QCoreApplication::translate("CustomTripWindow", "MainWindow", nullptr));
        confirmButtonCT->setText(QCoreApplication::translate("CustomTripWindow", "Confirm", nullptr));
        subtitle2CT->setText(QCoreApplication::translate("CustomTripWindow", "Select Colleges to Visit", nullptr));
        startTripButtonCT->setText(QCoreApplication::translate("CustomTripWindow", "Start Trip", nullptr));
        backButtCT->setText(QCoreApplication::translate("CustomTripWindow", "Back", nullptr));
        subtitle1CT->setText(QCoreApplication::translate("CustomTripWindow", "Select a Starting College", nullptr));
        titleCT->setText(QCoreApplication::translate("CustomTripWindow", "Plan a Custom Tour", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomTripWindow: public Ui_CustomTripWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMTRIPWINDOW_H
