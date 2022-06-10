#include "informationwindowgallery.h"
#include "ui_informationwindowgallery.h"

InformationWindowGallery::InformationWindowGallery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWindowGallery)
{
    ui->setupUi(this);
}

InformationWindowGallery::~InformationWindowGallery()
{
    delete ui;
}
