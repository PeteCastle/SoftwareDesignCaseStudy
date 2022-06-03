#ifndef VIEWTHREAD_H
#define VIEWTHREAD_H

#include <QWidget>
#include "global.h"
#include "viewmessage.h"

namespace Ui {
class ViewThread;
}

class ViewThread : public QWidget
{
    Q_OBJECT
    //ViewThread(ViewThread&&)=default;
    //ViewThread& operator=(ViewThread&&)=default;

public:
    ViewThread(QWidget *parent = nullptr,QString ThreadID="");
    ~ViewThread();

private:
    Ui::ViewThread *ui;
};

#endif // VIEWTHREAD_H
