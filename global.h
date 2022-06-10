#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QSqlQuery>
#include <QMessageBox>
#include "azurestorage.h"
#include "qlabel.h"
#include <QPainter>
#include <QPainterPath>
#include <QMetaType>
#include <QSqlError>

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

   if(query.lastError().text().contains("connected party did not properly respond after a period of time")){
       QMessageBox::information(nullptr, "SQL Connection Error", "Please check your internet connection and try again.");
   }
   return query;
}

inline QString generateAlphaNumericString(){
    QSqlQuery getString = getQuery("SELECT RESULT = CONVERT(varchar(255), NEWID())");
    getString.next();
    return getString.value(0).toString();
}

inline QString generateAlphaNumericStringShort(){
    QSqlQuery getString = getQuery("SELECT RESULT = SUBSTRING(CONVERT(varchar(255), NEWID()),0,9)");
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
    QString department; //Latest entry to account
};

struct ThreadDetails{
    QString ThreadID;
    QDateTime ThreadCreationTime;
    int ThreadUserID; //Use Username instead
    QString ThreadUser;
    QString ThreadSubject;
    QStringList ThreadTags;
    QStringList ThreadAdditionalRecipients;
    bool isOpen;
    bool isVisible;
    QString ProfilePicture;
};
Q_DECLARE_METATYPE(ThreadDetails)

struct MessageDetails{
    //Base infos
    QString ThreadID;
    QString MessageID;
    QDateTime MessageCreationTime;
    QString MessageContents;
    QStringList MessageAttachments;
    QString FullName;
    //Added infos
    int MessageUserID;
    QString UserProfilePicture;
};

struct News{
    QString Headline;
    QString Subheadline;
    QString Contents;
    QString ThumbnailFileName;
    bool isActive;
    int newsID;
};
Q_DECLARE_METATYPE(News)

inline QVector<News> getNews(){
    QString getNewsQuery = "SELECT * FROM News;";
    QSqlQuery getNews = getQuery(getNewsQuery);
    QVector<News> NewsList;
    while(getNews.next()){
        News newsDetails;
        newsDetails.Headline = getNews.value(0).toString();
        newsDetails.Subheadline = getNews.value(1).toString();
        newsDetails.Contents = getNews.value(2).toString();
        newsDetails.ThumbnailFileName = getNews.value(3).toString();
        newsDetails.isActive = getNews.value(4).toBool();
        newsDetails.newsID = getNews.value(5).toInt();
        NewsList.append(newsDetails);
    }
    return NewsList;
}

inline QVector<MessageDetails> getMessagesFromThread(QString ThreadID){
    QString getMessagesQuery = "SELECT "
                                   "M.ThreadID,"
                                   "M.MessageID,"
                                   "CONVERT(varchar(max),M.MessageCreationTime,126) AS Date,"
                                   "M.MessageContents,"
                                   "M.MessageAttachments,"
                                   "M.MessageUserID, "
                                   "CONCAT(A.FirstName, ' ', A.LastName) AS FullName,"
                                   "A.AccountProfilePicture "
                               "FROM Messages AS M "
                               "LEFT JOIN Accounts AS A "
                               "ON M.MessageUserID = A.UserID "
                               "WHERE ThreadID = ? "
                               "ORDER BY M.MessageCreationTime;";
    QSqlQuery getMessages = getQuery(getMessagesQuery, QStringList(ThreadID));

    QVector<MessageDetails> ThreadMessages;
    while(getMessages.next()){
        MessageDetails message;
        message.ThreadID = getMessages.value(0).toString();
        message.MessageID = getMessages.value(1).toString();

        QStringList tempDates = getMessages.value(2).toString().split(QRegularExpression("[T+]"));
        if(tempDates.size()>1){
            QDate tempDate = QDate::fromString(tempDates[0],"yyyy-MM-dd");
            tempDates[1].truncate(12);
            QTime tempTime = QTime::fromString(tempDates[1],"hh:mm:ss.zzz");
            QDateTime dateTime = QDateTime(tempDate,tempTime,Qt::UTC);
            message.MessageCreationTime = dateTime;
        }

        message.MessageContents = getMessages.value(3).toString();
        message.MessageAttachments = getMessages.value(4).toString().split(',');
        message.MessageUserID = getMessages.value(5).toInt();
        message.FullName = getMessages.value(6).toString();
        message.UserProfilePicture = getMessages.value(7).toString();
        ThreadMessages.append(message);
    }
    return ThreadMessages;
}



