#ifndef INQUIRYWINDOW_H
#define INQUIRYWINDOW_H

#include <QMainWindow>
#include "newthreadswindow.h"
#include "global.h"
#include "inquirywindowmenu.h"
#include "threadswindow.h"
#include "inquirywindowhome.h"
#include "inquirywindowaccount.h"
#include "viewthreadguest.h"
#include "adminsqlmanager.h"
#include <QListWidgetItem>


namespace Ui {
class InquiryWindow;
}

class InquiryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InquiryWindow(QWidget *parent = nullptr, int userID=0);
    ~InquiryWindow();

private slots:
    void on_LogoutButton_clicked();
    void on_MainMenu_itemClicked(QListWidgetItem *item);
    void modifyHeaderVisibility(bool isVisible);

private:
    Ui::InquiryWindow *ui;
    ThreadsWindow *threadsWindow;
    NewThreadsWindow *newThreadsWindow;
    AccountCredentials userCredentials;
    QTimer *timer = new QTimer(this);

    QMap<QString, QString> menuDictionary;
    QMap<QString, QWidget*> activeMenuList;
    QMap<QString,QPair<std::function<void()>,std::function<void()>>> menuList;

};

#endif // INQUIRYWINDOW_H
