#include "informationwindowfaq.h"
#include "ui_informationwindowfaq.h"

InformationWindowFAQ::InformationWindowFAQ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWindowFAQ)
{
    ui->setupUi(this);
}

InformationWindowFAQ::~InformationWindowFAQ()
{
    delete ui;
}
