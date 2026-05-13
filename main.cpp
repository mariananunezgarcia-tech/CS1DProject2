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
    Login w;
    w.show();
    return a.exec();
}
