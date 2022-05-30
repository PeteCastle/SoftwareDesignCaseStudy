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
   }
   else if(userCredentials.accountType==0){
       ui->AccountTypeLabel->setText("Non-student Account");
   }
   else if(userCredentials.accountType==1){
       ui->AccountTypeLabel->setText("Student Account");
   }
   else if(userCredentials.accountType==2){
       ui->AccountTypeLabel->setText("Organizational Account");
   }
   else if(userCredentials.accountType==3){
       ui->AccountTypeLabel->setText("Administrator");
   }
   reshapeProfilePicture(":/Icons/ProgramIcons/DefaultProfilePicture.jpg",ui->ProfilePictureLabel,80); // Default profile pic

   StorageAccess storageAccess;
   storageAccess.downloadFile(userCredentials.accountProfilePicture,"profilepictures");
   connect(timer, SIGNAL(timeout()),this,SLOT(updateProfilePicture()));
   timer->start(1000);

   QStringList test = {"Create Thread","View Threads"};
   QStringListModel *mainMenuList = new QStringListModel(test,this);

   ui->MainMenu->setModel(mainMenuList);

   ThreadsWindow *threadsWindow = new ThreadsWindow(this, userCredentials);
   NewThreadsWindow *newThreadsWindow = new NewThreadsWindow(this, userCredentials);

   ui->MainStackedWidget->addWidget(newThreadsWindow);
   ui->MainStackedWidget->addWidget(threadsWindow);
}

InquiryWindow::~InquiryWindow()
{
    delete ui;
}

void InquiryWindow::on_MainMenu_clicked(const QModelIndex &index)
{
    ui->MainStackedWidget->setCurrentIndex(index.row());
}

void InquiryWindow::updateProfilePicture(){
    QString profilePicture = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/CaseStudy/profilepictures/";
    if(QFile::exists(profilePicture + userCredentials.accountProfilePicture)){
        QFile file(profilePicture + userCredentials.accountProfilePicture);
        QSqlQuery getRealFile = getQuery("SELECT fileName FROM FileDictionary WHERE fileID = ?", QStringList(userCredentials.accountProfilePicture));
        getRealFile.next();
        QString realname = getRealFile.value(0).toString();
        file.rename(profilePicture+userCredentials.accountProfilePicture,profilePicture+realname);
        reshapeProfilePicture(profilePicture+realname,ui->ProfilePictureLabel,80);
        file.rename(profilePicture+realname,profilePicture+userCredentials.accountProfilePicture);
        timer->stop();
        disconnect(timer, SIGNAL(timeout()),this,SLOT(updateProfilePicture()));
    }
}

