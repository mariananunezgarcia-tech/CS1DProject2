#ifndef BASICTRIPWINDOW_H
#define BASICTRIPWINDOW_H

#include "tripwindow.h"
#include "mainwindow.h"

//#include <QMainWindow>

//#include <QCoreApplication>
//#include <QDir>
//#include <QFileInfo>
//#include <QMessageBox>

//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QSqlQuery>

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class BasicTripWindow; }
QT_END_NAMESPACE

class BasicTripWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BasicTripWindow(QWidget *parent = nullptr);
    ~BasicTripWindow();

private slots:
    void on_startTripButtonBT_clicked();
    void on_backButtonBT_clicked();   // add this

private:
    Ui::BasicTripWindow *ui;

    bool ensureDbOpen();
    void loadStadiumsFromDb();
    void rebuildNumDropdown(int stadiumCount);
};

#endif // BASICTRIPWINDOW_H
