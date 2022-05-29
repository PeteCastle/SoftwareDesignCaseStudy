#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QSqlQuery>
#include "azurestorage.h"

inline QSqlQuery getQuery(QString queryString, QStringList bindValues={}){ //Applicable only to queries without passing any value except of queryString
   QSqlQuery query;

   qDebug() << queryString;
   qDebug() << bindValues;

   query.setForwardOnly(true);
   query.prepare(queryString);
   if(!bindValues.isEmpty()){
       for(int i=0; i<bindValues.size();i++){
           query.bindValue(i,bindValues[i]);
       }
   }
   query.exec();
   return query;
}

inline QString generateAlphaNumericString(){
    QSqlQuery getString = getQuery("SELECT RESULT = CONVERT(varchar(255), NEWID())");
    getString.next();
    return getString.value(0).toString();
}


class StorageAccess : public QObject{
private:
    QString accountName = "inquirystorage";
    QString accountKey = "ST2xRKonWOBqUJyb2VkXH/ij7l9BuqmSb0foUHf9Pc41Z6vAevAuRuY+U8u3x6qsyEc1S43FqER4+AStFRX7HQ==";
    //QString container = "maincontainer";
    AzureStorage *azure = new AzureStorage(accountName,accountKey);
    //QString localFileToUpload = "D:/Educational Folders/Sophomore - Second Sem/Numerical Methods/CaycoFM_CPE0221-3_AssignmentNo.1.pdf";
    //QString azureFilenameForUpload = "F8A9137B-360A-4379-A120-5F4217F88087";
    //QString azureFilenameToDownload = "F8A9137B-360A-4379-A120-5F4217F88087";

public:
    void uploadFile(QString localFileToUpload, QString azureFilenameForUpload, QString container){
        QNetworkReply* uploadFileReply = azure->uploadFile(localFileToUpload, container, azureFilenameForUpload);
        if (uploadFileReply != nullptr){
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
    }

    void downloadFile(QString azureFilenameToDownload, QString container){
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
    }
};

#endif // GLOBAL_H
