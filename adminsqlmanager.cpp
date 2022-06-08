#include "adminsqlmanager.h"
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include "ui_adminsqlmanager.h"

AdminSQLManager::AdminSQLManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminSQLManager)
{
    ui->setupUi(this);

    //QSqlQuery getTableList = getQuery("SELECT Username FROM accounts");
    //QSqlQueryModel *tableListModel = new QSqlQueryModel();
    //tableListModel->setf
    //tableListModel->setQuery("SELECT TABLE_NAME FROM information_schema.tables WHERE TABLE_SCHEMA = 'dbo';");
    //tableListModel->setQuery(getTableList);
    //tableListModel->setQuery(std::move(getTableList));
    //ui->TableList->setModel(tableListModel);

    QSqlTableModel  *mainTableListModel = new QSqlTableModel();
    mainTableListModel->setTable("Accounts");
    mainTableListModel->select();
    ui->SQLTable->setModel(mainTableListModel);



}

AdminSQLManager::~AdminSQLManager()
{
    delete ui;
}
