#ifndef NEWTHREADSWINDOW_H
#define NEWTHREADSWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSqlQuery>
#include "global.h"
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>

namespace Ui {
class NewThreadsWindow;
}

class NewThreadsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NewThreadsWindow(QWidget *parent = nullptr);
    ~NewThreadsWindow();

private slots:
    void on_AddFileButton_clicked();

    void on_RemoveFileButton_clicked();

    void on_TagsRemoveButton_clicked();

    void on_TagsList_clicked(const QModelIndex &index);

    void on_RecipientsSearchButton_clicked();

    void on_RecipientsSearchList_clicked(const QModelIndex &index);

    void on_RecipientsRemoveButton_clicked();

    void on_CreateButton_clicked();

private:
    Ui::NewThreadsWindow *ui;
    QStringList fileAttachments;
    QStringList selectedTags;
    QStringList selectedRecipients;
    QStringList availableTags;
    QStandardItemModel *fileAttachmentsModel = new QStandardItemModel();
    QStandardItemModel *tagsModel = new QStandardItemModel();
    QStandardItemModel *recipientsModel = new QStandardItemModel();
    //QStringList additionalRecipients;

};

#endif // NEWTHREADSWINDOW_H
