#ifndef INFORMATIONWINDOWNEWS_H
#define INFORMATIONWINDOWNEWS_H

#include <QWidget>
#include "global.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>


namespace Ui {
class InformationWindowNews;
}

class InformationWindowNews : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindowNews(QWidget *parent = nullptr, QVector<News> NewsList={});
    ~InformationWindowNews();

private slots:
    void on_NextButton_clicked();

    void on_PreviousButton_clicked();

private:
    Ui::InformationWindowNews *ui;


    void showEvent(QShowEvent*);
    QVector<News> NewsList;
    void getNews();
    void displayNews();
    void displayNewsThumbnail(bool defaultThumbnail=0);
    QMap<int,QGraphicsScene*> loadedPictures;
    int currentNewsIndex=0;
};



class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags()) : QLabel(parent){};
    ~ClickableLabel(){};

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event){
        emit clicked();
    }

};






#endif // INFORMATIONWINDOWNEWS_H
