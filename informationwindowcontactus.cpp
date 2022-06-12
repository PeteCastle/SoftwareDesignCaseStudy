#include "informationwindowcontactus.h"
#include "ui_informationwindowcontactus.h"
#include <QTableView>

InformationWindowContactUs::InformationWindowContactUs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWindowContactUs)
{
    ui->setupUi(this);

    QList<QSqlRecord> contactUsNonAcadList;
    QSqlQuery getContactUsNonAcad = getQuery("SELECT * FROM ContactUsNonAcads");
    while(getContactUsNonAcad.next()) contactUsNonAcadList.append(QSqlRecord(getContactUsNonAcad.record()));
    SQLTableModel *contactUsNonAcad = new SQLTableModel({},contactUsNonAcadList);
    ui->NonAcadsTable->setModel(contactUsNonAcad);
    ui->NonAcadsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    QList<QSqlRecord> contactUsAcadList;
    QSqlQuery getContactUsAcad = getQuery("SELECT * FROM ContactUsAcads");
    while(getContactUsAcad.next()) contactUsAcadList.append(QSqlRecord(getContactUsAcad.record()));
    SQLTableModel *contactUsAcad = new SQLTableModel({},contactUsAcadList);
    ui->AcadsTable->setModel(contactUsAcad);
    ui->AcadsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    QList<QSqlRecord> contactUsOthersList;
    QSqlQuery getContactUsOthers = getQuery("SELECT * FROM ContactUsOthers");
    while(getContactUsOthers.next()) contactUsOthersList.append(QSqlRecord(getContactUsOthers.record()));
    SQLTableModel *contactOthers = new SQLTableModel({},contactUsOthersList);
    ui->OthersTable->setModel(contactOthers);
    ui->OthersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->OthersTable->resizeRowsToContents();

}

InformationWindowContactUs::~InformationWindowContactUs()
{
    delete ui;
}
