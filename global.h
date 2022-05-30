#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QSqlQuery>
#include <QMessageBox>
#include "azurestorage.h"
#include "qlabel.h"
#include <QPainter>
#include <QPainterPath>

inline QSqlQuery getQuery(QString queryString, QStringList bindValues={}){
   QSqlQuery query;
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

struct AccountCredentials{
    int UserID;
    QString Username;
    QByteArray Password;
    QString firstName;
    QString middleName;
    QString lastName;
    QString academicEmail;
    QString personalEmail;
    QString contactNumber;
    QString gender;
    QDateTime accountCreationTime;
    int accountType; //Updated: -1 - Guest , 0 - Non-student Account, 1 - Student Account, 2 - Faculty, 3 - Admin
    QString accountProfilePicture; //Stored in alphanumeric characters to be retrieved from cloud storage
    QStringList accountTags;
};

inline AccountCredentials getAccountCredentials(int UserID){
    QString getAccountQuery = "SELECT UserID, Username, 'Protected', "
                              "FirstName,MiddleName,LastName,AcademicEmail,"
                              "PersonalEmail,ContactNumber,Gender,CONVERT(varchar(max),AccountCreationTime,126),"
                              "AccountType,AccountProfilePicture,AccountTags FROM Accounts WHERE UserID = ?;";
    QSqlQuery getAccount = getQuery(getAccountQuery, QStringList(QString::number(UserID)));
    getAccount.next();

    AccountCredentials accountCredentials;
    accountCredentials.UserID = getAccount.value(0).toInt();
    accountCredentials.Username = getAccount.value(1).toString();
    accountCredentials.Password = getAccount.value(2).toByteArray(); //Not really recorded
    accountCredentials.firstName = getAccount.value(3).toString();
    accountCredentials.middleName = getAccount.value(4).toString();
    accountCredentials.lastName = getAccount.value(5).toString();
    accountCredentials.academicEmail = getAccount.value(6).toString();
    accountCredentials.personalEmail = getAccount.value(7).toString();
    accountCredentials.contactNumber = getAccount.value(8).toString();
    accountCredentials.gender = getAccount.value(9).toString();

    QStringList tempDates = getAccount.value(10).toString().split(QRegularExpression("[T+]"));
    if(tempDates.size()>1){
        QDate tempDate = QDate::fromString(tempDates[0],"yyyy-MM-dd");
        tempDates[1].truncate(12);
        QTime tempTime = QTime::fromString(tempDates[1],"hh:mm:ss.zzz");
        QDateTime dateTime = QDateTime(tempDate,tempTime,Qt::UTC);
        accountCredentials.accountCreationTime = dateTime;
    }

    accountCredentials.accountType = getAccount.value(11).toInt();
    accountCredentials.accountProfilePicture = getAccount.value(12).toString();

    QStringList tempTags = getAccount.value(13).toString().split(',');
    accountCredentials.accountTags = tempTags;

    return accountCredentials;
}

class StorageAccess : public QObject{
private:
    QString accountName = "inquirystorage";
    QString accountKey = "ST2xRKonWOBqUJyb2VkXH/ij7l9BuqmSb0foUHf9Pc41Z6vAevAuRuY+U8u3x6qsyEc1S43FqER4+AStFRX7HQ==";
    AzureStorage *azure = new AzureStorage(accountName,accountKey);

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
                QMessageBox::critical(nullptr,"An error has occured", "Error while trying to upload file " + localFileToUpload + " in " + container + "/" + azureFilenameForUpload + " (error code " + QString::number(code) + ")");
              }

              uploadFileReply->deleteLater();
            }
          );
        }
    }
signals:
    void downloadFile(QString azureFilenameToDownload, QString container){
        //Checks if file IN ALPHANUMERIC NAME is already in temp directory (%temp% folder).  Else, download the file
        QString localFolder = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/CaseStudy/" + container;
        QDir dir(localFolder);
        if(!dir.exists()){
            qDebug() << "gumana ito";
            dir.mkpath(localFolder);
        }
        QString localFile =  localFolder + "/" + azureFilenameToDownload;
        qDebug() <<  localFile;
        if(!QFile::exists(localFile)){
            qDebug() << "File " << localFile << " is not available.  Downloading the file.";
            QNetworkReply* downloadFileReply = azure->downloadFile(container, azureFilenameToDownload);
            if (downloadFileReply != nullptr){
              QObject::connect(downloadFileReply, &QNetworkReply::finished,[downloadFileReply, container, azureFilenameToDownload, localFile]() {
                  QNetworkReply::NetworkError code = downloadFileReply->error();
                  if (downloadFileReply->error() == QNetworkReply::NetworkError::NoError){
                        QByteArray b = downloadFileReply->readAll();

                        QFile file(localFile);
                        if(!file.open(QIODevice::WriteOnly)){
                            QMessageBox::critical(nullptr,"An error has occured", "Failed to open the file\n" + file.fileName());
                        }
                        file.write(b);
                        //QDataStream out(&file);
                        //out << b;
                        file.flush();
                        file.close();
                        qDebug() << "File " + container + "/" + azureFilenameToDownload + " downloaded with success";
                  }
                  else{
                        QString a = "Error while trying to download file from " + container + "/" + azureFilenameToDownload + " (error code " + QString::number(code) + ") \n";
                        QString b = "Error content : " + QString(downloadFileReply->readAll().data());
                        QMessageBox::critical(nullptr, "An error has occured", a + b);
                  }
                  downloadFileReply->deleteLater();
              });
            }
        }
        else{
            qDebug() << "File " << localFile << " already exists.";
        }
    }
};

inline QPixmap* reshapeProfilePicture(QString imageFilePath, QLabel *label, int labelSize){
    QPixmap source(imageFilePath);

    auto size = qMin(source.width(),source.height());

    QPixmap *target = new QPixmap(size,size);
    target->fill(Qt::transparent);

    QPainter *qp = new QPainter(target);

    qp->setRenderHints(qp->Antialiasing);

    auto path = QPainterPath();
    path.addEllipse(0,0,size,size);
    qp->setClipPath(path);

    auto sourceRect = QRect(0,0,size,size);
    sourceRect.moveCenter(source.rect().center());

    qp->drawPixmap(target->rect(),source,sourceRect);

    *target = target->scaled(labelSize,labelSize,Qt::KeepAspectRatio,Qt::FastTransformation);

    label->setPixmap(*target);
    label->setAlignment(Qt::AlignCenter);
    return target;

}

#endif // GLOBAL_H
