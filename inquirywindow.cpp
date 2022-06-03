#include "inquirywindow.h"
#include "ui_inquirywindow.h"
#include <QStringListModel>
#include "global.h"

InquiryWindow::InquiryWindow(QWidget *parent, int userID) :
    QMainWindow(parent),
    ui(new Ui::InquiryWindow)
{
    ui->setupUi(this);

   userCredentials = getAccountCredentials(userID);

   //Display relevant user values
   ui->FullNameLabel->setText(userCredentials.lastName.toUpper() + ", " + userCredentials.firstName + " " + userCredentials.middleName);
   ui->UsernameLabel->setText(userCredentials.Username);
   if(userCredentials.accountType==-1){
       ui->AccountTypeLabel->setText("Guest");
       ui->FullNameLabel->setVisible(false);
       ui->UsernameLabel->setVisible(false);
       ui->AccountButton->setVisible(false);
       ui->LogoutButton->setText("Logout");
       //List of all available features for guest account
       featureList = {"Create Thread","View Threads"};
       QStringListModel *mainMenuList = new QStringListModel(featureList,this);
       ui->MainMenu->setModel(mainMenuList);
       ThreadsWindow *threadsWindow = new ThreadsWindow(this, userCredentials);
       NewThreadsWindow *newThreadsWindow = new NewThreadsWindow(this, userCredentials);
       ui->MainStackedWidget->addWidget(newThreadsWindow);
       ui->MainStackedWidget->addWidget(threadsWindow);
       ui->WindowTitleLabel->setText(featureList[0]);

   }
   else if(userCredentials.accountType==0){
       ui->AccountTypeLabel->setText("Non-student Account");
       //List of all available fetures for non-student account
       featureList = {"Create Thread","View Threads"};
       QStringListModel *mainMenuList = new QStringListModel(featureList,this);
       ui->MainMenu->setModel(mainMenuList);
       ThreadsWindow *threadsWindow = new ThreadsWindow(this, userCredentials);
       NewThreadsWindow *newThreadsWindow = new NewThreadsWindow(this, userCredentials);
       ui->MainStackedWidget->addWidget(newThreadsWindow);
       ui->MainStackedWidget->addWidget(threadsWindow);
       ui->WindowTitleLabel->setText(featureList[0]);
   }
   else if(userCredentials.accountType==1){
       ui->AccountTypeLabel->setText("Student Account");
       //List of all available features for student account
       featureList = {"Create Thread","View Threads"};
       QStringListModel *mainMenuList = new QStringListModel(featureList,this);
       ui->MainMenu->setModel(mainMenuList);
       ThreadsWindow *threadsWindow = new ThreadsWindow(this, userCredentials);
       NewThreadsWindow *newThreadsWindow = new NewThreadsWindow(this, userCredentials);
       ui->MainStackedWidget->addWidget(newThreadsWindow);
       ui->MainStackedWidget->addWidget(threadsWindow);
       ui->WindowTitleLabel->setText(featureList[0]);
   }
   else if(userCredentials.accountType==2){
       ui->AccountTypeLabel->setText("Organizational Account");
       //List of all available features for organizational account
       featureList = {"Create Thread","View Threads"};
       QStringListModel *mainMenuList = new QStringListModel(featureList,this);
       ui->MainMenu->setModel(mainMenuList);
       ThreadsWindow *threadsWindow = new ThreadsWindow(this, userCredentials);
       NewThreadsWindow *newThreadsWindow = new NewThreadsWindow(this, userCredentials);
       ui->MainStackedWidget->addWidget(newThreadsWindow);
       ui->MainStackedWidget->addWidget(threadsWindow);
       ui->WindowTitleLabel->setText(featureList[0]);
   }
   else if(userCredentials.accountType==3){
       ui->AccountTypeLabel->setText("Administrator");
       //List of all available features for administrator
       featureList = {"Create Thread","View Threads"};
       QStringListModel *mainMenuList = new QStringListModel(featureList,this);
       ui->MainMenu->setModel(mainMenuList);
       ThreadsWindow *threadsWindow = new ThreadsWindow(this, userCredentials);
       NewThreadsWindow *newThreadsWindow = new NewThreadsWindow(this, userCredentials);
       ui->MainStackedWidget->addWidget(newThreadsWindow);
       ui->MainStackedWidget->addWidget(threadsWindow);
       ui->WindowTitleLabel->setText(featureList[0]);
   }
   reshapeProfilePicture(":/Icons/ProgramIcons/DefaultProfilePicture.jpg",ui->ProfilePictureLabel,80); // Default profile pic

   if(userCredentials.accountType!=-1 || userCredentials.accountProfilePicture!=""){
       storageAccess.downloadFile(userCredentials.accountProfilePicture,"profilepictures");
       QString profilePicture = userCredentials.accountProfilePicture;
       connect(timer, &QTimer::timeout, [this,profilePicture]{
           if(reshapeProfilePicture(profilePicture, ui->ProfilePictureLabel,80)){
               timer->stop();
           }
       });
       timer->start(1000);
   }


}

InquiryWindow::~InquiryWindow()
{
    delete ui;
}

void InquiryWindow::on_MainMenu_clicked(const QModelIndex &index)
{
    ui->MainStackedWidget->setCurrentIndex(index.row());
    ui->WindowTitleLabel->setText(featureList[index.row()]);
}

/*
void InquiryWindow::updateProfilePicture(){
    QString profilePicture = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/CaseStudy/profilepictures/";
    if(QFile::exists(profilePicture + userCredentials.accountProfilePicture)){
        QFile file(profilePicture + userCredentials.accountProfilePicture);
        if(!FileDictionary.contains(userCredentials.accountProfilePicture)){
            QSqlQuery getRealFile = getQuery("SELECT fileName FROM FileDictionary WHERE fileID = ?", QStringList(userCredentials.accountProfilePicture));
            getRealFile.next();
            FileDictionary.insert(userCredentials.accountProfilePicture,getRealFile.value(0).toString());
            qDebug() << "GUMANA PA ITO";
        }
        QString realname = FileDictionary[userCredentials.accountProfilePicture];
        file.rename(profilePicture+userCredentials.accountProfilePicture,profilePicture+realname);
        reshapeProfilePicture(profilePicture+realname,ui->ProfilePictureLabel,80);
        file.rename(profilePicture+realname,profilePicture+userCredentials.accountProfilePicture);
        timer->stop();
        disconnect(timer, SIGNAL(timeout()),this,SLOT(updateProfilePicture()));
    }
}*/




void InquiryWindow::on_LogoutButton_clicked()
{
    this->close();
}

