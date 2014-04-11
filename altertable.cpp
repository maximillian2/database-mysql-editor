#include "altertable.h"
#include "ui_altertable.h"
#include <QDebug>


AlterTable::AlterTable(QWidget *parent, QString table) : QDialog(parent), ui(new Ui::AlterTable)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this, QSqlDatabase::database("general"));
    model->setTable("information.schema");
//    QString string("table_name='" + table + "'");
//    model->setFilter(string);
//    model->setQuery("select column_name from information_schema.columns where table_name='" + table + "'"/*, QSqlDatabase::database("general")*/);
//    model->setQuery(query);
//    model->setHeaderData(0, Qt::Horizontal, tr("Column name"));
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    // connecting to database
//    dbConnection = QSqlDatabase::database("general");

    // FIXME: make func checkConnection.
//    bool connected = dbConnection.open();

//    if(!connected)
//        qCritical() << "Error opening database.";

}

AlterTable::~AlterTable()
{
    delete ui;
    dbConnection.close();
}

void AlterTable::on_cancelButton_clicked()
{
    this->reject();
}

void AlterTable::on_saveButton_clicked()
{
    this->accept();
}

void AlterTable::on_addButton_clicked()
{
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->insertRow(model->rowCount());
}

void AlterTable::on_removeButton_clicked()
{
//    QModelIndex currentIndex = ui->tableView->selectionModel()->currentIndex();
//    model->removeRow(currentIndex.row());
}
