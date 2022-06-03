#ifndef INQUIRYWINDOW_H
#define INQUIRYWINDOW_H

#include <QMainWindow>
#include "threadswindow.h"
#include "newthreadswindow.h"
#include "global.h"

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
    void on_MainMenu_clicked(const QModelIndex &index);
    //void updateProfilePicture();

    void on_LogoutButton_clicked();

private:
    Ui::InquiryWindow *ui;
    ThreadsWindow *threadsWindow;
    NewThreadsWindow *newThreadsWindow;
    AccountCredentials userCredentials;
    QTimer *timer = new QTimer(this);
    QStringList featureList;

};

#endif // INQUIRYWINDOW_H
