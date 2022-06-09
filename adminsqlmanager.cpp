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

    QSqlQuery *query = new QSqlQuery();
    //query->setForwardOnly(true);
    query->prepare("SELECT Username FROM accounts");
    query->exec();

    QSqlQueryModel *tableListModel = new QSqlQueryModel();
    tableListModel->setQuery(*query);


    //QSqlQuery getTableList = getQuery("SELECT Username FROM accounts");
    //QSqlQueryModel *tableListModel = new QSqlQueryModel();
    //tableListModel->setf
    //tableListModel->query().setForwardOnly(true);
    //tableListModel->setQuery("SELECT TABLE_NAME FROM information_schema.tables WHERE TABLE_SCHEMA = 'dbo';");
    //tableListModel->setQuery(getTableList);
    //tableListModel->setQuery(std::move(getTableList));
    ui->TableList->setModel(tableListModel);
    //ui->TableList->model()->setf

    /*
    QSqlTableModel  *mainTableListModel = new QSqlTableModel();

    mainTableListModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mainTableListModel->setTable("Accounts");
    mainTableListModel->select();
    ui->SQLTable->setModel(mainTableListModel);*/

    //database.closeDatabase("tableOnly");

    //mainTableListModel->

}

void AdminSQLManager::runQuery(){

}

AdminSQLManager::~AdminSQLManager()
{
    delete ui;
}
