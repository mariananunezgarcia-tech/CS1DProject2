#ifndef TRIPWINDOW_H
#define TRIPWINDOW_H

#include "dijkstraalgo.h"
#include "summarywindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QIntValidator>

//#include <QDialog>
#include <QHash>
#include <QSet>
//#include <QString>

//#include <vector>
#include <queue>
#include <utility>
#include <cstddef>

QT_BEGIN_NAMESPACE
namespace Ui { class tripWindow; }
QT_END_NAMESPACE

class QLineEdit;
class QListWidget;
class QPushButton;
class QTableWidget;

class tripWindow : public QDialog
{
    Q_OBJECT

public:
    explicit tripWindow(const QString &startStadium,
                        const QStringList &stadiums,
                        int maxStops,
                        bool forceExact,
                        QWidget *parent = nullptr, QString type = "default");
    ~tripWindow();

private slots:
    void onGoNextClicked();
    void onBuyClicked();

private:
    Ui::tripWindow *ui;

    // =================================================================
    // Vector-backed Binary Search Tree (BST)
    // =================================================================
private:
    struct VectorBst
    {
        struct Node
        {
            int key = 0;           // destination index
            double value = 0.0;    // miles
            int left = -1;
            int right = -1;
        };

        int root = -1;
        std::vector<Node> nodes;

        mutable std::vector<std::pair<int, double>> q;
        mutable size_t qHead = 0;

        void clear()
        {
            root = -1;
            nodes.clear();
            q.clear();
            qHead = 0;
        }

        void insert(int key, double value)
        {
            root = insertRec(root, key, value);
        }

        bool find(int key, double &out) const
        {
            return findRec(root, key, out);
        }

        void resetQueueInOrder() const
        {
            q.clear();
            qHead = 0;
            inorderRec(root);
        }

        bool queueEmpty() const
        {
            return qHead >= q.size();
        }

        std::pair<int, double> queuePop() const
        {
            if (queueEmpty())
                return std::make_pair(-1, 0.0);
            return q[qHead++];
        }

    private:
        int insertRec(int cur, int key, double value)
        {
            if (cur == -1)
            {
                nodes.push_back(Node{key, value, -1, -1});
                return static_cast<int>(nodes.size()) - 1;
            }

            if (key < nodes[static_cast<size_t>(cur)].key)
            {
                const int child = insertRec(nodes[static_cast<size_t>(cur)].left, key, value);
                nodes[static_cast<size_t>(cur)].left = child;
            }
            else if (key > nodes[static_cast<size_t>(cur)].key)
            {
                const int child = insertRec(nodes[static_cast<size_t>(cur)].right, key, value);
                nodes[static_cast<size_t>(cur)].right = child;
            }
            else
            {
                nodes[static_cast<size_t>(cur)].value = value;
            }

            return cur;
        }

        bool findRec(int cur, int key, double &out) const
        {
            if (cur == -1)
                return false;

            const Node &n = nodes[static_cast<size_t>(cur)];
            if (key == n.key)
            {
                out = n.value;
                return true;
            }

            if (key < n.key)
                return findRec(n.left, key, out);
            return findRec(n.right, key, out);
        }

        void inorderRec(int cur) const
        {
            if (cur == -1)
                return;

            const Node &n = nodes[static_cast<size_t>(cur)];
            inorderRec(n.left);
            q.push_back(std::make_pair(n.key, n.value));
            inorderRec(n.right);
        }
    };

    struct Souvenir
    {
        QString stadium;
        QString name;
        double price = 0.0;
        int quantity = 1;
    };

    // UI elements
    QLineEdit   *m_currentEdit   = nullptr;
    QLineEdit   *m_totalEdit     = nullptr;
    QLineEdit   *m_souvenirCount = nullptr;
    QLineEdit   *m_souvenirCost  = nullptr;
    QLineEdit   *m_nextEdit      = nullptr;
    QLineEdit   *m_nextDistEdit  = nullptr;
    QListWidget *m_routeList     = nullptr;
    QPushButton *m_goNextBtn     = nullptr;
    QPushButton *m_buyBtn        = nullptr;
    QTableWidget *m_souvTable    = nullptr;

    void buildRuntimeUi();
    void setReadOnly(QLineEdit *edit);

    QString type = "default";
    double totalDist = 0.0;

    // Routing / DB
    static QString norm(const QString &s);
    bool ensureDbOpen();
    void loadAllDistances();
    double dist(const QString &a, const QString &b) const;

    QSet<QString> m_visibleStadiums;
    void loadVisibleStadiums();
    bool isVisibleStadium(const QString &stadium) const;

    QString m_start;
    std::vector<QString> m_candidates;
    int m_maxStops = 1;
    bool m_forceExact = false;

    QHash<QString, QHash<QString, double>> m_dist;

    std::vector<QString> m_route;
    std::vector<double>  m_legs;
    int m_index = 0;
    double m_traveled = 0.0;

    std::queue<std::pair<QString, double>> m_stepQueue;
    void buildStepQueue();

    void planNearestNeighbor();
    void nearestStepRecursive(const QString &current, QSet<QString> &visited);
    void customStep(const QString &current, QSet<QString> &visited, QVector<vertex> allStadiums);
    void customOrderStep(const QString &current, QSet<QString> &visited, QVector<vertex> allStadiums);

    void planExactDp();
    double tspRecursive(int mask, int last);

    std::vector<QString> m_nodes;
    std::vector<VectorBst> m_adj;
    std::vector<std::vector<double>> m_memo;
    std::vector<std::vector<int>> m_next;
    int m_allMask = 0;

    // Souvenirs
    std::queue<Souvenir> m_purchasedSouvenirs;
    int m_totalSouvenirCount = 0;
    double m_totalSouvenirCost = 0.0;

    void loadSouvenirsForCurrentStadium();
    void updateSouvenirTotals();
    void clearSouvenirChecks();

    // UI refresh
    void refreshUi();
    void populateRouteList();
};

#endif // TRIPWINDOW_H
