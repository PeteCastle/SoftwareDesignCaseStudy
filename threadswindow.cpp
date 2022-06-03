#include "threadswindow.h"
#include "ui_threadswindow.h"

ThreadsWindow::ThreadsWindow(QWidget *parent, AccountCredentials userCredentials) :
    QWidget(parent),
    ui(new Ui::ThreadsWindow)
{
    ui->setupUi(this);

    this->UserCredentials = userCredentials;
    updateThreadsList();
    updateThreadsList();
    updateThreadsList();

    connect(ui->RefreshThreadsButton,&QPushButton::clicked,[this]{updateThreadsList();});
}

ThreadsWindow::~ThreadsWindow()
{
    delete ui;
}

void ThreadsWindow::updateThreadsList(){
    //ui->ThreadsList->clear();

    //For non-student and student accounts

    if(UserCredentials.accountType==0 || UserCredentials.accountType==2){ //IMPORTNAT CHANGE THIS LATER
        QString threadQueryStringWithConditions = "WHERE T.ThreadUserID = ? AND T.isOpen=1";
        threadsList = getThreadDetails(threadQueryStringWithConditions, QStringList(QString::number(UserCredentials.UserID)));
    }
    foreach(ThreadDetails thread,threadsList){

        ThreadsListWidget *threadItem = new ThreadsListWidget(this,thread.ProfilePicture,thread.ThreadSubject,thread.ThreadTags,thread.ThreadUser);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(100,100));
        item->setData(Qt::UserRole,QVariant(thread.ThreadID));
        ui->ThreadsList->addItem(item);
        ui->ThreadsList->setItemWidget(item,threadItem);
        connect(ui->RefreshThreadsButton,&QPushButton::clicked,[this,item]{
            ui->ThreadsList->removeItemWidget(item);
            delete ui->ThreadsList->takeItem(ui->ThreadsList->row(item));
        });
    }
    //ui->ThreadsList->r
}

void ThreadsWindow::on_RefreshThreadsButton_clicked()
{
    //updateThreadsList();
}


void ThreadsWindow::on_ThreadsList_itemClicked(QListWidgetItem *item)
{
    QString threadID = item->data(Qt::UserRole).value<QString>();
    if(!ActiveThreadsList.contains(threadID)){

        ViewThread *viewThread = new ViewThread(this, threadID);
        ActiveThreadsList.insert(threadID, viewThread);
        ui->ThreadTab->addTab(viewThread,"Thread " + threadID);
    }
    else{
    }
}

