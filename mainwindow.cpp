#include "mainwindow.h"
#include "ui_mainwindow.h"

// classes with windows
#include "addtable.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

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

    connectDB();

    dbComboboxQuery = new QSqlQuery("SHOW DATABASES;", db);

    QStringList tablesComboboxList;
    while(dbComboboxQuery->next())
    {
            tablesComboboxList << dbComboboxQuery->value(0).toString();
    }

    ui->dbComboBox->addItems(tablesComboboxList);

    stringToRemove = '0';     // default value
    model->setHeaderData(0, Qt::Horizontal, tr("Column name"));

    // set up alter field
    QStringList typeList;
    typeList << "integer" << "varchar" << "float" << "boolean" << "date";
    ui->typesComboBox->addItems(typeList);

    qDebug() << ui->alterTableLayout->count();
//    for(int i = 0; i < ui->alterTableLayout->count(); i++)
//    {
//      ui->alterTableLayout->itemAt(i)->widget()->setVisible(false);
//    }
    ui->rowLineEdit->setEnabled(false);
    ui->typesComboBox->setEnabled(false);
    ui->lengthSpinBox->setEnabled(false);
    ui->addRowButton->setEnabled(false);
    ui->deleteRowButton->setEnabled(false);
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

void MainWindow::setDefaultFields()
{
    ui->rowLineEdit->clear();
    ui->lengthSpinBox->setValue(0); // ??
    ui->typesComboBox->setCurrentIndex(0);
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

void MainWindow::on_checkBox_toggled(bool checked)
{
    setDefaultFields();

    if(checked)
    {
        ui->rowLineEdit->setEnabled(true);
        ui->typesComboBox->setEnabled(true);
        ui->lengthSpinBox->setEnabled(true);
        ui->addRowButton->setEnabled(true);
        ui->deleteRowButton->setEnabled(true);
    } else {
        ui->rowLineEdit->setEnabled(false);
        ui->typesComboBox->setEnabled(false);
        ui->lengthSpinBox->setEnabled(false);
        ui->addRowButton->setEnabled(false);
        ui->deleteRowButton->setEnabled(false);
    }
}

void MainWindow::on_addRowButton_clicked()
{
    QString completeString;
    int length = ui->lengthSpinBox->value();

    completeString = "ALTER TABLE " + ui->tableComboBox->currentText()
                    + " ADD " + ui->rowLineEdit->text() + " " + ui->typesComboBox->currentText()
                    + "(" + QString::number(length) + ")";

    // open connection to write things
    QSqlDatabase::database("general").open();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("general"));
    query->exec(completeString);

    // TODO: clean all fields after toggling 'alter'
    // clean up some code :/
    updateInfo();
}

void MainWindow::on_deleteRowButton_clicked()
{
    QSqlDatabase::database("general").open();
    QSqlQuery* deleteQuery = new QSqlQuery(QSqlDatabase::database("general"));

    deleteQuery->exec("ALTER TABLE " + ui->tableComboBox->currentText() + " DROP COLUMN " + stringToRemove + ";");
    updateInfo();
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QVariant variant = index.data();
    stringToRemove = variant.toString();
}
