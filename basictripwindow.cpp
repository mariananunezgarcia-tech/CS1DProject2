#include "basictripwindow.h"
#include "ui_basictripwindow.h"

BasicTripWindow::BasicTripWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BasicTripWindow)
{
    ui->setupUi(this);
    setWindowTitle("Basic Trip");

    if (ensureDbOpen())
        loadStadiumsFromDb();

    // Number of visitable stadiums after choosing a starting stadium
    rebuildNumDropdown(ui->selectStartingStadiumDropdownBT->count() - 1);

    // No default stadium selected
    ui->selectStartingStadiumDropdownBT->setCurrentIndex(-1);
}

BasicTripWindow::~BasicTripWindow()
{
    delete ui;
}

bool BasicTripWindow::ensureDbOpen()
{
    QSqlDatabase db;

    if (QSqlDatabase::contains())
    {
        db = QSqlDatabase::database();
        if (db.isOpen())
            return true;
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    const QString exeDir = QCoreApplication::applicationDirPath();
    QStringList candidates;

    // Look in exe dir and walk upward a few levels (helps when you keep the DB in the project root)
    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("stadium_tour.sqlite");
        if (!d.cdUp())
            break;
    }

    // Also try current working directory (Qt Creator often sets this to the build folder)
    candidates << QDir::current().filePath("stadium_tour.sqlite");

QString dbPath;
    for (const QString &p : candidates)
    {
        if (QFileInfo::exists(p))
        {
            dbPath = p;
            break;
        }
    }
    if (dbPath.isEmpty())
        return false;

    db.setDatabaseName(dbPath);
    return db.open();
}

void BasicTripWindow::loadStadiumsFromDb()
{
    ui->selectStartingStadiumDropdownBT->clear();

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT campus
        FROM campus_access
        WHERE enabled = 1
            AND TRIM(campus) <> ''
        ORDER BY campus ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load campus list:\n" + q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString c = q.value(0).toString().trimmed();
        if (!c.isEmpty())
            ui->selectStartingStadiumDropdownBT->addItem(c);
    }
}

void BasicTripWindow::rebuildNumDropdown(int campusCount)
{
    ui->numStadiumstoVisitDropdownBT->clear();

    // campusCount = how many OTHER stadiums can still be visited
    if (campusCount < 0)
        campusCount = 0;

    ui->numStadiumstoVisitDropdownBT->setEnabled(true);

    // Allow visiting zero additional stadiums
    for (int i = 0; i <= campusCount; ++i)
        ui->numStadiumstoVisitDropdownBT->addItem(QString::number(i));
}

void BasicTripWindow::on_startTripButtonBT_clicked()
{
    const QString start = ui->selectStartingStadiumDropdownBT->currentText().trimmed();
    const int maxStops = ui->numStadiumstoVisitDropdownBT->currentText().toInt();

    if (start.isEmpty())
    {
        QMessageBox::information(this, "Select Start",
                                 "Please select a starting stadium.");
        return;
    }

    QStringList stadiums;
    for (int i = 0; i < ui->selectStartingStadiumDropdownBT->count(); ++i)
        stadiums << ui->selectStartingStadiumDropdownBT->itemText(i);

    this->close();

    tripWindow dlg(start, stadiums, maxStops, /*forceExact=*/false, nullptr);
    dlg.setModal(true);
    dlg.exec();

    if (parentWidget())
        parentWidget()->show();
}

void BasicTripWindow::on_backButtonBT_clicked()
{
    MainWindow *mainWin = new MainWindow(nullptr);
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    mainWin->show();

    this->close();
}
