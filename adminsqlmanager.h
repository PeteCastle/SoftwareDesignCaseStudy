#ifndef ADMINSQLMANAGER_H
#define ADMINSQLMANAGER_H

#include <QWidget>
#include "global.h"
#include "qsqlfield.h"
#include <QSqlRecord>

namespace Ui {
class AdminSQLManager;
}

class AdminSQLManager : public QWidget
{
    Q_OBJECT

public:
    explicit AdminSQLManager(QWidget *parent = nullptr);
    ~AdminSQLManager();

private slots:

    void on_UpdateButton_clicked();

    void on_TableList_currentIndexChanged(int index);

    void on_QueryLineEdit_textChanged(const QString &arg1);

    void on_AddButton_clicked();

    void on_RemoveButton_clicked();

private:
    Ui::AdminSQLManager *ui;
    void updateTableValues(QString queryString, QStringList queryArgs);
    QStringList tableList;
};



#endif // ADMINSQLMANAGER_H
