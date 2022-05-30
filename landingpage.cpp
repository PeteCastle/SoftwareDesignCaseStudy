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
    if(ui->EmailLineEdit->text().isEmpty() || ui->PasswordLineEdit->text().isEmpty()){
        QMessageBox::information(this, "Missing fields", "Please enter your username and/or password.");
        return;
    }
    QString authenticateUserQuery = "DECLARE @hashVarchar varchar(128) = ?;"
                                    " SELECT UserID FROM Accounts WHERE Username = ? AND Userpass = HASHBYTES('SHA2_256',@hashVarchar);";
    QStringList loginCredentials = {ui->PasswordLineEdit->text(),ui->EmailLineEdit->text()};
    QSqlQuery authenticateUser = getQuery(authenticateUserQuery, loginCredentials);
    authenticateUser.next();
    int userID = authenticateUser.value(0).toInt();

    if(userID!=0){
        QMessageBox::information(this,"Login success", "Navigating to the inquiry system.");
        InquiryWindow *inquiryWindow = new InquiryWindow(this, userID);
        inquiryWindow->setWindowModality(Qt::ApplicationModal);
        inquiryWindow->show();
    }
    else{
        QMessageBox::warning(this, "Invalid credentials", "Username and/or password is incorrect.  Please try again.");
    }



}

