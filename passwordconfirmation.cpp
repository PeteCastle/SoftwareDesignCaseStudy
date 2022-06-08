#include "passwordconfirmation.h"
#include "ui_passwordconfirmation.h"

PasswordConfirmation::PasswordConfirmation(QWidget *parent,QString labelContents, QString userFullName, int buttonBehavior, int userID) :
    QDialog(parent),
    ui(new Ui::PasswordConfirmation)
{
    ui->setupUi(this);
    ui->LabelContents->setText(labelContents);
    ui->UserFullName->setText(userFullName);


    connect(ui->ConfirmPassword,&QPushButton::clicked,[this, buttonBehavior,userID]{
        triggerButton(buttonBehavior,userID);
    });
}

bool PasswordConfirmation::getPasswordAuthenticated(){
    return isAuthenticated;
}

QString PasswordConfirmation::getPasswordInput(){
    return ui->PasswordLineEdit->text();
}

PasswordConfirmation::~PasswordConfirmation()
{
    delete ui;
}

void PasswordConfirmation::triggerButton(int buttonBehavior, int userID){
    if(buttonBehavior==1){
        QString confirmPasswordQuery = "DECLARE @hashVarchar varchar(128) = ?; "
                                       "SELECT 1 "
                                       "FROM Accounts "
                                       "WHERE UserID = ? AND Userpass = HASHBYTES('SHA2_256',@hashVarchar);";
        QSqlQuery confirmPassword = getQuery(confirmPasswordQuery, QStringList{ui->PasswordLineEdit->text(),QString::number(userID)});
        confirmPassword.next();
        if(confirmPassword.value(0).toBool()){
            isAuthenticated=1;
            this->close();
        }
        else{
            QMessageBox::warning(this,"Wrong password","Invalid password. Please try again.");
            isAuthenticated=0;
            ui->PasswordLineEdit->setText("");
        }
    }
    else if(buttonBehavior==2){
        if(ui->PasswordLineEdit->text().isEmpty()){
            isAuthenticated=0;
            QMessageBox::warning(this,"Empty input","Please input a password and try again.");
        }
        else{
            isAuthenticated=1;
            this->close();
        }
    }
}

void PasswordConfirmation::on_Cancel_clicked()
{
    this->close();
}


void PasswordConfirmation::on_ConfirmPassword_clicked()
{

}

