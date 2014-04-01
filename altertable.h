#ifndef ALTERTABLE_H
#define ALTERTABLE_H

#include <QDialog>

namespace Ui {
class AlterTable;
}

class AlterTable : public QDialog
{
    Q_OBJECT
    
public:
    explicit AlterTable(QWidget *parent = 0);
    ~AlterTable();
    
private:
    Ui::AlterTable *ui;
};

#endif // ALTERTABLE_H
