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

class SQLTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    SQLTableModel(QObject *parent = {}, QList<QSqlRecord> recordList={}) : QAbstractTableModel{parent}{
        this->recordList = recordList;
    };
    void addRows(const QSqlRecord record){
        beginInsertRows({}, record.count(), record.count());
        recordList.append(record);
        endInsertRows();
    }
    int rowCount(const QModelIndex &) const{
        return recordList.count();
    }
    int columnCount(const QModelIndex &) const{
        return recordList.size()!=0 ? recordList[0].count() : 0 ;
    }
    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole){
           return recordList.size()!=0 ? QVariant(recordList[index.row()].value(index.column())) : QVariant();
        }
        return QVariant();
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const{
        if (role == Qt::DisplayRole){
            if (orientation == Qt::Horizontal) {
                return recordList.size()!=0 ? QVariant(recordList[0].fieldName(section)) : QVariant();
            }
        }
        return QVariant();
    }
private:
    QList<QSqlRecord> recordList;
};

#endif // ADMINSQLMANAGER_H
