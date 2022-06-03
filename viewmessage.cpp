#include "viewmessage.h"
#include "ui_viewmessage.h"

ViewMessage::ViewMessage(QWidget *parent, MessageDetails message) :
    QWidget(parent),
    ui(new Ui::ViewMessage)
{
    ui->setupUi(this);
    ui->FullName->setText(message.FullName);
    ui->MessageContents->setText(message.MessageContents);
    qDebug() << message.MessageContents;
    auto timeSinceSent = message.MessageCreationTime.secsTo(QDateTime::currentDateTimeUtc());

    if(message.FullName==""){
        ui->FullName->setText("Unknown");
    }

    if(timeSinceSent<60){
        ui->Time->setText(QString::number(timeSinceSent) + " secs ago");
    }
    else if(timeSinceSent<3600){
        ui->Time->setText(QString::number(floor(timeSinceSent/60)) + " mins ago");
    }
    else if(timeSinceSent<86400){
        ui->Time->setText(QString::number(floor(timeSinceSent/3600)) + " hours ago");
    }
    else{
        ui->Time->setText(message.MessageCreationTime.toString("MMMM d h:mm ap"));
    }

    ui->ProfilePicture->setPixmap(ProfilePictureDictionary["Default"]);
    storageAccess.downloadFile(message.UserProfilePicture,"profilepictures");
    QString profilePicture = message.UserProfilePicture;

    timer->start(2000);
    connect(timer, &QTimer::timeout, [this,profilePicture]{
        if(reshapeProfilePicture(profilePicture, ui->ProfilePicture,40)){
            timer->stop();
        }
    });
}

ViewMessage::~ViewMessage()
{
    delete ui;
}
