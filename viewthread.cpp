    #include "viewthread.h"
#include "ui_viewthread.h"
#include <QSqlQuery>
#include <QScrollBar>
#include <QSqlError>
#include <QShortcut>
#include <QKeySequence>

ViewThread::ViewThread(QWidget *parent, QString ThreadID,ThreadDetails thread, int userID, AccountCredentials userCredentials) :
    QWidget(parent),
    ui(new Ui::ViewThread)
{
    ui->setupUi(this);

    if(thread.ThreadSubject==""){
        QList<ThreadDetails> threadTemp = getThreadDetails(" WHERE ThreadID = ? ", QStringList(ThreadID));
        thread = threadTemp[0];
    }

    this->Thread = thread;
    this->userID = userID;
    ui->RemoveFileButton->setVisible(false);
    ui->AttachmentsList->setVisible(false);
    ui->AttachmentsList->setModel(fileAttachmentsModel);

    ui->Messages->horizontalScrollBar()->setEnabled(false);

    emit updateMessages(ThreadID);

    QWidget *subWidget = new QWidget(ui->Messages);
    subWidget->setObjectName("subWidget");
    subWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    subWidget->setLayout(new QVBoxLayout(subWidget));
    //mainScrollArea->setWidget(subWidget);

    ThreadDetailsBasicInfo *basicInfo = new ThreadDetailsBasicInfo(this,thread);
    ui->ThreadDetails->horizontalScrollBar()->setEnabled(false);
    ui->ThreadDetails->widget()->layout()->addWidget(basicInfo);

    QLabel *accountsLabel = new QLabel("Participants in this thread:");
    accountsLabel->setAlignment(Qt::AlignCenter);

    ui->ThreadDetails->widget()->layout()->addWidget(accountsLabel);

    QList<AccountCredentials> accountsList = getMultipleAccountCredentials(ThreadParticipants);
    foreach(AccountCredentials accounts,accountsList){

        ThreadDetailsParticipants *threadParticipants = new ThreadDetailsParticipants(this, accounts);
        ui->ThreadDetails->widget()->layout()->addWidget(threadParticipants);
    }

    if(thread.isOpen==0){
        ui->Message->setVisible(false);
        ui->AttachmentsList->setVisible(false);
        ui->AddFileButton->setVisible(false);
        ui->RemoveFileButton->setVisible(false);
        ui->Message->setVisible(false);
        ui->SendMessageButton->setVisible(false);
        ui->FileAttachmentsLabel->setVisible(false);
        ui->CloseThreadButton->setVisible(false);
    }

    if(thread.ThreadUserID==-1){
        ui->FileAttachmentsLabel->setVisible(false);
        ui->AddFileButton->setVisible(false);
    }

    if((userCredentials.accountType==-1 || userCredentials.accountType ==0 || userCredentials.accountType==1) && thread.isVisible==1){
        ui->CloseThreadButton->setVisible(false);
    }

    new QShortcut(QKeySequence(Qt::Key_F5),this,[this,ThreadID]{emit updateMessages(ThreadID);});

}

ViewThread::~ViewThread()
{
    delete ui;
}

void ViewThread::on_AddFileButton_clicked()
{


    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Image"),QStandardPaths::writableLocation((QStandardPaths::DownloadLocation)),tr("All Files (*)"));
    QFile file(filePath);
    if(file.size()>5000000){
        QMessageBox::information(this,"File too large","Due to storage limitations, file uploads are only limited up to 5MB.  Sorry!");
        return;
    }
    if (!filePath.isEmpty()){
        fileAttachments.append(filePath);

        QFile file(filePath);
        QFileInfo fileInfo(file);
        QFileIconProvider iconProvider;
        QStandardItem *item = new QStandardItem(iconProvider.icon(fileInfo), fileInfo.fileName());
        fileAttachmentsModel->appendRow(item);
        ui->RemoveFileButton->setVisible(true);
        ui->AttachmentsList->setVisible(true);

        //ui->FileAttachmentsList->setVisible(true);
    }
}


void ViewThread::on_RemoveFileButton_clicked()
{
    int index = ui->AttachmentsList->currentIndex().row();
    if(index!=-1){
        fileAttachments.remove(index);
        ui->AttachmentsList->model()->removeRow(index);
    }
    if(fileAttachments.isEmpty()){
        ui->AttachmentsList->setVisible(false);
    }
}


