#include "threaddetailsparticipants.h"
#include "ui_threaddetailsparticipants.h"

ThreadDetailsParticipants::ThreadDetailsParticipants(QWidget *parent, AccountCredentials ThreadParticipant) :
    QWidget(parent),
    ui(new Ui::ThreadDetailsParticipants)
{
    ui->setupUi(this);

    ui->AccountName->setText(ThreadParticipant.firstName + " " + ThreadParticipant.lastName);
    if(ThreadParticipant.academicEmail!=""){
        ui->AccountEducationalEmail->setText(ThreadParticipant.academicEmail);
    }
    else{
        ui->AccountEducationalEmail->setVisible(false);
    }
    if(ThreadParticipant.department!=""){
        ui->AccountOrganization->setText(ThreadParticipant.department);
    }
    else{
        ui->AccountOrganization->setVisible(false);
    }
    if(ThreadParticipant.academicEmail!=""){
        ui->AccountEducationalEmail->setText(ThreadParticipant.academicEmail);
    }
    else{
        ui->AccountEducationalEmail->setVisible(false);
    }
    reshapeProfilePicture(ThreadParticipant.accountProfilePicture,ui->AccountProfilePicture,40);

    if(ThreadParticipant.UserID==0){ //User ID for guest
        ui->AccountName->setText("Guest");
    }

}

ThreadDetailsParticipants::~ThreadDetailsParticipants()
{
    delete ui;
}
