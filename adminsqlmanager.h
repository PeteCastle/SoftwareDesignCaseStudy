#ifndef ADMINSQLMANAGER_H
#define ADMINSQLMANAGER_H

#include <QWidget>
#include "global.h"

namespace Ui {
class AdminSQLManager;
}

class AdminSQLManager : public QWidget
{
    Q_OBJECT

public:
    explicit AdminSQLManager(QWidget *parent = nullptr);
    ~AdminSQLManager();

private:
    Ui::AdminSQLManager *ui;
};

#endif // ADMINSQLMANAGER_H
