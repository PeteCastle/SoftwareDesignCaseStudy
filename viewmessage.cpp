#include "viewmessage.h"
#include "qpushbutton.h"
#include "ui_viewmessage.h"
//#include <QFileStandardPaths>

ViewMessage::ViewMessage(QWidget *parent, MessageDetails message) :
    QWidget(parent),
    ui(new Ui::ViewMessage)
{
    ui->setupUi(this);
    ui->FullName->setText(message.FullName);
    ui->MessageContents->setText(message.MessageContents);
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

    ui->MessageAttachmentsWidget->setVisible(false);

    if(message.MessageAttachments[0]!=""){
        QString getRealFileQueryString = message.MessageAttachments.join("' OR fileID = '") + "'";
        qDebug() << getRealFileQueryString;
        QSqlQuery getRealFile = getQuery("SELECT fileName FROM FileDictionary WHERE fileID = '" + getRealFileQueryString + ";");
        foreach(QString attachment,message.MessageAttachments){
            getRealFile.next();
            if(!FileDictionary.contains(attachment)){
                FileDictionary.insert(attachment,getRealFile.value(0).toString());
            }
            ui->MessageAttachmentsWidget->setVisible(true);
            QWidget *attachmentWidget = new QWidget();

            QHBoxLayout *attachmentLayout = new QHBoxLayout();

            QLabel *attachmentLabel = new QLabel(FileDictionary[attachment].left(50));
            attachmentLayout->addWidget(attachmentLabel);
            QPushButton *attachmentDownloadButton = new QPushButton("Download");
            connect(attachmentDownloadButton,&QPushButton::clicked,[attachmentDownloadButton,attachment]{
                attachmentDownloadButton->setText("Downloading...");
                storageAccess.downloadFile(attachment,"fileattachments");
                QTimer *timer = new QTimer();
                timer->setInterval(3000);
                timer->start();
                connect(timer,&QTimer::timeout,[timer,attachmentDownloadButton,attachment]{
                    QString oldFileLocation = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/CaseStudy/fileattachments/" + attachment;
                    if(QFile::exists(oldFileLocation)){
                        QString newLocation = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+ "/" + FileDictionary[attachment];
                        qDebug() << "Copying file from " << oldFileLocation << " to " << newLocation;
                        QFile::rename(oldFileLocation,newLocation);
                        attachmentDownloadButton->setText("Downloaded!");
                        attachmentDownloadButton->setDisabled(true);
                        QMessageBox::information(nullptr,"File downloaded", "File " + FileDictionary[attachment] + " downloaded.  Please check your downloads folder.");
                        timer->stop();
                    }
                });

            });
            attachmentLayout->addWidget(attachmentDownloadButton);
            attachmentWidget->setLayout(attachmentLayout);
            attachmentLayout->setContentsMargins(0,0,0,0);
            attachmentLayout->setSpacing(0);
            attachmentLayout->setStretch(0,80);
            attachmentLayout->setStretch(1,20);
            ui->MessageAttachmentsWidget->layout()->addWidget(attachmentWidget);
        }
    }

    if(message.MessageUserID==0){ //User ID for guest
        ui->FullName->setText("Guest");
    }

}

ViewMessage::~ViewMessage()
{
    delete ui;
}
