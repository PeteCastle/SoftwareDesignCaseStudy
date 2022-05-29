#ifndef INQUIRYWINDOW_H
#define INQUIRYWINDOW_H

#include <QMainWindow>
#include "threadswindow.h"
#include "newthreadswindow.h"

namespace Ui {
class InquiryWindow;
}

class InquiryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InquiryWindow(QWidget *parent = nullptr);
    ~InquiryWindow();

private slots:
    void on_MainMenu_clicked(const QModelIndex &index);

private:
    Ui::InquiryWindow *ui;
    ThreadsWindow *threadsWindow;
    NewThreadsWindow *newThreadsWindow;

};

#endif // INQUIRYWINDOW_H
