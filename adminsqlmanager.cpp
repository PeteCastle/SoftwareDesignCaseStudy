#include "adminsqlmanager.h"
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QtConcurrent/QtConcurrent>
#include "ui_adminsqlmanager.h"


AdminSQLManager::AdminSQLManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminSQLManager)
{
    ui->setupUi(this);


    QSqlQuery getTableList = getQuery("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE='BASE TABLE'");

    while(getTableList.next()){
        tableList.append(getTableList.value(0).toString());
    }
    tableList.append("(Custom query)");
    ui->TableList->setModel(new QStringListModel(tableList));


}

void AdminSQLManager::updateTableValues(QString queryString, QStringList queryArgs={}){
    QSqlQuery performQueryOperation = getQuery(queryString,queryArgs);

    if(performQueryOperation.lastError().text()!=""){
        QMessageBox::information(this, "An error has occured", performQueryOperation.lastError().text());
        return;
    }

    delete ui->SQLTable->model();

    QList<QSqlRecord> recordList;
    while(performQueryOperation.next()){
        QSqlRecord record = performQueryOperation.record();
        recordList.append(record);
    }
    SQLTableModel *model = new SQLTableModel(0,recordList);

    QList<QWidget*> allSubwidgets = ui->FiltersList->widget()->findChildren<QWidget*>(QString(),Qt::FindChildrenRecursively);
    //int size = allSubwidgets.size();
    for(int i = 0; i < allSubwidgets.size(); i++) {
        delete allSubwidgets[i];
    }

    QList<QSortFilterProxyModel*> sortFilterLists;
    if(recordList.size()!=0){
        for(int i=0; i<recordList[0].count();i++){
            QLabel *label = new QLabel(recordList[0].fieldName(i));
            ui->FiltersList->widget()->layout()->addWidget(label);
            QLineEdit *lineEdit = new QLineEdit();
            ui->FiltersList->widget()->layout()->addWidget(lineEdit);
            QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel();
            i==0 ? proxyModel->setSourceModel(model) : proxyModel->setSourceModel(sortFilterLists[i-1]);
            proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
            proxyModel->setFilterKeyColumn(i);
            connect(lineEdit, &QLineEdit::textChanged,proxyModel, &QSortFilterProxyModel::setFilterFixedString);
            sortFilterLists.append(proxyModel);
        }
        ui->SQLTable->setModel(sortFilterLists[sortFilterLists.size()-1]);
    }
}

AdminSQLManager::~AdminSQLManager()
{
    delete ui;
}


void AdminSQLManager::on_UpdateButton_clicked()
{
    if(ui->TableList->currentText()!="(Custom query)"){
        updateTableValues("SELECT * FROM " + ui->TableList->currentText());
    }
    else if(ui->QueryLineEdit->text()!=""){
        updateTableValues(ui->QueryLineEdit->text());
    }
}
void AdminSQLManager::on_TableList_currentIndexChanged(int index)
{
    ui->QueryLineEdit->setText("");
}


void AdminSQLManager::on_QueryLineEdit_textChanged(const QString &arg1)
{
    ui->TableList->setCurrentText("(Custom query)");
}

