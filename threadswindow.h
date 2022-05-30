#ifndef THREADSWINDOW_H
#define THREADSWINDOW_H

#include <QWidget>
#include "global.h"

namespace Ui {
class ThreadsWindow;
}

class ThreadsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadsWindow(QWidget *parent = nullptr, AccountCredentials userCredentials={});
    ~ThreadsWindow();

private:
    Ui::ThreadsWindow *ui;
};

#endif // THREADSWINDOW_H
