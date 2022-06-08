#include "inquirywindowhome.h"
#include "ui_inquirywindowhome.h"

InquiryWindowHome::InquiryWindowHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InquiryWindowHome)
{
    ui->setupUi(this);
}

InquiryWindowHome::~InquiryWindowHome()
{
    delete ui;
}
