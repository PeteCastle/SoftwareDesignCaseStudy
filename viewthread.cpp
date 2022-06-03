#include "viewthread.h"
#include "ui_viewthread.h"
#include <QSqlQuery>
#include <QScrollBar>

ViewThread::ViewThread(QWidget *parent, QString ThreadID) :
    QWidget(parent),
    ui(new Ui::ViewThread)
{
    ui->setupUi(this);

    ui->Messages->horizontalScrollBar()->setEnabled(false);
    QVector<MessageDetails> ThreadMessages = getMessagesFromThread(ThreadID);

    //QScrollArea *mainScrollArea = new QScrollArea(ui->Messages);
    //mainScrollArea->setBackgroundRole(QPalette::Window);
    //mainScrollArea->setBackgroundRole(QPalette::Window);
    //mainScrollArea->setFrameShadow(QFrame::Plain);
    //mainScrollArea->setFrameShape(QFrame::NoFrame);
    //mainScrollArea->setWidgetResizable(true);

    QWidget *subWidget = new QWidget(ui->Messages);
    subWidget->setObjectName("subWidget");
    subWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    subWidget->setLayout(new QVBoxLayout(subWidget));
    //mainScrollArea->setWidget(subWidget);



    foreach(MessageDetails message, ThreadMessages){
        ViewMessage *createMessage = new ViewMessage(this, message);
        ui->Messages->widget()->layout()->addWidget(createMessage);
    }
    //ui->Messages->setLayout(layout);
}

ViewThread::~ViewThread()
{
    delete ui;
}
