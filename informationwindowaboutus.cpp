#include "informationwindowaboutus.h"
#include "ui_informationwindowaboutus.h"

InformationWindowAboutUs::InformationWindowAboutUs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWindowAboutUs)
{
    ui->setupUi(this);
}

InformationWindowAboutUs::~InformationWindowAboutUs()
{
    delete ui;
}
