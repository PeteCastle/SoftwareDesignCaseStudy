#ifndef INFORMATIONWINDOWNEWSDETAILED_H
#define INFORMATIONWINDOWNEWSDETAILED_H

#include <QWidget>
#include "global.h"
#include <QGraphicsPixmapItem>

namespace Ui {
class InformationWindowNewsDetailed;
}

class InformationWindowNewsDetailed : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindowNewsDetailed(QWidget *parent = nullptr, News news={});
    ~InformationWindowNewsDetailed();

private:
    Ui::InformationWindowNewsDetailed *ui;
    void displayNewsThumbnail();
    void showEvent(QShowEvent*);
    News news;
};

#endif // INFORMATIONWINDOWNEWSDETAILED_H
