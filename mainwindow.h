#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QLabel>
#include <QFrame>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void refreshTablesCombobox();
    void refreshTableView();
    bool connectDB();
    void setDefaultFields();
    ~MainWindow();
    
public slots:
    void updateInfo();

private slots:
    void on_dbComboBox_currentIndexChanged(const QString &selectedDB);
    void on_tableComboBox_currentIndexChanged(const QString &selectedTable);
    void on_addButton_clicked();
    void on_dropButton_clicked();
    void on_checkBox_toggled(bool checked);
    void on_addRowButton_clicked();
    void on_deleteRowButton_clicked();
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery *dbComboboxQuery;
    QSqlQuery *tableComboboxQuery;
    QSqlQueryModel *model;
    QString updateTablesString;
    QString stringToRemove;

    QLabel *statusBarDB;
    QLabel *statusBarTable;
    QLabel *statusBarRows;

    QFrame *sbSeparator1;
    QFrame *sbSeparator2;

};

#endif // MAINWINDOW_H
