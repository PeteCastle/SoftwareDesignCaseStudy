#include "viewthreadguest.h"
#include "ui_viewthreadguest.h"

ViewThreadGuest::ViewThreadGuest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewThreadGuest)
{
    ui->setupUi(this);
    this->layout();
    ui->ThreadLabel->setVisible(false);
    ui->PlaceholderLabel1->setText("Enter your Thread ID:");
}

ViewThreadGuest::~ViewThreadGuest()
{
    delete ui;
}

void ViewThreadGuest::on_ThreadIDButton_clicked()
{
    QString threadID = ui->ThreadIDLineEdit->text();
    QSqlQuery verifyThreadID = getQuery("SELECT 1 FROM Threads WHERE ThreadID = ?",QStringList(threadID));
    verifyThreadID.next();
    if(verifyThreadID.value(0).toBool()){
        ViewThread *viewThread = new ViewThread(nullptr,threadID,{},-1,{});

        ui->PlaceholderLabel1->setText("Current Thread:");
        ui->ThreadLabel->setVisible(true);
        ui->ThreadLabel->setText(viewThread->getSubject());
        this->layout()->removeWidget(ui->ThreadIDLineEdit);
        this->layout()->removeWidget(ui->ThreadIDButton);

        delete ui->Thread->layout()->takeAt(0);

        ui->Thread->layout()->addWidget(viewThread);
        ui->MainSubLayout->addWidget(ui->ThreadIDLineEdit);
        ui->MainSubLayout->addWidget(ui->ThreadIDButton);
        ui->MainSubLayout->addItem(new QSpacerItem(50,5,QSizePolicy::Expanding,QSizePolicy::Fixed));
    }
    else{
        QMessageBox::information(this, "Thread ID Error", "The specified Thread ID does not exist.  Please try again.");
    }



}

