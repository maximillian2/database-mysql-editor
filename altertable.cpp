#include "altertable.h"
#include "ui_altertable.h"

AlterTable::AlterTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlterTable)
{
    ui->setupUi(this);
}

AlterTable::~AlterTable()
{
    delete ui;
}
