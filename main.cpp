#include "landingpage.h"

#include <QApplication>
#include <QtSql>
#include <QtDebug>
#include <QMessageBox>
#include <QUrl>
#include <QDesktopServices>
#include <QSplashScreen>
#include <QPixmap>

#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *loading = new QSplashScreen(QPixmap(":/new/prefix1/ProgramIcons/LoadingScreen.png"));
    loading->show();

    QString serverName = "tcp:petecastle.database.windows.net,1433";
    QString initialCatalog = "MainDataBase";
    QString userID = "PeteCastle";
    QString userPassword = "CEzt9%P7nsoZ#EyuK$";
    QString connectionTimout = "30";

    QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");
    database.setConnectOptions();

    /*
    database.setHostName(serverName);
    database.setDatabaseName(initialCatalog);
    database.setUserName(userID);
    database.setPassword(userPassword);*/

    QString databaseConfiguration = QString(
        "Driver={ODBC Driver 13 for SQL Server};"
        "Server=%1;"
        "Database=%2;"
        "Uid=%3;"
        "Pwd=%4;"
        "Encrypt=yes;"
        "TrustServerCertificate=no;"
        "Connection Timeout=%5;"
    ).arg(serverName).arg(initialCatalog).arg(userID).arg(userPassword).arg(connectionTimout);

    //qDebug() << databaseConfiguration;

    database.setConnectOptions();
    database.setDatabaseName(databaseConfiguration);

    /*
    qDebug() << "Host Name:\t" << database.hostName();
    qDebug() << "Database Name:\t" << database.databaseName();
    qDebug() << "User Name:\t" << database.userName();
    qDebug() << "Password:\t" << database.password();
    qDebug () << "ODBC driver valid?\t" << database.isValid ();*/

    if(database.open()){
        qDebug() << "Successful connection!";
    }
    else{
        qDebug() << "Error Connection: \n\t" << database.lastError().text();
        QMessageBox::StandardButton connectionError = QMessageBox::question(nullptr, "Database Connection Error",
                      "Running this program requires the driver 'ODBC Driver for SQL Server' to be installed on your system."
                      "\nClicking Yes will redirect you to the driver installation page.\n\n"
                      "An internet connection is also required.", QMessageBox::Yes|QMessageBox::No);
        if(connectionError == QMessageBox::Yes){
            QDesktopServices::openUrl(QUrl("https://www.microsoft.com/en-us/download/details.aspx?id=50420",QUrl::TolerantMode));
        }
        return 0;
    }


    LandingPage w;
    w.show();
    loading->finish(&w);
    return a.exec();
}

