#include "landingpage.h"
#include "ui_landingpage.h"
#include "global.h"

#include <QSqlQuery>
#include <QDebug>

#include "azurestorage.h"

LandingPage::LandingPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LandingPage)
{
    ui->setupUi(this);

    /*

    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT * FROM SAMPLEDATASHEET");
    query.exec();

    while(query.next()){
        qDebug() << query.value(4).toString();
    }

    QString accountName = "inquirystorage";
    QString accountKey = "ST2xRKonWOBqUJyb2VkXH/ij7l9BuqmSb0foUHf9Pc41Z6vAevAuRuY+U8u3x6qsyEc1S43FqER4+AStFRX7HQ==";
    QString container = "maincontainer";
    QString localFileToUpload = "D:/Educational Folders/Sophomore - Second Sem/Numerical Methods/CaycoFM_CPE0221-3_AssignmentNo.1.pdf";
    QString azureFilenameForUpload = "F8A9137B-360A-4379-A120-5F4217F88087";
    QString azureFilenameToDownload = "F8A9137B-360A-4379-A120-5F4217F88087";

    AzureStorage *azure = new AzureStorage(accountName,accountKey);

    QNetworkReply* uploadFileReply = azure->uploadFile(localFileToUpload, container, azureFilenameForUpload);
    //qDebug() << uploadFileReply->error();
    if (uploadFileReply != nullptr)
    {
      connect(uploadFileReply, &QNetworkReply::finished,
        [uploadFileReply, container, localFileToUpload, azureFilenameForUpload]()
        {
          QNetworkReply::NetworkError code = uploadFileReply->error();
          if (uploadFileReply->error() == QNetworkReply::NetworkError::NoError)
          {
            qDebug() << "File " + localFileToUpload + " uploaded with success into " + container + "/" + azureFilenameForUpload;
          }
          else
          {
            qWarning() << "Error while trying to upload file " + localFileToUpload + " in " + container + "/" + azureFilenameForUpload + " (error code " + QString::number(code) + ")";
          }

          uploadFileReply->deleteLater();
        }
      );
    }

    QNetworkReply* downloadFileReply = azure->downloadFile(container, azureFilenameToDownload);
    if (downloadFileReply != nullptr)
    {
      QObject::connect(downloadFileReply, &QNetworkReply::finished,
        [downloadFileReply, container, azureFilenameToDownload]() {
          QNetworkReply::NetworkError code = downloadFileReply->error();
          if (downloadFileReply->error() == QNetworkReply::NetworkError::NoError)
          {
            qDebug() << "File " + container + "/" + azureFilenameToDownload + " downloaded with success";
            //qDebug() << "File content : " << QString(downloadFileReply->readAll().data());

            QByteArray b = downloadFileReply->readAll();
            QFile file("test.pdf");
            if(!file.open(QIODevice::WriteOnly)){
                    qDebug() << "DI SIYA GUMANA";
            }
            QDataStream out(&file);
            out << b;
            file.flush();
            file.close();
          }
          else
          {
            qWarning() << "Error while trying to download file from " + container + "/" + azureFilenameToDownload + " (error code " + QString::number(code) + ")";
            qWarning() << "Error content : " << QString(downloadFileReply->readAll().data());
          }

          downloadFileReply->deleteLater();
      });
    }

    */

}

void LandingPage::testSlot1(){
     qDebug() << "GUMANA KA BA?";
}
LandingPage::~LandingPage()
{
    delete ui;
}




void LandingPage::on_SignUpButton_clicked()
{
    RegisterWindow *registerWindow = new RegisterWindow(this);
    registerWindow->exec();
}

