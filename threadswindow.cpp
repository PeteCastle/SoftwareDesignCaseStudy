#include "threadswindow.h"
#include "ui_threadswindow.h"

ThreadsWindow::ThreadsWindow(QWidget *parent, AccountCredentials userCredentials) :
    QWidget(parent),
    ui(new Ui::ThreadsWindow)
{
    ui->setupUi(this);

    this->UserCredentials = userCredentials;
    updateThreadsList();
    //updateThreadsList();
    //updateThreadsList();

    connect(ui->RefreshThreadsButton,&QPushButton::clicked,[this]{updateThreadsList();});
}

ThreadsWindow::~ThreadsWindow()
{
    delete ui;
}

void ThreadsWindow::updateThreadsList(){
    ui->ThreadsList->clear();

    //For non-student and student accounts

    if(UserCredentials.accountType==0 || UserCredentials.accountType==2){ //IMPORTNAT CHANGE THIS LATER
        QString threadQueryStringWithConditions = "WHERE T.ThreadUserID = ? AND T.isOpen=1";
        threadsList = getThreadDetails(threadQueryStringWithConditions, QStringList(QString::number(UserCredentials.UserID)));
    }
    if(threadsList.isEmpty()){
        ThreadsListWidget *threadItem = new ThreadsListWidget(this,"","",{},"Your inbox is empty.  Try creating a new inquiry!");
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(100,100));
        ui->ThreadsList->addItem(item);
        ui->ThreadsList->setItemWidget(item,threadItem);
        return;
    }
    foreach(ThreadDetails thread,threadsList){

        ThreadsListWidget *threadItem = new ThreadsListWidget(this,thread.ProfilePicture,thread.ThreadSubject,thread.ThreadTags,thread.ThreadUser);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(100,100));
        item->setData(Qt::UserRole,QVariant::fromValue<ThreadDetails>(thread));
        ui->ThreadsList->addItem(item);
        ui->ThreadsList->setItemWidget(item,threadItem);
        /*connect(ui->RefreshThreadsButton,&QPushButton::clicked,[this,item]{
            ui->ThreadsList->setItemWidget(item, nullptr);
            ui->ThreadsList->removeItemWidget(item);
            delete ui->ThreadsList->takeItem(ui->ThreadsList->row(item));
        });*/
    }
    //ui->ThreadsList->r

}

void ThreadsWindow::on_RefreshThreadsButton_clicked()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(10000);
    timer->start();
    ui->RefreshThreadsButton->setDisabled(true);
    connect(timer, &QTimer::timeout,[this,timer]{
        ui->RefreshThreadsButton->setEnabled(true);
        timer->stop();
    });
}


void ThreadsWindow::on_ThreadsList_itemClicked(QListWidgetItem *item)
{
    ThreadDetails thread = item->data(Qt::UserRole).value<ThreadDetails>();
    if(!ActiveThreadsList.contains(thread.ThreadID)){
        ViewThread *viewThread = new ViewThread(this, thread.ThreadID, thread, UserCredentials.UserID);
        ActiveThreadsList.insert(thread.ThreadID, viewThread);
        ui->ThreadTab->addTab(viewThread,thread.ThreadSubject.left(15) + "... " "-" + thread.ThreadID);
        ui->ThreadTab->setCurrentWidget(viewThread);
    }
    else{
    }
}


void ThreadsWindow::on_ThreadTab_tabCloseRequested(int index)
{
    QWidget *tabItem = ui->ThreadTab->widget(index);



    QStringList tabTitle = ui->ThreadTab->tabText(index).split("-");
    qDebug() << ui->ThreadTab->tabText(0);
    qDebug() << ui->ThreadTab->tabText(1);
    qDebug() << ui->ThreadTab->tabText(2);

    ActiveThreadsList.remove(tabTitle[1]);

    ui->ThreadTab->removeTab(index);

    delete tabItem;
    tabItem = nullptr;
}

