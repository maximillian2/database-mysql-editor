#include "addtable.h"
#include "ui_addtable.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

AddTable::AddTable(QWidget *parent, QString sourceDB) : QDialog(parent), ui(new Ui::AddTable)
{
    ui->setupUi(this);

    addIcon = new QIcon(":/images/square-add.png");
    removeIcon = new QIcon(":/images/square-cross.png");

    ui->addButton->setIcon(*addIcon);
    ui->removeButton->setIcon(*removeIcon);

    // where to save table
    db = sourceDB;

    model = new QStandardItemModel();
    model->setColumnCount(3);
    QStringList list;
    list << "Name" << "Type" << "Length";
    model->setHorizontalHeaderLabels(list);

    ui->tableView->setModel(model);
    ui->tableView->show();
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    // connecting to database
    dbConnection = QSqlDatabase::database("general");

    // FIXME: make func checkConnection.
    bool connected = dbConnection.open();

    if(!connected)
        qCritical() << "Error opening database.";
}



AddTable::~AddTable()
{
    delete ui;

    delete addIcon;
    delete removeIcon;
    dbConnection.close();
}



void AddTable::on_cancelButton_clicked()
{
    this->reject();
}



void AddTable::on_addButton_clicked()
{
    model->insertRow(model->rowCount());
}



void AddTable::on_removeButton_clicked()
{
    QModelIndex currentIndex = ui->tableView->selectionModel()->currentIndex();
    model->removeRow(currentIndex.row());
}



void AddTable::on_saveButton_clicked()
{
    QString output;
    /*QList<QString>*/ QStringList parsedOutput[model->rowCount()];

    QModelIndex item;
    for(int i = 0; i < model->rowCount(); i++)
    {
        for(int j = 0; j < model->columnCount(); j++)
        {
            item = model->index(i, j);
            output += item.data(Qt::DisplayRole).toString();
            output += " ";
        }
        parsedOutput[i] = output.split(" ");
        output.clear();
    }

    QString finalString;

    finalString = "CREATE TABLE " + ui->tableNamelineEdit->text() + "(\n";
    for(int i = 0; i < model->rowCount(); i++)
    {
        if(i == model->rowCount()-1)
        {
            finalString += parsedOutput[i].at(0);
            finalString += " ";
            finalString += parsedOutput[i].at(1);
            finalString += "(" + parsedOutput[i].at(2) + ")\n";
            finalString += ");";
            break;
        }
        finalString += parsedOutput[i].at(0);
        finalString += " ";
        finalString += parsedOutput[i].at(1);
        finalString += "(" + parsedOutput[i].at(2) + "),\n";
    }
    qDebug() << finalString;
    QSqlQuery *query = new QSqlQuery(dbConnection);

    qDebug() << query->exec(finalString);
    qDebug() << query->lastError().text();

    this->accept();

    delete query;
}
