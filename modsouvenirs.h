#ifndef MODSOUVENIRS_H
#define MODSOUVENIRS_H

#include <QDialog>
#include <QString>

namespace Ui {
class ModSouvenirs;
}

class ModSouvenirs : public QDialog
{
    Q_OBJECT

public:
    explicit ModSouvenirs(QWidget *parent = nullptr);
    ~ModSouvenirs();

    void setSelectedCampus(const QString &campus);

private slots:
    void on_pushButton_clicked();              // Back
    void on_cancelButt_clicked();              // Reset to default state
    void on_SubmitButt_clicked();              // Confirm selected college
    void on_SuvModifybutton_clicked();         // Save checkbox states
    void on_comboBox_currentIndexChanged(int); // Clear pending UI when selection changes
    void on_addSouvButt_clicked();             // Open add souvenir window
    void on_changePrButt_clicked();            // Open change price window
    void on_delSouvButt_clicked();             // Open delete souvenir window

private:
    Ui::ModSouvenirs *ui;

    bool ensureDbOpen();
    void ensureSouvenirAccessTable();
    void syncSouvenirsIntoAccessTable();
    void loadCollegeDropdown();
    void clearSouvenirTable();
    void loadSouvenirsForCollege(const QString &campus);
    void resetToDefaultState();
    int souvenirCountForCampus(const QString &campus);

    QString m_selectedCampus;
};

#endif // MODSOUVENIRS_H
