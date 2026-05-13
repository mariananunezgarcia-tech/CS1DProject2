#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui { class Login; }
class MainWindow;

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_CancelButt_clicked();
    void on_LogButton_clicked();

private:
    Ui::Login *ui = nullptr;
    MainWindow *m_mainWindow = nullptr;
};

#endif // LOGIN_H
