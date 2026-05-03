#ifndef MODCOLLEGES_H
#define MODCOLLEGES_H

#include <QDialog>

namespace Ui {
class ModColleges;
}

class ModColleges : public QDialog
{
    Q_OBJECT

public:
    explicit ModColleges(QWidget *parent = nullptr);
    ~ModColleges();

private slots:
    void on_pushButton_clicked();          // Back
    void on_ColModifybutton_clicked();     // Save checkbox states

private:
    Ui::ModColleges *ui;

    bool ensureDbOpen();
    void ensureCampusAccessTable();
    void syncCampusesIntoAccessTable();
    void loadCampusChecklist();
};

#endif // MODCOLLEGES_H
