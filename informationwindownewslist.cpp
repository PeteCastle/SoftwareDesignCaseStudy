#include "informationwindownewslist.h"
#include "ui_informationwindownewslist.h"

InformationWindowNewsList::InformationWindowNewsList(QWidget *parent, QVector<News> NewsList) :
    QWidget(parent),
    ui(new Ui::InformationWindowNewsList)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel();
    foreach(News news, NewsList){
        QStandardItem *item = new QStandardItem();
        item->setData(QString(news.Headline),Qt::DisplayRole);
        item->setData(QVariant::fromValue<News>(news),Qt::UserRole);
        //item->setData(Qt::UserRole,);
        //item->setData(int(news.newsID),Qt::UserRole);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignCenter);
        //item->setData()
        model->appendRow(item);
    }
    ui->NewsList->setModel(model);

}

InformationWindowNewsList::~InformationWindowNewsList()
{
    delete ui;
}

void InformationWindowNewsList::on_NewsList_clicked(const QModelIndex &index)
{
    News news = index.data(Qt::UserRole).value<News>();


    //ThreadDetails thread = item->data(Qt::UserRole).value<ThreadDetails>();
     //ui->TagsList->currentIndex().data(Qt::DisplayRole).toString();
     if(!ActiveNewsList.contains(news.newsID)){
         InformationWindowNewsDetailed *newNews = new InformationWindowNewsDetailed(this,news);
         ui->NewsWidget->addWidget(newNews);
         ui->NewsWidget->setCurrentWidget(newNews);
         ActiveNewsList[news.newsID] = newNews;
     }
     else{
         ui->NewsWidget->setCurrentWidget(ActiveNewsList[news.newsID]);
     }
}

