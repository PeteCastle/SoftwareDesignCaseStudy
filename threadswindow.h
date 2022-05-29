#ifndef THREADSWINDOW_H
#define THREADSWINDOW_H

#include <QWidget>

namespace Ui {
class ThreadsWindow;
}

class ThreadsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadsWindow(QWidget *parent = nullptr);
    ~ThreadsWindow();

private:
    Ui::ThreadsWindow *ui;
};

#endif // THREADSWINDOW_H
