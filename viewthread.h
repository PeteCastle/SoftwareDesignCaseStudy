#ifndef VIEWTHREAD_H
#define VIEWTHREAD_H

#include <QWidget>
#include "global.h"
#include "viewmessage.h"
#include "threaddetailsbasicinfo.h"
#include "threaddetailsparticipants.h"
#include <QFileIconProvider>
#include <QFileDialog>
#include <QStandardItem>

namespace Ui {
class ViewThread;
}

class ViewThread : public QWidget
{
    Q_OBJECT
    //ViewThread(ViewThread&&)=default;
    //ViewThread& operator=(ViewThread&&)=default;

public:
    ViewThread(QWidget *parent = nullptr,QString ThreadID="",ThreadDetails={}, int userID=0, AccountCredentials userCredentials={});
    ~ViewThread();
    QString getSubject();

private slots:
    void on_AddFileButton_clicked();
    void on_RemoveFileButton_clicked();
    void on_SendMessageButton_clicked();
    void updateMessages(QString ThreadID);
    void on_CloseThreadButton_clicked();

private:
    Ui::ViewThread *ui;
    QStringList fileAttachments;
    QStandardItemModel *fileAttachmentsModel = new QStandardItemModel();
    ThreadDetails Thread;
    int userID;
    QStringList ThreadParticipants;
    QMap<QString,ViewMessage*> SavedMessages; //Key - MessageID; Value - Widget of message
};



#endif // VIEWTHREAD_H
