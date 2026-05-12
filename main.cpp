<<<<<<< HEAD
#include <QApplication>
#include <QFile>

#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("C:/Users/l4syt/OneDrive/Documents/QtProjects/Project2take2/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
    }
=======
/**
 * @file main.cpp
 * @brief Entry point of the Campus Tour application.
 *
 * Creates the Qt application object, opens the login window,
 * and starts the event loop.
 */

#include <QApplication>
#include "login.h"

/*
 * Function: main
 * Purpose : Initializes the application, displays the login
 *           window, and starts the Qt event loop.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    Login w;
    w.show();
    return a.exec();
}
// test CI trigger