inline QList<ThreadDetails> getThreadDetails(QString threadQueryStringWithConditions, QStringList threadQueryPlaceHolder){
    //Get thread details depending on account type
    QString threadQueryStringInitial="SELECT "
                                     "T.ThreadID,"
                                     "CONVERT(varchar(max),T.ThreadCreationTime,126),"
                                     "T.ThreadUserID,"
                                     "CONCAT(A.FirstName,' ',A.LastName) AS FullName,"
                                     "T.ThreadSubject,"
                                     "T.ThreadTags,"
                                     "T.ThreadAdditionalRecipients,"
                                     "T.isOpen,"
                                     "T.isVisible,"
                                     "A.AccountProfilePicture "
                                 "FROM Threads AS T "
                                 "LEFT JOIN Accounts AS A "
                                 "ON T.ThreadUserID = A.UserID ";
     threadQueryStringInitial.append(threadQueryStringWithConditions);
     QSqlQuery getThreadQuery = getQuery(threadQueryStringInitial,threadQueryPlaceHolder);

     QList<ThreadDetails> threadList;
     while(getThreadQuery.next()){
         ThreadDetails thread;

         thread.ThreadID = getThreadQuery.value(0).toString();
         QStringList tempDates = getThreadQuery.value(1).toString().split(QRegularExpression("[T+]"));
         if(tempDates.size()>1){
             QDate tempDate = QDate::fromString(tempDates[0],"yyyy-MM-dd");
             tempDates[1].truncate(12);
             QTime tempTime = QTime::fromString(tempDates[1],"hh:mm:ss.zzz");
             QDateTime dateTime = QDateTime(tempDate,tempTime,Qt::UTC);
             thread.ThreadCreationTime = dateTime;
         }
         thread.ThreadUserID = getThreadQuery.value(2).toInt();
         thread.ThreadUser = getThreadQuery.value(3).toString();
         thread.ThreadSubject = getThreadQuery.value(4).toString();
         thread.ThreadTags = getThreadQuery.value(5).toString().split(',');
         thread.ThreadAdditionalRecipients = getThreadQuery.value(6).toString().split(',');
         thread.isOpen = getThreadQuery.value(7).toBool();
         thread.isVisible = getThreadQuery.value(8).toBool();
         thread.ProfilePicture = getThreadQuery.value(9).toString();
         threadList.append(thread);
     }
    return threadList;
}

inline AccountCredentials account(QSqlQuery *getAccount){
    AccountCredentials accountCredentials;
    accountCredentials.UserID = getAccount->value(0).toInt();
    accountCredentials.Username = getAccount->value(1).toString();
    accountCredentials.Password = getAccount->value(2).toByteArray(); //Not really recorded
    accountCredentials.firstName = getAccount->value(3).toString();
    accountCredentials.middleName = getAccount->value(4).toString();
    accountCredentials.lastName = getAccount->value(5).toString();
    accountCredentials.academicEmail = getAccount->value(6).toString();
    accountCredentials.personalEmail = getAccount->value(7).toString();
    accountCredentials.contactNumber = getAccount->value(8).toString();
    accountCredentials.gender = getAccount->value(9).toString();


    QStringList tempDates = getAccount->value(10).toString().split(QRegularExpression("[T+]"));
    if(tempDates.size()>1){
        QDate tempDate = QDate::fromString(tempDates[0],"yyyy-MM-dd");
        tempDates[1].truncate(12);
        QTime tempTime = QTime::fromString(tempDates[1],"hh:mm:ss.zzz");
        QDateTime dateTime = QDateTime(tempDate,tempTime,Qt::UTC);
        accountCredentials.accountCreationTime = dateTime;
    }

    accountCredentials.accountType = getAccount->value(11).toInt();
    accountCredentials.accountProfilePicture = getAccount->value(12).toString();

    QStringList tempTags = getAccount->value(13).toString().split(',');
    accountCredentials.department = getAccount->value(14).toString();
    accountCredentials.accountTags = tempTags;

    return accountCredentials;
}

inline AccountCredentials getAccountCredentials(int UserID){
    QString getAccountQuery = "SELECT UserID, Username, 'Protected', "
                              "FirstName,MiddleName,LastName,AcademicEmail,"
                              "PersonalEmail,ContactNumber,Gender,CONVERT(varchar(max),AccountCreationTime,126),"
                              "AccountType,AccountProfilePicture,AccountTags,Department FROM Accounts WHERE UserID = ?;";
    QSqlQuery getAccount = getQuery(getAccountQuery, QStringList(QString::number(UserID)));
    getAccount.next();

    AccountCredentials accountCredentials = account(&getAccount);
    return accountCredentials;
}

inline QList<AccountCredentials> getMultipleAccountCredentials(QStringList UserID){
    QString getAccountQuery = "SELECT UserID, Username, 'Protected', "
                              "FirstName,MiddleName,LastName,AcademicEmail,"
                              "PersonalEmail,ContactNumber,Gender,CONVERT(varchar(max),AccountCreationTime,126),"
                              "AccountType,AccountProfilePicture,AccountTags,Department FROM Accounts WHERE UserID = " + UserID.join(" OR UserID = ") + ";";
    QSqlQuery getAccount = getQuery(getAccountQuery);
    QList<AccountCredentials> MultipleAccounts;
    while(getAccount.next()){
        AccountCredentials accountCredentials = account(&getAccount);
        MultipleAccounts.append(accountCredentials);
    }
    return MultipleAccounts;
}



