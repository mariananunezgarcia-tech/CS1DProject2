#ifndef CUSTOMTRIPWINDOW_H
#define CUSTOMTRIPWINDOW_H

#include "tripwindow.h"
#include "mainwindow.h"

//#include <QCoreApplication>
//#include <QDir>
//#include <QFileInfo>
//#include <QMessageBox>
//#include <QTableWidgetItem>

//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QSqlQuery>
//#include <QString>

//#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CustomTripWindow; }
QT_END_NAMESPACE

class CustomTripWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomTripWindow(QWidget *parent = nullptr);
    ~CustomTripWindow();

private slots:
    void on_confirmButtonCT_clicked();
    void on_startTripButtonCT_clicked();
    void on_backButtCT_clicked();   // add this

private:
    Ui::CustomTripWindow *ui;

    bool ensureDbOpen();
    QStringList loadStadiums();

    QString m_startLocked;
    QStringList m_allStadiums;
};

#endif // CUSTOMTRIPWINDOW_H
