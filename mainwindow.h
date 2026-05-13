#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QResizeEvent>
#include "login.h"

#include "basictripwindow.h"
#include "customtripwindow.h"
#include "custominordertrip.h"
#include "dijkstratripwindow.h"
#include "specialalgorithms.h"
#include "tripwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QHeaderView>
#include <QComboBox>
#include <QTableView>
#include <QRect>
#include <QAbstractItemView>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QComboBox;
class QTableView;
class QSqlQueryModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void previewSouvenirButtonClick();
    void cancelButtonClick();
    void on_buttonDistancesSubmit_clicked();
    void on_buttonBasicTrip_clicked();
    void on_buttonCustomTrip_clicked();
    void on_buttonCustomOrderTrip_clicked();
    void on_buttonDijkstraTrip_clicked();
    void on_pushButton_clicked();

    void on_ClosestCenterField_clicked();
    void on_SortByName_clicked();
    void on_SortByStadium_clicked();
    void on_SortByAL_clicked();
    void on_SortByNL_clicked();
    void on_SortByTypology_clicked();
    void on_SortByOpenRoof_clicked();
    void on_DisplayAll_clicked();
    void on_SortByNewest_clicked();
    void on_SortByCapacity_clicked();
    void on_FurthestCenterField_clicked();
    void on_buttonSpecAlgorithms_clicked();
    void on_buttonInfoSubmit_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui = nullptr;
    QSqlDatabase m_db;

    bool ensureDbOpen();
    void populateSouvenirTeamCombo();
    void setupDistanceUi();
    void populateStartingStadiumCombo();
    void loadDistancesForTeam(const QString &fromStadium);
    void loadSouvenirsForTeam(const QString &Team);
    void resizeDistanceTableColumns();
    void sortByName();
    void sortByStadium();
    void displayALTeams();
    void displayNLTeams();
    void sortByTopology();
    void sortByOpenRoof();
    void sortByNewestOpened();
    void sortBySmallestCapacity();
    void loadClosestCenterField();
    void loadFurthestCenterField();
    void displayAllInformation();
    void infoDropdown(const QString &team);


private:
    QComboBox *m_fromTeamCombo = nullptr;
    QComboBox *m_fromStadiumCombo = nullptr;
    QTableView *m_distanceTable = nullptr;
    QSqlQueryModel *m_distanceModel = nullptr;
};

#endif // MAINWINDOW_H
