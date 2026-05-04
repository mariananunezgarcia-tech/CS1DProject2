/********************************************************************************
** Form generated from reading UI file 'basictripwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASICTRIPWINDOW_H
#define UI_BASICTRIPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BasicTripWindow
{
public:
    QWidget *centralwidget;
    QPushButton *startTripButtonBT;
    QComboBox *numCollegestoVisitDropdownBT;
    QComboBox *selectStartingCollegeDropdownBT;
    QLabel *selectStartingCollegeLabelBT;
    QLabel *numCollegestoVisitLabelBT;
    QPushButton *backButtonBT;
    QLabel *titleBT;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *BasicTripWindow)
    {
        if (BasicTripWindow->objectName().isEmpty())
            BasicTripWindow->setObjectName("BasicTripWindow");
        BasicTripWindow->resize(241, 339);
        centralwidget = new QWidget(BasicTripWindow);
        centralwidget->setObjectName("centralwidget");
        startTripButtonBT = new QPushButton(centralwidget);
        startTripButtonBT->setObjectName("startTripButtonBT");
        startTripButtonBT->setGeometry(QRect(80, 210, 80, 25));
        numCollegestoVisitDropdownBT = new QComboBox(centralwidget);
        numCollegestoVisitDropdownBT->setObjectName("numCollegestoVisitDropdownBT");
        numCollegestoVisitDropdownBT->setGeometry(QRect(45, 160, 150, 25));
        selectStartingCollegeDropdownBT = new QComboBox(centralwidget);
        selectStartingCollegeDropdownBT->setObjectName("selectStartingCollegeDropdownBT");
        selectStartingCollegeDropdownBT->setGeometry(QRect(45, 90, 161, 25));
        selectStartingCollegeLabelBT = new QLabel(centralwidget);
        selectStartingCollegeLabelBT->setObjectName("selectStartingCollegeLabelBT");
        selectStartingCollegeLabelBT->setGeometry(QRect(45, 70, 171, 16));
        selectStartingCollegeLabelBT->setAlignment(Qt::AlignmentFlag::AlignCenter);
        numCollegestoVisitLabelBT = new QLabel(centralwidget);
        numCollegestoVisitLabelBT->setObjectName("numCollegestoVisitLabelBT");
        numCollegestoVisitLabelBT->setGeometry(QRect(20, 140, 191, 20));
        numCollegestoVisitLabelBT->setAlignment(Qt::AlignmentFlag::AlignCenter);
        backButtonBT = new QPushButton(centralwidget);
        backButtonBT->setObjectName("backButtonBT");
        backButtonBT->setGeometry(QRect(80, 250, 80, 25));
        titleBT = new QLabel(centralwidget);
        titleBT->setObjectName("titleBT");
        titleBT->setGeometry(QRect(10, 20, 211, 31));
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        font.setItalic(true);
        titleBT->setFont(font);
        titleBT->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BasicTripWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(BasicTripWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 241, 21));
        BasicTripWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(BasicTripWindow);
        statusbar->setObjectName("statusbar");
        BasicTripWindow->setStatusBar(statusbar);

        retranslateUi(BasicTripWindow);

        QMetaObject::connectSlotsByName(BasicTripWindow);
    } // setupUi

    void retranslateUi(QMainWindow *BasicTripWindow)
    {
        BasicTripWindow->setWindowTitle(QCoreApplication::translate("BasicTripWindow", "MainWindow", nullptr));
        startTripButtonBT->setText(QCoreApplication::translate("BasicTripWindow", "Start Trip", nullptr));
        selectStartingCollegeLabelBT->setText(QCoreApplication::translate("BasicTripWindow", "Select Starting College:", nullptr));
        numCollegestoVisitLabelBT->setText(QCoreApplication::translate("BasicTripWindow", "Number of Other Colleges to Visit:", nullptr));
        backButtonBT->setText(QCoreApplication::translate("BasicTripWindow", "Back", nullptr));
        titleBT->setText(QCoreApplication::translate("BasicTripWindow", "Plan a Basic Tour", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BasicTripWindow: public Ui_BasicTripWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASICTRIPWINDOW_H
