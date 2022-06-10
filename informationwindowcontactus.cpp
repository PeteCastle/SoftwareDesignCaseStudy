#include "informationwindowcontactus.h"
#include "ui_informationwindowcontactus.h"

InformationWindowContactUs::InformationWindowContactUs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWindowContactUs)
{
    ui->setupUi(this);
}

InformationWindowContactUs::~InformationWindowContactUs()
{
    delete ui;
}
