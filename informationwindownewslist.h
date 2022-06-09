#ifndef INFORMATIONWINDOWNEWSLIST_H
#define INFORMATIONWINDOWNEWSLIST_H

#include <QWidget>
#include "global.h"
#include <QStandardItem>
#include "informationwindownewsdetailed.h"

namespace Ui {
class InformationWindowNewsList;
}

class InformationWindowNewsList : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindowNewsList(QWidget *parent = nullptr, QVector<News> NewsList={});
    ~InformationWindowNewsList();

private slots:
    void on_NewsList_clicked(const QModelIndex &index);

private:
    Ui::InformationWindowNewsList *ui;
    QMap<int,QWidget*> ActiveNewsList;
};

#endif // INFORMATIONWINDOWNEWSLIST_H
