#ifndef VIEWTHREADGUEST_H
#define VIEWTHREADGUEST_H

#include <QWidget>
#include "global.h"
#include "viewthread.h"

namespace Ui {
class ViewThreadGuest;
}

class ViewThreadGuest : public QWidget
{
    Q_OBJECT

public:
    explicit ViewThreadGuest(QWidget *parent = nullptr);
    ~ViewThreadGuest();

private slots:
    void on_ThreadIDButton_clicked();

private:
    Ui::ViewThreadGuest *ui;
};

#endif // VIEWTHREADGUEST_H
