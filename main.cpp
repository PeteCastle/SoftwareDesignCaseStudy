#include "landingpage.h"

#include <QApplication>
#include <QtSql>
#include <QtDebug>
#include <QMessageBox>
#include <QUrl>

#include <QSplashScreen>
#include <QPixmap>

#include "global.h"

#include <QDesktopServices>

QMap<QString,QString> FileDictionary;
QMap<QString,QPixmap> ProfilePictureDictionary;
StorageAccess storageAccess;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *loading = new QSplashScreen(QPixmap(":/Icons/ProgramIcons/LoadingScreen.png"));
    loading->show();




    QString serverName = "tcp:petecastle.database.windows.net,1433";
    QString initialCatalog = "MainDataBase";
    QString userID = "PeteCastle";
    QString userPassword = "CEzt9%P7nsoZ#EyuK$";
    QString connectionTimout = "30";

    QStringList possibleDrivers = {"OBDC Driver 17 for SQL Server","OBDC Driver 13 for SQL Server","SQL Server Native Client 11.0","SQL Server"};

    for(int i=0; i<possibleDrivers.size();i++){
        loading->showMessage("Connecting to SQL Server (Attempt " + QString::number(i) + ")...",Qt::AlignLeft,QColor(229,175,0));
        QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");
        database.setConnectOptions();

        QString databaseConfiguration = QString(
            "Driver={%1};"
            "Server=%2;"
            "Database=%3;"
            "Uid=%4;"
            "Pwd=%5;"
            "Encrypt=yes;"
            "TrustServerCertificate=no;"
            "Connection Timeout=%6;"
            "MultipleActiveResultSets=True;"
        ).arg(possibleDrivers[i]).arg(serverName).arg(initialCatalog).arg(userID).arg(userPassword).arg(connectionTimout);
        database.setDatabaseName(databaseConfiguration);

        if(database.open()){
            qDebug() << "Successful connection!";
            break;
        }
        else if(i==possibleDrivers.size()-1){
            if(database.lastError().text().contains("Data source name not found and no default driver specified")){
                qDebug() << "Error Connection: \n\t" << database.lastError().text();
                QMessageBox::StandardButton connectionError = QMessageBox::question(nullptr, "Database Connection Error",
                              "Running this program requires the driver 'ODBC Driver for SQL Server' to be installed on your system."
                              "\nClicking Yes will redirect you to the driver installation page.\n\n"
                              "An internet connection is also required.", QMessageBox::Yes|QMessageBox::No);
                if(connectionError == QMessageBox::Yes){
                    QDesktopServices::openUrl(QUrl("https://www.microsoft.com/en-us/download/details.aspx?id=50420",QUrl::TolerantMode));
                }
            }
        }
    }


    loading->showMessage("Initializing program directory...",Qt::AlignLeft,QColor(229,175,0));
    QDir defaultProgramDirectory(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/CaseStudy");
    if(!defaultProgramDirectory.exists()){
        defaultProgramDirectory.mkpath(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/CaseStudy");
    }

    reshapeProfilePicture("",nullptr,50);



    loading->showMessage("Creating Main Window...",Qt::AlignLeft,QColor(229,175,0));
    LandingPage w;
    w.show();
    loading->finish(&w);
    return a.exec();
}

