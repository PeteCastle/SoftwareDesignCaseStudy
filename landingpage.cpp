#include "landingpage.h"
#include "ui_landingpage.h"
#include "global.h"

#include <QSqlQuery>
#include <QDebug>


LandingPage::LandingPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LandingPage)
{
    ui->setupUi(this);
}

LandingPage::~LandingPage()
{
    delete ui;
}

void LandingPage::on_SignUpButton_clicked()
{
    RegisterWindow *registerWindow = new RegisterWindow(this);
    registerWindow->exec();

    ui->EmailLineEdit->setText(registerWindow->getUsername());
    ui->PasswordLineEdit->setText(registerWindow->getPassword());
}


void LandingPage::on_LoginButton_clicked()
{
    InquiryWindow *inquiryWindow = new InquiryWindow(this);
    inquiryWindow->show();
}

