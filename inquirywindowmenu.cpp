#include "inquirywindowmenu.h"
#include "ui_inquirywindowmenu.h"

InquiryWindowMenu::InquiryWindowMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InquiryWindowMenu)
{
    ui->setupUi(this);
}

InquiryWindowMenu::~InquiryWindowMenu()
{
    delete ui;
}
