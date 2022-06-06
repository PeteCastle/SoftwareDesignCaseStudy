#include "newthreadswindow.h"
#include "ui_newthreadswindow.h"
#include <QFileDialog>
#include <QStandardPaths>

#include <QFileIconProvider>
#include <QSqlError>
#include <QClipboard>

NewThreadsWindow::NewThreadsWindow(QWidget *parent, AccountCredentials userCredentials) :
    QWidget(parent),
    ui(new Ui::NewThreadsWindow)
{
    ui->setupUi(this);
    this->UserCredentials = userCredentials;

    if(userCredentials.accountType==-1 || userCredentials.accountType==0){
        ui->RecipientsAddLabel->setVisible(false);
        ui->RecipientsSelectedLabel->setVisible(false);
        ui->RecipientsSelectedList->setVisible(false);
        ui->RecipientsSearchList->setVisible(false);
        ui->RecipientsSearchLineEdit->setVisible(false);
        ui->RecipientsRemoveButton->setVisible(false);
        if(userCredentials.accountType==-1 ){
            ui->FileAttachmentsLabel->setVisible(false);
            ui->FileAttachmentsList->setVisible(false);
            ui->AddFileButton->setVisible(false);
            ui->RemoveFileButton->setVisible(false);
        }
    }

    ui->FileAttachmentsList->setVisible(false);
    ui->FileAttachmentsList->setModel(fileAttachmentsModel);
    ui->FileAttachmentsList->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QSqlQuery getTagsList = getQuery("SELECT * FROM ThreadTags");
    while(getTagsList.next()){
        availableTags.append(getTagsList.value(0).toString());
    }

    QStringListModel *threadTagModel = new QStringListModel(availableTags);
    QSortFilterProxyModel *threadTagFilterModel = new QSortFilterProxyModel();
    threadTagFilterModel->setSourceModel(threadTagModel);
    threadTagFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    threadTagFilterModel->setFilterKeyColumn(0);

    connect(ui->TagsSearch,&QLineEdit::textChanged,threadTagFilterModel,&QSortFilterProxyModel::setFilterFixedString);
    ui->TagsList->setModel(threadTagFilterModel);
    ui->TagsList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->TagsYourList->setModel(tagsModel);
    ui->TagsYourList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->FileAttachmentsList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->RecipientsSearchList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->RecipientsSelectedList->setModel(recipientsModel);
    ui->RecipientsSelectedList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

NewThreadsWindow::~NewThreadsWindow()
{
    delete ui;
}

void NewThreadsWindow::on_AddFileButton_clicked()
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
        ui->FileAttachmentsList->setVisible(true);
    }

}


void NewThreadsWindow::on_RemoveFileButton_clicked()
{
    int index = ui->FileAttachmentsList->currentIndex().row();
    if(index!=-1){
        fileAttachments.remove(index);
        ui->FileAttachmentsList->model()->removeRow(index);
    }
}




void NewThreadsWindow::on_TagsRemoveButton_clicked()
{
    int index = ui->TagsYourList->currentIndex().row();
    if(index!=-1){
        selectedTags.remove(index);
        ui->TagsYourList->model()->removeRow(index);
    }
}


void NewThreadsWindow::on_TagsList_clicked(const QModelIndex &index)
{
    QString selected = ui->TagsList->currentIndex().data(Qt::DisplayRole).toString();
    if(selectedTags.indexOf(selected)==-1){
        selectedTags.append(selected);
        tagsModel->appendRow(new QStandardItem(selected));
    }
}


void NewThreadsWindow::on_RecipientsSearchButton_clicked()
{
    QString nameToSearch = "%" + ui->RecipientsSearchLineEdit->text() + "%";
    if(nameToSearch==""){
        QMessageBox::information(this,"Empty input","Please enter any name and try again.");
    }

    QString nameQueryQString = "SELECT CONCAT(FirstName,' ',LastName)"
                                "FROM [dbo].[Accounts]"
                                "WHERE CONCAT(FirstName,' ',LastName) LIKE ?;";
    QSqlQuery getNames = getQuery(nameQueryQString, QStringList(nameToSearch));

    QStringList listNames;
    while(getNames.next()){
        listNames.append(getNames.value(0).toString());
    }
    if(listNames.isEmpty()){
        listNames.append("(No items found on the search query)");
    }
    ui->RecipientsSearchList->setModel(new QStringListModel(listNames));

}


void NewThreadsWindow::on_RecipientsSearchList_clicked(const QModelIndex &index)
{
    QString selected = ui->RecipientsSearchList->currentIndex().data(Qt::DisplayRole).toString();
    if(selected==""){
        return;
    }
    if(selectedRecipients.indexOf(selected)==-1 && selected!="(No items found on the search query)"){
        selectedRecipients.append(selected);
        recipientsModel->appendRow(new QStandardItem(selected));
    }

}


void NewThreadsWindow::on_RecipientsRemoveButton_clicked()
{
    int index = ui->RecipientsSelectedList->currentIndex().row();
    if(index!=-1){
        selectedRecipients.remove(index);
        ui->RecipientsSelectedList->model()->removeRow(index);
    }
}


