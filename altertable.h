#ifndef ALTERTABLE_H
#define ALTERTABLE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QStandardItemModel>

namespace Ui {
class AlterTable;
}

class AlterTable : public QDialog
{
    Q_OBJECT
    
public:
    explicit AlterTable(QWidget *parent = 0, QString table = 0);
    ~AlterTable();
    
private slots:
    void on_cancelButton_clicked();
    void on_saveButton_clicked();
    void on_addButton_clicked();
    void on_removeButton_clicked();

private:
    QSqlDatabase dbConnection;
    Ui::AlterTable *ui;
    QSqlTableModel *model;
//    QStandardItemModel* standardModel;
};

#endif // ALTERTABLE_H
