#ifndef THREADDETAILSPARTICIPANTS_H
#define THREADDETAILSPARTICIPANTS_H

#include <QWidget>
#include "global.h"

namespace Ui {
class ThreadDetailsParticipants;
}

class ThreadDetailsParticipants : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadDetailsParticipants(QWidget *parent = nullptr, AccountCredentials ThreadParticipant={});
    ~ThreadDetailsParticipants();

private:
    Ui::ThreadDetailsParticipants *ui;
};

#endif // THREADDETAILSPARTICIPANTS_H