//Maps below (hopefully) reduces load time among the client program and SQL queries
extern QMap<QString,QString> FileDictionary; //Key - Alphanumeric Code; Value - Real file name
extern QMap<QString,QPixmap> ProfilePictureDictionary; //Key - FileAlphaNumericCode_PictureSize; Value - Pixmap
extern QMap<QString,AccountCredentials> AccountDictionary; //Key - UserID; Value - AccountCredentials


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
    void deleteFile(QString azureFileNameToDelete, QString container){
        QNetworkReply* deleteFileReply = azure->deleteFile(container,azureFileNameToDelete);
        if(deleteFileReply!=nullptr){
            if(deleteFileReply->error()== QNetworkReply::NetworkError::NoError){
                qDebug()<< "File " + azureFileNameToDelete + " in container " + container + " has been deleted.";
            }
            else
            {
              QMessageBox::critical(nullptr,"An error has occured", "Error while trying to delete file " + azureFileNameToDelete + " in " + container + " (error code " + QString::number(deleteFileReply->error()) + ")");
            }
            deleteFileReply->deleteLater();
        }
    }
    void downloadFile(QString azureFilenameToDownload, QString container){
        //Checks if file IN ALPHANUMERIC NAME is already in temp directory (%temp% folder).  Else, download the file
        QString localFolder = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/CaseStudy/" + container;
        QDir dir(localFolder);
        if(!dir.exists()){
            dir.mkpath(localFolder);
        }
        QString localFile =  localFolder + "/" + azureFilenameToDownload;
        //qDebug() <<  localFile;
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

inline QPixmap* pictureTransformCircular(QString filePath, int labelSize){
    QPixmap source(filePath);
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

    return target;
}


inline bool reshapeProfilePicture(QString pictureFileAlphaNumeric, QLabel *label, int labelSize, bool isLocalFile=0){
    QString profilePictureDefaultPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/CaseStudy/profilepictures/";

    if(pictureFileAlphaNumeric=="" ){  //For default profile pictures
        if(!FileDictionary.contains("Default")){
            QPixmap *circularPicture = pictureTransformCircular(":/Icons/ProgramIcons/DefaultProfilePicture.jpg",labelSize);
            ProfilePictureDictionary.insert("Default",*circularPicture);
            if(label!=nullptr){
                label->setPixmap(ProfilePictureDictionary["Default"]);
            }
        }
        else if(label!=nullptr){
            label->setPixmap(ProfilePictureDictionary["Default"]);
        }
        return 1;
    }
    else if(ProfilePictureDictionary.contains(pictureFileAlphaNumeric + "_" + QString::number(labelSize))){
        label->setPixmap(ProfilePictureDictionary[pictureFileAlphaNumeric + "_" + QString::number(labelSize)]);
        return 1;
    }
    else if(QFile::exists(profilePictureDefaultPath + pictureFileAlphaNumeric)){
        qDebug() << "Creating new profilepicture of " << pictureFileAlphaNumeric << " with size " <<  labelSize;
        if(!FileDictionary.contains(pictureFileAlphaNumeric)){
            QSqlQuery getRealFile = getQuery("SELECT fileName FROM FileDictionary WHERE fileID = ?", QStringList(pictureFileAlphaNumeric));

            if(getRealFile.next()){
                FileDictionary.insert(pictureFileAlphaNumeric,getRealFile.value(0).toString());
            }
        }
        QFile file(profilePictureDefaultPath+pictureFileAlphaNumeric);
        QString realname = FileDictionary[pictureFileAlphaNumeric];
        file.rename(profilePictureDefaultPath+pictureFileAlphaNumeric,profilePictureDefaultPath+realname);

        QPixmap *circularPicture = pictureTransformCircular(profilePictureDefaultPath+realname,labelSize);

        file.rename(profilePictureDefaultPath+realname,profilePictureDefaultPath+pictureFileAlphaNumeric);
        ProfilePictureDictionary.insert(pictureFileAlphaNumeric + "_" + QString::number(labelSize), *circularPicture);
        if(label!=nullptr){
            label->setPixmap(ProfilePictureDictionary[pictureFileAlphaNumeric + "_" + QString::number(labelSize)]);
        }
        return 1;
    }
    else if(isLocalFile){ //Now re-added profile pictures for locally added files
        //QFile file(pictureFileAlphaNumeric);
        QPixmap *circularPicture = pictureTransformCircular(pictureFileAlphaNumeric,labelSize);

        if(label!=nullptr){
            label->setPixmap(*circularPicture);
        }
        return 1;
    }
    else{
        qDebug() << "Updating Profile Picture: None have satisfied the conditions";
        return 0;
    }
}

extern StorageAccess storageAccess;

#endif // GLOBAL_H
