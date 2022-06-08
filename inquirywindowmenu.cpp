#include "inquirywindowmenu.h"
#include "ui_inquirywindowmenu.h"

InquiryWindowMenu::InquiryWindowMenu(QWidget *parent, QString menuName, QString menuIconLocation) :
    QWidget(parent),
    ui(new Ui::InquiryWindowMenu)
{
    ui->setupUi(this);

    QPixmap pixmap(menuIconLocation);
    pixmap = pixmap.scaled(100,100,Qt::KeepAspectRatio,Qt::FastTransformation);
    ui->MenuPicture->setPixmap(pixmap);
    ui->MenuName->setText(menuName);
}

InquiryWindowMenu::~InquiryWindowMenu()
{
    delete ui;
}
