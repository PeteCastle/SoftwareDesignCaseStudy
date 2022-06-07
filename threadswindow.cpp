#include "threadswindow.h"
#include "ui_threadswindow.h"

ThreadsWindow::ThreadsWindow(QWidget *parent, AccountCredentials userCredentials) :
    QWidget(parent),
    ui(new Ui::ThreadsWindow)
{
    ui->setupUi(this);

    this->UserCredentials = userCredentials;
    ui->ThreadTab->tabBar()->tabButton(0,QTabBar::RightSide)->deleteLater();
    ui->ThreadTab->tabBar()->setTabButton(0, QTabBar::RightSide, 0);

    QPushButton *refreshButton = new QPushButton("Reload");
    connect(refreshButton,&QPushButton::clicked,[this, refreshButton]{
        refreshButton->setText("Reloading...");
        int index = ui->ThreadTab->currentIndex();
        QString threadID = ui->ThreadTab->tabToolTip(index);

        QWidget *widgetToDelete = ui->ThreadTab->widget(index);
        ui->ThreadTab->removeTab(index);
        widgetToDelete->deleteLater();

        ViewThread *viewThread = new ViewThread(this, threadID, {}, UserCredentials.UserID);
        QString threadSubject = viewThread->getSubject();
        //ActiveThreadsList.insert(thread.ThreadID, viewThread);
        ui->ThreadTab->insertTab(index, viewThread, threadSubject + (threadSubject.size()>=30 ? "...":""));
        ui->ThreadTab->setTabToolTip(index,threadID);
        ui->ThreadTab->setCurrentWidget(viewThread);


        refreshButton->setText("Reload");
    });

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

    //For non-student and student accounts

    if(UserCredentials.accountType==0 || UserCredentials.accountType==2){ //IMPORTNAT CHANGE THIS LATER
        qDebug() << "TEST " << AdditionalArguments;
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
    if(threadsList.isEmpty()){
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
        ViewThread *viewThread = new ViewThread(this, thread.ThreadID, thread, UserCredentials.UserID);
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

