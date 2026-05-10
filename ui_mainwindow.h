/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QComboBox *dropdownSouvenirPreview;
    QPushButton *buttonSouvenirPreview;
    QPushButton *buttonSouvenirCancel;
    QListView *distanceListView;
    QPushButton *buttonDistancesSubmit;
    QComboBox *dropdownDistances;
    QLabel *labelTrip;
    QPushButton *buttonBasicTrip;
    QPushButton *buttonCustomTrip;
    QPushButton *pushButton;
    QLabel *labelSouvenirs;
    QLabel *labelDistances;
    QPushButton *ClosestCenterField;
    QPushButton *SortByName;
    QPushButton *SortByStadium;
    QPushButton *SortByAL;
    QPushButton *SortByNL;
    QPushButton *SortByTypology;
    QPushButton *SortByOpenRoof;
    QPushButton *DisplayAll;
    QPushButton *SortByNewest;
    QPushButton *SortByCapacity;
    QPushButton *FurthestCenterField;
    QPushButton *buttonSpecAlgorithms;
    QComboBox *dropdownInformation;
    QPushButton *buttonInfoSubmit;
    QLabel *labelDistances_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1163, 716);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        dropdownSouvenirPreview = new QComboBox(centralwidget);
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->setObjectName("dropdownSouvenirPreview");
        dropdownSouvenirPreview->setGeometry(QRect(10, 30, 190, 41));
        dropdownSouvenirPreview->setEditable(false);
        buttonSouvenirPreview = new QPushButton(centralwidget);
        buttonSouvenirPreview->setObjectName("buttonSouvenirPreview");
        buttonSouvenirPreview->setGeometry(QRect(10, 70, 191, 32));
        buttonSouvenirCancel = new QPushButton(centralwidget);
        buttonSouvenirCancel->setObjectName("buttonSouvenirCancel");
        buttonSouvenirCancel->setGeometry(QRect(940, 610, 81, 31));
        distanceListView = new QListView(centralwidget);
        distanceListView->setObjectName("distanceListView");
        distanceListView->setGeometry(QRect(260, 30, 671, 581));
        buttonDistancesSubmit = new QPushButton(centralwidget);
        buttonDistancesSubmit->setObjectName("buttonDistancesSubmit");
        buttonDistancesSubmit->setGeometry(QRect(10, 160, 191, 32));
        dropdownDistances = new QComboBox(centralwidget);
        dropdownDistances->addItem(QString());
        dropdownDistances->setObjectName("dropdownDistances");
        dropdownDistances->setGeometry(QRect(10, 130, 190, 41));
        dropdownDistances->setEditable(false);
        labelTrip = new QLabel(centralwidget);
        labelTrip->setObjectName("labelTrip");
        labelTrip->setGeometry(QRect(950, 30, 91, 20));
        QFont font;
        font.setBold(true);
        labelTrip->setFont(font);
        buttonBasicTrip = new QPushButton(centralwidget);
        buttonBasicTrip->setObjectName("buttonBasicTrip");
        buttonBasicTrip->setGeometry(QRect(950, 60, 191, 32));
        buttonCustomTrip = new QPushButton(centralwidget);
        buttonCustomTrip->setObjectName("buttonCustomTrip");
        buttonCustomTrip->setGeometry(QRect(950, 110, 191, 32));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(1030, 610, 81, 31));
        labelSouvenirs = new QLabel(centralwidget);
        labelSouvenirs->setObjectName("labelSouvenirs");
        labelSouvenirs->setGeometry(QRect(10, 10, 261, 16));
        labelSouvenirs->setFont(font);
        labelDistances = new QLabel(centralwidget);
        labelDistances->setObjectName("labelDistances");
        labelDistances->setGeometry(QRect(10, 110, 241, 16));
        labelDistances->setFont(font);
        ClosestCenterField = new QPushButton(centralwidget);
        ClosestCenterField->setObjectName("ClosestCenterField");
        ClosestCenterField->setGeometry(QRect(30, 600, 141, 31));
        SortByName = new QPushButton(centralwidget);
        SortByName->setObjectName("SortByName");
        SortByName->setGeometry(QRect(30, 330, 141, 31));
        SortByStadium = new QPushButton(centralwidget);
        SortByStadium->setObjectName("SortByStadium");
        SortByStadium->setGeometry(QRect(30, 360, 141, 31));
        SortByAL = new QPushButton(centralwidget);
        SortByAL->setObjectName("SortByAL");
        SortByAL->setGeometry(QRect(30, 390, 141, 31));
        SortByNL = new QPushButton(centralwidget);
        SortByNL->setObjectName("SortByNL");
        SortByNL->setGeometry(QRect(30, 420, 141, 31));
        SortByTypology = new QPushButton(centralwidget);
        SortByTypology->setObjectName("SortByTypology");
        SortByTypology->setGeometry(QRect(30, 450, 141, 31));
        SortByOpenRoof = new QPushButton(centralwidget);
        SortByOpenRoof->setObjectName("SortByOpenRoof");
        SortByOpenRoof->setGeometry(QRect(30, 480, 141, 31));
        DisplayAll = new QPushButton(centralwidget);
        DisplayAll->setObjectName("DisplayAll");
        DisplayAll->setGeometry(QRect(30, 300, 141, 31));
        SortByNewest = new QPushButton(centralwidget);
        SortByNewest->setObjectName("SortByNewest");
        SortByNewest->setGeometry(QRect(30, 510, 141, 31));
        SortByCapacity = new QPushButton(centralwidget);
        SortByCapacity->setObjectName("SortByCapacity");
        SortByCapacity->setGeometry(QRect(30, 540, 141, 31));
        FurthestCenterField = new QPushButton(centralwidget);
        FurthestCenterField->setObjectName("FurthestCenterField");
        FurthestCenterField->setGeometry(QRect(30, 570, 141, 31));
        buttonSpecAlgorithms = new QPushButton(centralwidget);
        buttonSpecAlgorithms->setObjectName("buttonSpecAlgorithms");
        buttonSpecAlgorithms->setGeometry(QRect(950, 160, 191, 32));
        dropdownInformation = new QComboBox(centralwidget);
        dropdownInformation->addItem(QString());
        dropdownInformation->setObjectName("dropdownInformation");
        dropdownInformation->setGeometry(QRect(10, 230, 190, 41));
        dropdownInformation->setEditable(false);
        buttonInfoSubmit = new QPushButton(centralwidget);
        buttonInfoSubmit->setObjectName("buttonInfoSubmit");
        buttonInfoSubmit->setGeometry(QRect(10, 260, 191, 32));
        labelDistances_2 = new QLabel(centralwidget);
        labelDistances_2->setObjectName("labelDistances_2");
        labelDistances_2->setGeometry(QRect(10, 210, 241, 16));
        labelDistances_2->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1163, 39));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(buttonSouvenirPreview, SIGNAL(clicked()), MainWindow, SLOT(previewSouvenirButtonClick()));
        QObject::connect(buttonSouvenirCancel, SIGNAL(clicked()), MainWindow, SLOT(cancelButtonClick()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        dropdownSouvenirPreview->setItemText(0, QCoreApplication::translate("MainWindow", "Select an MLB Team", nullptr));

        buttonSouvenirPreview->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        buttonSouvenirCancel->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        buttonDistancesSubmit->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        dropdownDistances->setItemText(0, QCoreApplication::translate("MainWindow", "Select an MLB Team", nullptr));

        labelTrip->setText(QCoreApplication::translate("MainWindow", "Plan a Trip:", nullptr));
        buttonBasicTrip->setText(QCoreApplication::translate("MainWindow", "Basic Trip", nullptr));
        buttonCustomTrip->setText(QCoreApplication::translate("MainWindow", "Custom Trip", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Back to Login", nullptr));
        labelSouvenirs->setText(QCoreApplication::translate("MainWindow", "Select a MLB Team to Display Souvenirs", nullptr));
        labelDistances->setText(QCoreApplication::translate("MainWindow", "Display Distances from a MLB Team:", nullptr));
        ClosestCenterField->setText(QCoreApplication::translate("MainWindow", "Closest Center Field", nullptr));
        SortByName->setText(QCoreApplication::translate("MainWindow", "Sort By Name", nullptr));
        SortByStadium->setText(QCoreApplication::translate("MainWindow", "Sort By Stadium", nullptr));
        SortByAL->setText(QCoreApplication::translate("MainWindow", "Sort By AL", nullptr));
        SortByNL->setText(QCoreApplication::translate("MainWindow", "Sort By NL", nullptr));
        SortByTypology->setText(QCoreApplication::translate("MainWindow", "Sort By Typology", nullptr));
        SortByOpenRoof->setText(QCoreApplication::translate("MainWindow", "Sort By Open Roof", nullptr));
        DisplayAll->setText(QCoreApplication::translate("MainWindow", "Sort All Information", nullptr));
        SortByNewest->setText(QCoreApplication::translate("MainWindow", "Sort By Oldest Stadium", nullptr));
        SortByCapacity->setText(QCoreApplication::translate("MainWindow", "Sort By Capacity", nullptr));
        FurthestCenterField->setText(QCoreApplication::translate("MainWindow", "Furthest Center Field", nullptr));
        buttonSpecAlgorithms->setText(QCoreApplication::translate("MainWindow", "Special Algorithms", nullptr));
        dropdownInformation->setItemText(0, QCoreApplication::translate("MainWindow", "Select an MLB Team", nullptr));

        buttonInfoSubmit->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        labelDistances_2->setText(QCoreApplication::translate("MainWindow", "Display Information about a Team:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
