#ifndef VIEWMESSAGE_H
#define VIEWMESSAGE_H

#include <QWidget>
#include "global.h"

namespace Ui {
class ViewMessage;
}

class ViewMessage : public QWidget
{
    Q_OBJECT

public:
    explicit ViewMessage(QWidget *parent = nullptr, MessageDetails message={});
    ~ViewMessage();

private:
    Ui::ViewMessage *ui;
    QTimer *timer = new QTimer();
};

#endif // VIEWMESSAGE_H
