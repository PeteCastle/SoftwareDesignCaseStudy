#include "newthreadswindow.h"
#include "ui_newthreadswindow.h"
#include <QFileDialog>
#include <QStandardPaths>

#include <QFileIconProvider>

NewThreadsWindow::NewThreadsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewThreadsWindow)
{
    ui->setupUi(this);

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
    QMessageBox::information(this,"sdsf","di pa to gumagana hehe");
}

