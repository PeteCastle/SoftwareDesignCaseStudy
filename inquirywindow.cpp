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

   QStringList userAvailableWindows;
   switch(userCredentials.accountType){
        case -1: userAvailableWindows={"Home","CreateThread","ViewThreadsGuest","ViewThreadsPublic"}; break;
        case 0: userAvailableWindows={"Home","CreateThread","ViewThreadsNonGuest","ViewThreadsPublic","ViewAccount"}; break;
        case 1: userAvailableWindows={"Home","CreateThread","ViewThreadsNonGuest","ViewThreadsPublic","ViewAccount"}; break;
        case 2: userAvailableWindows={"Home","CreateThread","ViewThreadsNonGuest","ViewThreadsPublic","ViewAccount"}; break;
        case 3: userAvailableWindows={"Home","CreateThread","ViewThreadsNonGuest","ViewThreadsPublic","ViewAccount","AdminSQLManager"}; break;
   }
   menuList["CreateThread"] = qMakePair(
        [this]{
            QString menuName = "Create Thread";
            QString menuIconFile = ":/Icons/ProgramIcons/LoadingScreen.png";
            InquiryWindowMenu *newMenu = new InquiryWindowMenu(nullptr,menuName,menuIconFile);
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(QSize(100,100));
            item->setData(Qt::UserRole,QString("CreateThread"));
            ui->MainMenu->addItem(item);
            ui->MainMenu->setItemWidget(item, newMenu);
        },
        [this]{
            NewThreadsWindow *newThreadsWindow = new NewThreadsWindow(this, userCredentials);
            ui->MainStackedWidget->addWidget(newThreadsWindow);
            ui->MainStackedWidget->setCurrentWidget(newThreadsWindow);
            activeMenuList.insert("CreateThread", newThreadsWindow);
            menuDictionary.insert("CreateThread", "Create New Thread");
            qDebug() << "Created new window of Create Thread";
        });
   menuList["ViewThreadsNonGuest"] = qMakePair(
       [this]{
           QString menuName = "View My Threads";
           QString menuIconFile = ":/Icons/ProgramIcons/LoadingScreen.png";
           InquiryWindowMenu *newMenu = new InquiryWindowMenu(nullptr,menuName,menuIconFile);
           QListWidgetItem *item = new QListWidgetItem();
           item->setSizeHint(QSize(100,100));
           item->setData(Qt::UserRole,QString("ViewThreadsNonGuest"));
           ui->MainMenu->addItem(item);
           ui->MainMenu->setItemWidget(item, newMenu);
       },
       [this]{
           ThreadsWindow *threadsWindow = new ThreadsWindow(this, userCredentials);
           ui->MainStackedWidget->setCurrentWidget(threadsWindow);
           activeMenuList.insert("ViewThreadsNonGuest", threadsWindow);
           menuDictionary.insert("ViewThreadsNonGuest", "View My Threads");
           ui->MainStackedWidget->addWidget(threadsWindow);
           qDebug() << "Created new window of View Threads";
       });
   menuList["Home"] = qMakePair(
       [this]{
           QString menuName = "Home";
           QString menuIconFile = ":/Icons/ProgramIcons/LoadingScreen.png";
           InquiryWindowMenu *newMenu = new InquiryWindowMenu(nullptr,menuName,menuIconFile);
           QListWidgetItem *item = new QListWidgetItem();
           item->setSizeHint(QSize(100,100));
           item->setData(Qt::UserRole,QString("Home"));
           ui->MainMenu->addItem(item);
           ui->MainMenu->setItemWidget(item, newMenu);
       },
       [this]{
           InquiryWindowHome *homeWindow = new InquiryWindowHome(this);
           ui->MainStackedWidget->setCurrentWidget(homeWindow);
           activeMenuList.insert("Home", homeWindow);
           menuDictionary.insert("Home", "Home");
           ui->MainStackedWidget->addWidget(homeWindow);
           qDebug() << "Created new window of Home";
       });
   menuList["ViewAccount"] = qMakePair(
       [this]{
           QString menuName = "My Acccount";
           QString menuIconFile = ":/Icons/ProgramIcons/LoadingScreen.png";
           InquiryWindowMenu *newMenu = new InquiryWindowMenu(nullptr,menuName,menuIconFile);
           QListWidgetItem *item = new QListWidgetItem();
           item->setSizeHint(QSize(100,100));
           item->setData(Qt::UserRole,QString("ViewAccount"));
           ui->MainMenu->addItem(item);
           ui->MainMenu->setItemWidget(item, newMenu);
       },
       [this]{
           InquiryWindowAccount *accountWindow = new InquiryWindowAccount(nullptr, userCredentials);
           ui->MainStackedWidget->setCurrentWidget(accountWindow);
           activeMenuList.insert("ViewAccount", accountWindow);
           menuDictionary.insert("ViewAccount", "My Account");
           ui->MainStackedWidget->addWidget(accountWindow);
           qDebug() << "Created new window of Accounts";
       });

   menuList["ViewThreadsGuest"] = qMakePair(
        [this]{
            QString menuName = "Display Thread";
            QString menuIconFile = ":/Icons/ProgramIcons/LoadingScreen.png";
            InquiryWindowMenu *newMenu = new InquiryWindowMenu(nullptr,menuName,menuIconFile);
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(QSize(100,100));
            item->setData(Qt::UserRole,QString("ViewThreadsGuest"));
            ui->MainMenu->addItem(item);
            ui->MainMenu->setItemWidget(item, newMenu);
        },
        [this]{
            ViewThreadGuest *newThreadGuest = new ViewThreadGuest(nullptr);
            ui->MainStackedWidget->addWidget(newThreadGuest);
            ui->MainStackedWidget->setCurrentWidget(newThreadGuest);
            activeMenuList.insert("ViewThreadsGuest", newThreadGuest);
            menuDictionary.insert("ViewThreadsGuest", "View Thread");
            qDebug() << "Created new window of View Thread for Guest";
        });
    menuList["ViewThreadsPublic"] = qMakePair(
        [this]{
            QString menuName = "View Public Threads";
            QString menuIconFile = ":/Icons/ProgramIcons/LoadingScreen.png";
            InquiryWindowMenu *newMenu = new InquiryWindowMenu(nullptr,menuName,menuIconFile);
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(QSize(100,100));
            item->setData(Qt::UserRole,QString("ViewThreadsPublic"));
            ui->MainMenu->addItem(item);
            ui->MainMenu->setItemWidget(item, newMenu);
        },
        [this]{
       ThreadsWindow *newThreadPublic = new ThreadsWindow(nullptr,userCredentials,true);
            ui->MainStackedWidget->addWidget(newThreadPublic);
            ui->MainStackedWidget->setCurrentWidget(newThreadPublic);
            activeMenuList.insert("ViewThreadsPublic", newThreadPublic);
            menuDictionary.insert("ViewThreadsPublic", "View Public Threads");
            qDebug() << "Created new window of View Public Threads";
        });
   menuList["AdminSQLManager"] = qMakePair(
        [this]{
            QString menuName = "SQL Manager";
            QString menuIconFile = ":/Icons/ProgramIcons/LoadingScreen.png";
            InquiryWindowMenu *newMenu = new InquiryWindowMenu(nullptr,menuName,menuIconFile);
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(QSize(100,100));
            item->setData(Qt::UserRole,QString("AdminSQLManager"));
            ui->MainMenu->addItem(item);
            ui->MainMenu->setItemWidget(item, newMenu);
        },
        [this]{
       AdminSQLManager *SQLManager = new AdminSQLManager(nullptr);
            ui->MainStackedWidget->addWidget(SQLManager);
            ui->MainStackedWidget->setCurrentWidget(SQLManager);
            activeMenuList.insert("AdminSQLManager", SQLManager);
            menuDictionary.insert("AdminSQLManager", "SQL Manager");
            qDebug() << "Created new window of SQL Manager";
        });

   //Display relevant user values
   ui->FullNameLabel->setText(userCredentials.lastName.toUpper() + ", " + userCredentials.firstName + " " + userCredentials.middleName);
   ui->UsernameLabel->setText(userCredentials.Username);
   //Instantiate all menu
   foreach(QString menu, userAvailableWindows){
       qDebug() << menu;
       menuList[menu].first();
   }
  //Set usertype
   if(userCredentials.accountType==-1){ui->AccountTypeLabel->setText("Guest");}
   else if(userCredentials.accountType==0){ui->AccountTypeLabel->setText("Non-student Account");}
   else if(userCredentials.accountType==1){ui->AccountTypeLabel->setText("Student Account");}
   else if(userCredentials.accountType==2){ui->AccountTypeLabel->setText("Organizational Account");}
   else if(userCredentials.accountType==3){ui->AccountTypeLabel->setText("Administrator");}
   else{ui->AccountTypeLabel->setText("Unknown Account Type");}


   //Set profilepicture
   reshapeProfilePicture(":/Icons/ProgramIcons/DefaultProfilePicture.jpg",ui->ProfilePictureLabel,80,1); // Default profile pic

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

   if(userCredentials.accountType==-1){
       ui->FullNameLabel->setText("");
       ui->AccountTypeLabel->setText("");
       ui->UsernameLabel->setText("Guest Account");
   }


   //Set default menu
   menuList["Home"].second();
   ui->MainStackedWidget->setCurrentWidget(activeMenuList["Home"]);
   ui->MainMenu->setCurrentRow(0);
}

