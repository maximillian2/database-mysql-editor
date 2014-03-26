#ifndef ADDTABLE_H
#define ADDTABLE_H

#include <QDialog>
#include <QIcon>
#include <QStandardItemModel>
#include <QSqlDatabase>

namespace Ui {
class AddTable;
}

class AddTable : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddTable(QWidget *parent = 0, QString sourceDB = 0);
    ~AddTable();
    
private slots:
    void on_cancelButton_clicked();

    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::AddTable *ui;
    QIcon *addIcon;
    QIcon *removeIcon;
    QStandardItemModel *model;
    QString dbName;
    QSqlDatabase dbConnection;
};

#endif // ADDTABLE_H