void ViewThread::on_SendMessageButton_clicked()
{
    if(ui->Message->toPlainText().isEmpty()){
        QMessageBox::information(this,"Empty input", "Please enter any text before sending a message.");
        return;
    }
    QString contents = ui->Message->toHtml();

    QStringList insertMessagePlaceholder;
    insertMessagePlaceholder.append(Thread.ThreadID);
    insertMessagePlaceholder.append(generateAlphaNumericString());
    insertMessagePlaceholder.append(ui->Message->toHtml());

    QStringList uniqueIdentifiers;
    foreach(QString file,fileAttachments){
        QString uniqueIdentifier = generateAlphaNumericString();
        //Upload to storage
        storageAccess.uploadFile(file, uniqueIdentifier, "fileattachments");
        uniqueIdentifiers.append(uniqueIdentifier);

        QStringList filePlaceHolder;
        filePlaceHolder.append(uniqueIdentifier);
        filePlaceHolder.append("fileattachments");
        QFile fileFile(file);
        QFileInfo fileInfo(fileFile);
        filePlaceHolder.append(fileInfo.fileName());
        QString fileUploadQueryText = "INSERT INTO FileDictionary(fileID,fileContainer,fileName) "
                                       "VALUES(?,?,?);";
        QSqlQuery fileUploadQuery = getQuery(fileUploadQueryText,filePlaceHolder);
        if(fileUploadQuery.lastError().text()!=""){
            QMessageBox::warning(this,"Registration Error","An error has occured.  Please send the following details to the developer.\n\n" + fileUploadQuery.lastError().text());
            return;
        }
    }
    insertMessagePlaceholder.append(uniqueIdentifiers.join(","));
    insertMessagePlaceholder.append(QString::number(userID));

    QString insertMessageQuery = "INSERT INTO Messages(ThreadID, MessageID, MessageCreationTime, MessageContents, MessageAttachments, MessageUserID) "
                                 "VALUES(?,?,GETUTCDATE(),?,?,?);";
    QSqlQuery insertMessage = getQuery(insertMessageQuery,insertMessagePlaceholder);

    emit updateMessages(Thread.ThreadID);

    ui->Message->setText("");

    if(insertMessage.lastError().text()!=""){
        qDebug() << QMessageBox::critical(this,"An error has occured", "Please try again later.");
        return;
    }
    else{
        QMessageBox::information(this,"Reply success!", "Please reload the threads to see the updated message.");
        return;
    }


}


void ViewThread::on_pushButton_clicked()
{
    QMessageBox::StandardButton closeThread = QMessageBox::question(nullptr, "Close Thread?",
                          "Are you sure you want to close the thread?", QMessageBox::Yes|QMessageBox::No);
    if(closeThread == QMessageBox::Yes){
        QString updateTableQuery = "UPDATE Threads SET isOpen=0 WHERE ThreadID = ?";
        QSqlQuery updateTable = getQuery(updateTableQuery, QStringList(Thread.ThreadID));
        ui->Message->setVisible(false);
        ui->AttachmentsList->setVisible(false);
        ui->AddFileButton->setVisible(false);
        ui->RemoveFileButton->setVisible(false);
        ui->Message->setVisible(false);
        ui->SendMessageButton->setVisible(false);
        ui->FileAttachmentsLabel->setVisible(false);
        ui->CloseThreadButton->setVisible(false);
    }
}

void ViewThread::updateMessages(QString ThreadID){
    qDebug() << "Reloading messages...";
    //ui->Messages->widget()->layout()->cle
    QVector<MessageDetails> ThreadMessages = getMessagesFromThread(ThreadID);
    foreach(MessageDetails message, ThreadMessages){
        if(!SavedMessages.contains(message.MessageID)){
            ViewMessage *createMessage = new ViewMessage(this, message);
            createMessage->setContentsMargins(0,0,0,0);
            ui->Messages->widget()->layout()->addWidget(createMessage);
            SavedMessages[message.MessageID] = createMessage;
        }
        else{
            ui->Messages->widget()->layout()->addWidget(SavedMessages[message.MessageID]);
        }

        qDebug() << (message.MessageUserID);

        if(!ThreadParticipants.contains(QString::number(message.MessageUserID))){

            ThreadParticipants.append(QString::number(message.MessageUserID));
        }
    }
}

QString ViewThread::getSubject(){
    return Thread.ThreadSubject;
}

