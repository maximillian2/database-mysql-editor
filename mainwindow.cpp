#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addtable.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << "Start of constructor.";
    ui->setupUi(this);

    statusBarDB     = new QLabel();
    statusBarTable  = new QLabel();
    statusBarRows   = new QLabel();

    sbSeparator1 = new QFrame();
    sbSeparator1->setFrameStyle(QFrame::VLine | QFrame::Sunken);

    sbSeparator2 = new QFrame();
    sbSeparator2->setFrameStyle(QFrame::VLine | QFrame::Sunken);

    qDebug() << "Connect result: " << connectDB();

    dbComboboxQuery = new QSqlQuery("SHOW DATABASES;", db);

    QStringList tablesComboboxList;
    while(dbComboboxQuery->next())
    {
            tablesComboboxList << dbComboboxQuery->value(0).toString();
    }

    ui->dbComboBox->addItems(tablesComboboxList);

    qDebug() << "End of constructor.";
}



MainWindow::~MainWindow()
{
    delete ui;
    delete dbComboboxQuery;
    delete tableComboboxQuery;
    db.close();
}

void MainWindow::updateInfo()
{
    refreshTablesCombobox();
    refreshTableView();
}



void MainWindow::on_dbComboBox_currentIndexChanged(const QString &selectedDB)
{
        qDebug() << "Start of on_dbComboBox_currentIndexChanged.";

        db.setDatabaseName(selectedDB);
        updateTablesString = selectedDB;
        refreshTablesCombobox();

        qDebug() << "End of on_dbComboBox_currentIndexChanged.";
}



void MainWindow::on_tableComboBox_currentIndexChanged(const QString &selectedTable)
{
    qDebug() << "Start of on_tableComboBox_currentIndexChanged.";

    model = new QSqlQueryModel;

    // FUCKING BUG was HERE!!!!!!!!!
    model->setQuery("select column_name from information_schema.columns where table_name='" + selectedTable + "'", QSqlDatabase::database("general"));
    // -------------------------

    refreshTableView();

    statusBarDB->setText("Database: " + ui->dbComboBox->currentText());
    ui->statusBar->addWidget(statusBarDB);

    ui->statusBar->addWidget(sbSeparator1);

    statusBarTable->setText("Table: " + ui->tableComboBox->currentText());
    ui->statusBar->addWidget(statusBarTable);

    ui->statusBar->addWidget(sbSeparator2);

    statusBarRows->setText("Rows: " + QString("%1").arg(model->rowCount()));
    ui->statusBar->addWidget(statusBarRows);


    qDebug() << "End of on_tableComboBox_currentIndexChanged.";
}



void MainWindow::refreshTableView()
{
    qDebug() << "Start refreshTableView()";
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);
    ui->tableView->show();
    qDebug() << "End of refreshTableView()";
}



bool MainWindow::connectDB()
{
    // connecting to database
    db = QSqlDatabase::addDatabase("QMYSQL", "general");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("123");

    // open check
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
        db.lastError().text());
        return false;
    }
    return true;
}



void MainWindow::refreshTablesCombobox()
{
    qDebug() << "Start of refreshTablesCombobox.";

    tableComboboxQuery = new QSqlQuery(db);
    qDebug() << "Exec result:";
    qDebug() << tableComboboxQuery->exec("show tables from " + updateTablesString + ";");

    ui->tableComboBox->clear();

    QStringList columnComboboxList;

    while(tableComboboxQuery->next())
    {
            columnComboboxList << tableComboboxQuery->value(0).toString();
    }
    ui->tableComboBox->addItems(columnComboboxList);

    qDebug() << "End of refreshTablesCombobox.";
}



void MainWindow::on_addButton_clicked()
{
    qDebug() << "Start of on_addButton_clicked().";
    AddTable *table = new AddTable(this, ui->dbComboBox->currentText());

    table->setModal(true);
    table->show();

    connect(table, SIGNAL(finished(int)), this, SLOT(updateInfo()));

    qDebug() << "Stop of on_addButton_clicked().";
}



void MainWindow::on_dropButton_clicked()
{
    qDebug() << "Start of on_dropButton_clicked().";

    QSqlQuery query(db);
    query.exec("DROP TABLE " + ui->dbComboBox->currentText() + "." + ui->tableComboBox->currentText() + ";");


    refreshTablesCombobox();

    qDebug() << "Stop of on_dropButton_clicked().";
}
