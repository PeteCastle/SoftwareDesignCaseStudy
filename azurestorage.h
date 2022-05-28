#ifndef AZURESTORAGE_H
#define AZURESTORAGE_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>

class AzureStorage
{

public:
    AzureStorage(const QString& accountName, const QString& accountKey, QObject* parent = nullptr);
    QNetworkReply* uploadFile(const QString& filePath, const QString& container, const QString& blobName, const QString& blobType = "BlockBlob");
    QNetworkReply* downloadFile(const QString& container, const QString& blobName);


private:
  QString m_version = "2009-09-19";
  QString m_accountName;
  QString m_accountKey;
  QNetworkAccessManager *m_manager;

  QString generateCurrentTimeUTC();
  QString generateHeader(const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, const QString&,
                         const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, const QString&);
  QString generateAutorizationHeader(const QString& httpVerb, const QString& container, const QString& blobName,
                                     const QString& currentDateTime, const long& contentLength,
                                     const QStringList additionnalCanonicalHeaders = QStringList(),
                                     const QStringList additionnalCanonicalRessources = QStringList());
  QString generateUrl(const QString& container, const QString& blobName = QString(), const QString& additionnalParameters = QString());


};

#endif // AZURESTORAGE_H
