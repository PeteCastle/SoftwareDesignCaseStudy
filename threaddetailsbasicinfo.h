#ifndef THREADDETAILSBASICINFO_H
#define THREADDETAILSBASICINFO_H

#include <QWidget>
#include "global.h"

namespace Ui {
class ThreadDetailsBasicInfo;
}

class ThreadDetailsBasicInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadDetailsBasicInfo(QWidget *parent = nullptr,ThreadDetails thread={});
    ~ThreadDetailsBasicInfo();

private:
    Ui::ThreadDetailsBasicInfo *ui;
};

#endif // THREADDETAILSBASICINFO_H
