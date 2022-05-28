#include "azurestorage.h"

AzureStorage::AzureStorage(const QString& accountName, const QString& accountKey, QObject* parent):
m_accountName(accountName),
m_accountKey(accountKey)
{
    m_manager = new QNetworkAccessManager(nullptr);
}

QNetworkReply* AzureStorage::downloadFile(const QString& container, const QString& blobName)
{
  QString currentDateTime = generateCurrentTimeUTC();

  QString url = generateUrl(container, blobName);

  QString authorization = generateAutorizationHeader("GET", container, blobName, currentDateTime, 0);

  QNetworkRequest request;

  request.setUrl(QUrl(url));
  request.setRawHeader(QByteArray("Authorization"), QByteArray(authorization.toStdString().c_str()));
  request.setRawHeader(QByteArray("x-ms-date"), QByteArray(currentDateTime.toStdString().c_str()));
  request.setRawHeader(QByteArray("x-ms-version"), QByteArray(m_version.toStdString().c_str()));
  request.setRawHeader(QByteArray("Content-Length"), QByteArray("0"));

  return m_manager->get(request);
}

QNetworkReply* AzureStorage::uploadFile(const QString& filePath, const QString& container, const QString& blobName, const QString& blobType)
{
  QByteArray data;
  QFile file(filePath);

  if (file.open(QIODevice::ReadOnly))
  {
    data = file.readAll();
  }
  else
  {
    return nullptr;
  }

  QString currentDateTime = generateCurrentTimeUTC();
  int contentLength = data.size();

  QStringList additionalCanonicalHeaders;
  additionalCanonicalHeaders.append(QString("x-ms-blob-type:%1").arg(blobType));

  QString url = generateUrl(container, blobName);
  QString authorization = generateAutorizationHeader("PUT", container, blobName, currentDateTime, data.size(), additionalCanonicalHeaders, QStringList());

  qDebug() << "URL:" << url;

  QNetworkRequest request;

  request.setUrl(QUrl(url));
  request.setRawHeader(QByteArray("Authorization"),QByteArray(authorization.toStdString().c_str()));
  request.setRawHeader(QByteArray("x-ms-date"),QByteArray(currentDateTime.toStdString().c_str()));
  request.setRawHeader(QByteArray("x-ms-version"),QByteArray(m_version.toStdString().c_str()));
  request.setRawHeader(QByteArray("Content-Length"),QByteArray(QString::number(contentLength).toStdString().c_str()));
  request.setRawHeader(QByteArray("x-ms-blob-type"),QByteArray(blobType.toStdString().c_str()));

  return m_manager->put(request, data);
}

QString AzureStorage::generateCurrentTimeUTC()
{
  return QLocale(QLocale::English).toString(QDateTime::currentDateTimeUtc(), "ddd, dd MMM yyyy hh:mm:ss").append(" GMT");
}

QString AzureStorage::generateHeader(
    const QString& httpVerb, const QString& contentEncoding, const QString& contentLanguage, const QString& contentLength,
    const QString& contentMd5, const QString& contentType, const QString& date, const QString& ifModifiedSince,
    const QString& ifMatch, const QString& ifNoneMatch, const QString& ifUnmodifiedSince, const QString& range,
    const QString& canonicalizedHeaders, const QString& canonicalizedResource)
{
  QString result = "";

  result = result + httpVerb + "\n";
  result = result + contentEncoding + "\n";
  result = result + contentLanguage + "\n";
  result = result + contentLength + "\n";
  result = result + contentMd5 + "\n";
  result = result + contentType + "\n";
  result = result + date + "\n";
  result = result + ifModifiedSince + "\n";
  result = result + ifMatch + "\n";
  result = result + ifNoneMatch + "\n";
  result = result + ifUnmodifiedSince + "\n";
  result = result + range + "\n";
  result = result + canonicalizedHeaders + "\n";
  result = result + canonicalizedResource;

  return result;
}

QString AzureStorage::generateAutorizationHeader(const QString& httpVerb, const QString& container,
                                                         const QString& blobName, const QString& currentDateTime,
                                                         const long& contentLength, const QStringList additionnalCanonicalHeaders,
                                                         const QStringList additionnalCanonicalRessources)
{
  // Create canonicalized header
  QString canonicalizedHeaders;
  for (const QString& additionnalCanonicalHeader : additionnalCanonicalHeaders)
  {
    canonicalizedHeaders.append(additionnalCanonicalHeader+"\n");
  }
  canonicalizedHeaders.append(QString("x-ms-date:%1\nx-ms-version:%2").arg(currentDateTime, m_version));

  // Create canonicalized ressource
  QString canonicalizedResource;
  if (blobName.isEmpty())
  {
    canonicalizedResource = QString("/%1/%2").arg(m_accountName, container);
  }
  else
  {
    canonicalizedResource = QString("/%1/%2/%3").arg(m_accountName, container, blobName);
  }

  for (const QString& additionnalCanonicalRessource : additionnalCanonicalRessources)
  {
    canonicalizedResource.append("\n"+additionnalCanonicalRessource);
  }

  // Create signature
  QString signature = generateHeader(httpVerb, "", "", QString::number(contentLength), "", "", "", "",
                                     "", "", "", "", canonicalizedHeaders, canonicalizedResource);

  // Create authorization header
  QByteArray authorizationHeader = QMessageAuthenticationCode::hash(
        QByteArray(signature.toUtf8()),
        QByteArray(QByteArray::fromBase64(m_accountKey.toStdString().c_str())),
        QCryptographicHash::Sha256);
  authorizationHeader = authorizationHeader.toBase64();

  return QString("SharedKey %1:%2").arg(m_accountName, QString(authorizationHeader));
}

QString AzureStorage::generateUrl(const QString& container, const QString& blobName, const QString& additionnalParameters)
{

  QString blobEndPoint = QString("https://%1.blob.core.windows.net/").arg(m_accountName);
  QString url = blobEndPoint + container;
  if (!blobName.isEmpty())
  {
    url.append("/" + blobName);
  }

  if (!additionnalParameters.isEmpty())
  {
    url.append("?"+additionnalParameters);
  }

  return url;
}

