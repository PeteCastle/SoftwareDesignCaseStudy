#ifndef THREADSLISTWIDGET_H
#define THREADSLISTWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class ThreadsListWidget;
}

class ThreadsListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadsListWidget(QWidget *parent = nullptr, QString ProfilePicture="", QString ThreadSubject="", QStringList ThreadTags={}, QString ThreadCreator="");
    ~ThreadsListWidget();

private:
    Ui::ThreadsListWidget *ui;
    //void updateProfilePicture(QString profilePictureName, QTimer *timer);
};


#endif // THREADSLISTWIDGET_H