void NewThreadsWindow::on_CreateButton_clicked()
{
    QString title = ui->TitleLineEdit->text();
    if(title==""){
        QMessageBox::information(this, "Empty fields", "Please include a title on your inquiry");
        return;
    }
    QString contents = ui->ContentsLineEdit->toHtml();
    if(ui->ContentsLineEdit->toPlainText().isEmpty()){
        QMessageBox::information(this, "Empty fields", "Please include a content on your inquiry");
        return;
    }
    if(selectedTags.isEmpty()){
        QMessageBox::information(this, "Empty fields", "You have to include a tag based from your inquiry.");
        return;
    }

    //1ST STEP: UPLOAD FILE
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

    //2ND STEP: UPLOAD THREAD
    QString threadID = generateAlphaNumericStringShort();
    QStringList threadUploadPlaceholder;
    threadUploadPlaceholder.append(threadID);
    threadUploadPlaceholder.append(QString::number(UserCredentials.UserID));
    threadUploadPlaceholder.append(title);
    threadUploadPlaceholder.append(selectedTags.join(','));
    threadUploadPlaceholder.append(selectedRecipients.join(','));
    if(ui->VisibilityPublicButton->isChecked()){
        threadUploadPlaceholder.append("1");
    }
    else if(ui->VisibilityPrivateButton->isChecked()){
        threadUploadPlaceholder.append("0");
    }
    else{
        QMessageBox::information(this,"Empty fields","Please select the visibility of your thread");
        return;
    }
    QString threadUploadQueryText = "INSERT INTO Threads(ThreadID,ThreadCreationTime,ThreadUserID,ThreadSubject,ThreadTags,"
                              "ThreadAdditionalRecipients,isOpen,isVisible)"
                              "VALUES(?,GETUTCDATE(),?,?,?,?,1,?);";
    getQuery(threadUploadQueryText,threadUploadPlaceholder);

    //3RD STEP: UPLOAD MESSAGE
    QString messageID = generateAlphaNumericString();
    QStringList messageUploadPlaceholder;
    messageUploadPlaceholder.append(threadID);
    messageUploadPlaceholder.append(messageID);
    messageUploadPlaceholder.append(contents);
    messageUploadPlaceholder.append(uniqueIdentifiers.join(','));
    messageUploadPlaceholder.append(QString::number(UserCredentials.UserID));
    QString messageUploadQueryText = "INSERT INTO Messages(ThreadID,MessageID,MessageCreationTime,MessageContents,MessageAttachments,MessageUserID)"
                                     "VALUES(?,?,GETUTCDATE(),?,?,?)";
    getQuery(messageUploadQueryText,messageUploadPlaceholder);

    if(UserCredentials.accountType==-1){
        QMessageBox::StandardButton askCopy = QMessageBox::question(nullptr, "Success!",
                              "As a guest, the only way you can retrieve your current (and future)"
                              "messages is by entering the Thread ID in View Messages tab.\n"
                              "You need to create an account to view your threads without entering Thread ID\n"
                              "Your thread ID:\t" + threadID + "\nWould you like to copy your Thread ID to the clipboard?", QMessageBox::Yes|QMessageBox::No);
        if(askCopy == QMessageBox::Yes){
           QClipboard *clipboard = QGuiApplication::clipboard();
           clipboard->setText(threadID);
        }
    }
    else{
        QMessageBox::information(nullptr, "Success!", "Successfully created the thread.  Navigate to View Messages Tab"
                                                      " to view replies and messages.");
    }

    //Clear all fields
    ui->TitleLineEdit->setText("");
    ui->ContentsLineEdit->setText("");
    ui->RecipientsSearchLineEdit->setText("");

    if(ui->TagsYourList->model()->rowCount()>0){
        ui->TagsYourList->model()->removeRows(0,ui->TagsYourList->model()->rowCount());
    }
    if(ui->RecipientsSelectedList->model()->rowCount()>0){
        ui->RecipientsSelectedList->model()->removeRows(0,ui->RecipientsSelectedList->model()->rowCount());
    }
    if(ui->FileAttachmentsList->model()->rowCount()>0){
        ui->FileAttachmentsList->model()->removeRows(0,ui->FileAttachmentsList->model()->rowCount());
    }
    ui->FileAttachmentsList->setVisible(false);
    fileAttachments.clear();
    selectedTags.clear();
    selectedRecipients.clear();
    availableTags.clear();
}

/*
void NewThreadsWindow::on_BoldButton_clicked()
{

   QTextCharFormat format;
   format.setFontWeight(QFont::Bold);

   QTextCursor cursor = ui->ContentsLineEdit->textCursor();
   qDebug() << cursor;
   cursor.mergeCharFormat(format);

   if(ui->BoldButton->isChecked()){
       QTextCursor cursor = ui->ContentsLineEdit->textCursor();
       if(cursor.hasSelection()){
           QFont boldFont(ui->ContentsLineEdit->font());
           boldFont.setBold(true);
           QTextCharFormat format;
           format.setFont(boldFont);
           cursor.mergeCharFormat(format);
       }
       else{
           QFont boldFont(ui->ContentsLineEdit->font());
           QTextCharFormat format;
           boldFont.setBold(true);
           format.setFont(boldFont);
           cursor.beginEditBlock();
           cursor.setBlockCharFormat(format);
           cursor.endEditBlock();
           //ui->ContentsLineEdit->setTextCursor(cursor);
       }

   }
   else{
       QTextCursor cursor = ui->ContentsLineEdit->textCursor();
       QFont boldFont(ui->ContentsLineEdit->font());
       boldFont.setBold(false);
       QTextCharFormat format;
       format.setFont(boldFont);
       cursor.mergeCharFormat(format);
   }

}*/

