/********************************************************************************
** Form generated from reading UI file 'specialalgorithms.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPECIALALGORITHMS_H
#define UI_SPECIALALGORITHMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchAlgorithms
{
public:
    QComboBox *comboSelStadium;
    QPushButton *buttonBack;
    QLabel *labelTitle;
    QLabel *labelSelStadium;
    QLabel *labelSelAlg;
    QPushButton *buttonMST;
    QPushButton *buttonDFS;
    QPushButton *buttonBFS;
    QListView *listViewResults;

    void setupUi(QWidget *SearchAlgorithms)
    {
        if (SearchAlgorithms->objectName().isEmpty())
            SearchAlgorithms->setObjectName("SearchAlgorithms");
        SearchAlgorithms->resize(960, 570);
        comboSelStadium = new QComboBox(SearchAlgorithms);
        comboSelStadium->setObjectName("comboSelStadium");
        comboSelStadium->setGeometry(QRect(60, 110, 121, 24));
        buttonBack = new QPushButton(SearchAlgorithms);
        buttonBack->setObjectName("buttonBack");
        buttonBack->setGeometry(QRect(80, 340, 80, 24));
        labelTitle = new QLabel(SearchAlgorithms);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setGeometry(QRect(20, 25, 200, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(15);
        font.setBold(true);
        font.setItalic(true);
        labelTitle->setFont(font);
        labelTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelSelStadium = new QLabel(SearchAlgorithms);
        labelSelStadium->setObjectName("labelSelStadium");
        labelSelStadium->setGeometry(QRect(10, 80, 220, 20));
        labelSelStadium->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelSelAlg = new QLabel(SearchAlgorithms);
        labelSelAlg->setObjectName("labelSelAlg");
        labelSelAlg->setGeometry(QRect(5, 160, 230, 20));
        labelSelAlg->setAlignment(Qt::AlignmentFlag::AlignCenter);
        buttonMST = new QPushButton(SearchAlgorithms);
        buttonMST->setObjectName("buttonMST");
        buttonMST->setGeometry(QRect(45, 190, 150, 25));
        buttonDFS = new QPushButton(SearchAlgorithms);
        buttonDFS->setObjectName("buttonDFS");
        buttonDFS->setGeometry(QRect(45, 225, 150, 25));
        buttonBFS = new QPushButton(SearchAlgorithms);
        buttonBFS->setObjectName("buttonBFS");
        buttonBFS->setGeometry(QRect(45, 260, 150, 25));
        listViewResults = new QListView(SearchAlgorithms);
        listViewResults->setObjectName("listViewResults");
        listViewResults->setGeometry(QRect(250, 10, 700, 550));

        retranslateUi(SearchAlgorithms);

        QMetaObject::connectSlotsByName(SearchAlgorithms);
    } // setupUi

    void retranslateUi(QWidget *SearchAlgorithms)
    {
        SearchAlgorithms->setWindowTitle(QCoreApplication::translate("SearchAlgorithms", "Form", nullptr));
        buttonBack->setText(QCoreApplication::translate("SearchAlgorithms", "Back", nullptr));
        labelTitle->setText(QCoreApplication::translate("SearchAlgorithms", "Special Algorithms", nullptr));
        labelSelStadium->setText(QCoreApplication::translate("SearchAlgorithms", "Select the stadium you want to start at:", nullptr));
        labelSelAlg->setText(QCoreApplication::translate("SearchAlgorithms", "Select the algorithm you want to execute:", nullptr));
        buttonMST->setText(QCoreApplication::translate("SearchAlgorithms", "Minimum Spanning Tree", nullptr));
        buttonDFS->setText(QCoreApplication::translate("SearchAlgorithms", "Depth-First Search", nullptr));
        buttonBFS->setText(QCoreApplication::translate("SearchAlgorithms", "Breadth-First Search", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchAlgorithms: public Ui_SearchAlgorithms {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECIALALGORITHMS_H
