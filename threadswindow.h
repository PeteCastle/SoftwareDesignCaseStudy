#ifndef THREADSWINDOW_H
#define THREADSWINDOW_H

#include <QWidget>
#include "global.h"
#include "threadslistwidget.h"
#include <QListWidgetItem>
#include "viewthread.h"

namespace Ui {
class ThreadsWindow;
}

class ThreadsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadsWindow(QWidget *parent = nullptr, AccountCredentials userCredentials={});
    ~ThreadsWindow();

private slots:
    void on_RefreshThreadsButton_clicked();

    void on_ThreadsList_itemClicked(QListWidgetItem *item);

private:
    QMap<QString, ViewThread*> ActiveThreadsList; //Key: ThreadID; Value: Widget
    Ui::ThreadsWindow *ui;
    AccountCredentials UserCredentials;
    void updateThreadsList();
    QList<ThreadDetails> threadsList;
};

#endif // THREADSWINDOW_H
