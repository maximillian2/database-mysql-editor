#include "mainwindow.h"
#include "ui_mainwindow.h"

// classes with windows
#include "addtable.h"
#include "altertable.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
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
        db.setDatabaseName(selectedDB);
        updateTablesString = selectedDB;
        refreshTablesCombobox();
}



void MainWindow::on_tableComboBox_currentIndexChanged(const QString &selectedTable)
{
    model = new QSqlQueryModel;
    model->setQuery("select column_name from information_schema.columns where table_name='" + selectedTable + "'", QSqlDatabase::database("general"));

    refreshTableView();

    statusBarDB->setText("Database: " + ui->dbComboBox->currentText());
    ui->statusBar->addWidget(statusBarDB);

    ui->statusBar->addWidget(sbSeparator1);

    statusBarTable->setText("Table: " + ui->tableComboBox->currentText());
    ui->statusBar->addWidget(statusBarTable);

    ui->statusBar->addWidget(sbSeparator2);

    statusBarRows->setText("Rows: " + QString("%1").arg(model->rowCount()));
    ui->statusBar->addWidget(statusBarRows);
}



void MainWindow::refreshTableView()
{
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);
    ui->tableView->show();
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
    tableComboboxQuery = new QSqlQuery(db);
    tableComboboxQuery->exec("SHOW TABLES FROM " + updateTablesString + ";");

    ui->tableComboBox->clear();

    QStringList columnComboboxList;

    while(tableComboboxQuery->next())
    {
         columnComboboxList << tableComboboxQuery->value(0).toString();
    }
    ui->tableComboBox->addItems(columnComboboxList);
}



void MainWindow::on_addButton_clicked()
{
    AddTable *table = new AddTable(this, ui->dbComboBox->currentText());
    table->setModal(true);
    table->show();

    connect(table, SIGNAL(finished(int)), this, SLOT(updateInfo()));
}



void MainWindow::on_dropButton_clicked()
{
//    QSqlQuery query(db);
//    query.exec("DROP TABLE " + ui->dbComboBox->currentText() + "." + ui->tableComboBox->currentText() + ";");
//    refreshTablesCombobox();

    qDebug() << "Deleted " << ui->dbComboBox->currentText() << "->" << ui->tableComboBox->currentText() ;
}



void MainWindow::on_alterButton_clicked()
{
    AlterTable *alterTable = new AlterTable(this, ui->tableComboBox->currentText());
    alterTable->setModal(true);
    alterTable->show();

    connect(alterTable, SIGNAL(finished(int)), this, SLOT(updateInfo()));
}