InquiryWindow::~InquiryWindow()
{
    delete ui;
}

void InquiryWindow::on_LogoutButton_clicked()
{
    this->close();
}


void InquiryWindow::on_MainMenu_itemClicked(QListWidgetItem *item)
{
    QString windowName = item->data(Qt::UserRole).value<QString>();
    if(!activeMenuList.contains(windowName)){
        menuList[windowName].second();
        ui->MainStackedWidget->setCurrentWidget(activeMenuList[windowName]);
        qDebug() << "User clicked a new menu";
    }
    else{
        qDebug() << "User clicked an already existing menu";
        ui->MainStackedWidget->setCurrentWidget(activeMenuList[windowName]);
    }
    if(windowName=="ViewAccount" || windowName == "AdminSQLManager") modifyHeaderVisibility(0);
    else modifyHeaderVisibility(1);


    ui->WindowTitleLabel->setText(menuDictionary[windowName]);
}

void InquiryWindow::modifyHeaderVisibility(bool isVisible){
    ui->WindowTitleLabel->setVisible(isVisible);
    ui->AccountTypeLabel->setVisible(isVisible);
    ui->FullNameLabel->setVisible(isVisible);
    ui->ProfilePictureLabel->setVisible(isVisible);
    ui->LogoutButton->setVisible(isVisible);
    ui->UsernameLabel->setVisible(isVisible);
}


/*
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
}*/
