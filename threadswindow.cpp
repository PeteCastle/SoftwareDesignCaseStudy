#include "threadswindow.h"
#include "ui_threadswindow.h"

ThreadsWindow::ThreadsWindow(QWidget *parent, AccountCredentials userCredentials, bool PublicThreadsOnly) :
    QWidget(parent),
    ui(new Ui::ThreadsWindow)
{
    ui->setupUi(this);

    this->UserCredentials = userCredentials;
    this->PublicThreadsOnly = PublicThreadsOnly;
    ui->ThreadTab->tabBar()->tabButton(0,QTabBar::RightSide)->deleteLater();
    ui->ThreadTab->tabBar()->setTabButton(0, QTabBar::RightSide, 0);

    QPushButton *refreshButton = new QPushButton("Reload");
    connect(refreshButton,&QPushButton::clicked,[this, refreshButton]{

        int index = ui->ThreadTab->currentIndex();
        if(index==0){
            return;
        }
        refreshButton->setText("Reloading...");
        QString threadID = ui->ThreadTab->tabToolTip(index);

        QWidget *widgetToDelete = ui->ThreadTab->widget(index);
        ui->ThreadTab->removeTab(index);
        widgetToDelete->deleteLater();

        ViewThread *viewThread = new ViewThread(this, threadID, {}, UserCredentials.UserID, UserCredentials);
        QString threadSubject = viewThread->getSubject();
        //ActiveThreadsList.insert(thread.ThreadID, viewThread);
        ui->ThreadTab->insertTab(index, viewThread, threadSubject + (threadSubject.size()>=30 ? "...":""));
        ui->ThreadTab->setTabToolTip(index,threadID);
        ui->ThreadTab->setCurrentWidget(viewThread);


        refreshButton->setText("Reload Tab");
    });

    if(PublicThreadsOnly){
        ui->YourThreadsLabel->setText("Public Threads");
    }

    ui->ThreadTab->setCornerWidget(refreshButton);
    updateThreadsList();
    //updateThreadsList();
    //updateThreadsList();

    connect(ui->RefreshThreadsButton,&QPushButton::clicked,[this]{updateThreadsList();});
}

ThreadsWindow::~ThreadsWindow()
{
    delete ui;
}

void ThreadsWindow::updateThreadsList(QString AdditionalArguments){
    ui->ThreadsList->clear();

    //For public threads
    if(PublicThreadsOnly){
        QString threadQueryStringWithConditions = " WHERE T.isVisible=1 ";
        if(AdditionalArguments!=""){
            AdditionalArguments = "%" + AdditionalArguments + "%";
            threadQueryStringWithConditions.append(" AND T.ThreadSubject LIKE ? ");
            threadsList = getThreadDetails(threadQueryStringWithConditions, QStringList{AdditionalArguments});
        }
        else{
            threadsList = getThreadDetails(threadQueryStringWithConditions,QStringList(QString::number(UserCredentials.UserID)));
        }
    }
    //For non-student and student accounts
    else if(UserCredentials.accountType==0 || UserCredentials.accountType==1){ //IMPORTNAT CHANGE THIS LATER
        //qDebug() << "TEST " << AdditionalArguments;
        QString threadQueryStringWithConditions = " WHERE T.ThreadUserID = ? AND T.isOpen=1";
        if(AdditionalArguments!=""){
            AdditionalArguments = "%" + AdditionalArguments + "%";
            threadQueryStringWithConditions.append(" AND T.ThreadSubject LIKE ?");
            threadsList = getThreadDetails(threadQueryStringWithConditions, QStringList{QString::number(UserCredentials.UserID),AdditionalArguments});
        }
        else{
            threadsList = getThreadDetails(threadQueryStringWithConditions,QStringList(QString::number(UserCredentials.UserID)));
        }

    }
    //The main difference is that for organizational accounts, the said account will also receive threads from tags selected by user
    else if(UserCredentials.accountType==2){
        QString threadQueryStringWithConditions = " WHERE (T.ThreadUserID = ? OR  ";  //AND T.isOpen=1"
        threadQueryStringWithConditions+= "T.ThreadTags LIKE '%" + UserCredentials.accountTags.join("%' OR T.ThreadTags LIKE '%");
        threadQueryStringWithConditions+= "%') AND T.isOpen=1 ";
        if(AdditionalArguments!=""){
            AdditionalArguments = "%" + AdditionalArguments + "%";
            threadQueryStringWithConditions.append(" AND T.ThreadSubject LIKE ?");
            threadsList = getThreadDetails(threadQueryStringWithConditions, QStringList{QString::number(UserCredentials.UserID),AdditionalArguments});
        }
        else{
            threadsList = getThreadDetails(threadQueryStringWithConditions,QStringList(QString::number(UserCredentials.UserID)));
        }

    }
    else if(threadsList.isEmpty()){
        QString message = AdditionalArguments=="" ? "Your inbox is empty.  Try creating a new inquiry!" : "No results found.  Please try narrowing your query";
        ThreadsListWidget *threadItem = new ThreadsListWidget(this,"","",{},message);
        QListWidgetItem *item = new QListWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled);
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
    QCoreApplication::processEvents();
    ThreadDetails thread = item->data(Qt::UserRole).value<ThreadDetails>();
    if(!ActiveThreadsList.contains(thread.ThreadID) && thread.ThreadSubject!=""){
        ViewThread *viewThread = new ViewThread(this, thread.ThreadID, thread, UserCredentials.UserID, UserCredentials);
        ActiveThreadsList.insert(thread.ThreadID, viewThread);
        ui->ThreadTab->addTab(viewThread,thread.ThreadSubject.left(30) + (thread.ThreadSubject.size()>=30 ? "...":""));
        ui->ThreadTab->setTabToolTip(ui->ThreadTab->count()-1,thread.ThreadID);
        ui->ThreadTab->setCurrentWidget(viewThread);
    }
    else{
    }
}


void ThreadsWindow::on_ThreadTab_tabCloseRequested(int index)
{
    if(index==0){
        return;
    }
    QWidget *tabItem = ui->ThreadTab->widget(index);
    QString tabTitle = ui->ThreadTab->tabToolTip(index);

    ActiveThreadsList.remove(tabTitle);

    ui->ThreadTab->removeTab(index);

    delete tabItem;
    tabItem = nullptr;
}


void ThreadsWindow::on_SearchButton_clicked()
{
    updateThreadsList(ui->SearchThreadsLineEdit->text());
}

