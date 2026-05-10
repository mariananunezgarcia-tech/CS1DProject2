#ifndef SPECIALALGORITHMS_H
#define SPECIALALGORITHMS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QStringListModel>

#include <vector>

namespace Ui {
class SearchAlgorithms;
}

class MainWindow;

class SearchAlgorithms : public QWidget
{
    Q_OBJECT

public:
    explicit SearchAlgorithms(QWidget *parent = nullptr);
    ~SearchAlgorithms();

private slots:
    void on_buttonBFS_clicked();
    void on_buttonBack_clicked();
    void on_buttonDFS_clicked();

    void on_buttonMST_clicked();

private:
    struct DfsEdge
    {
        QString dest;
        int miles;
    };


    struct Neighbor
    {
        int stadiumIndex;
        int mileage;
    };

    struct EdgeRecord
    {
        int from;
        int to;
        int mileage;
    };

    Ui::SearchAlgorithms *ui = nullptr;
    QSqlDatabase m_db;
    QStringListModel *m_resultsModel = nullptr;

    bool ensureDbOpen();
    void loadStadiums();
    QString runBfsFrom(const QString &startStadium);
    void showResultsInListView(const QString &text);

    QString runDfsFromOracle();
};

#endif // SPECIALALGORITHMS_H
