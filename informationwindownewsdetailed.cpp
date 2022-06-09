#include "informationwindownewsdetailed.h"
#include "ui_informationwindownewsdetailed.h"

InformationWindowNewsDetailed::InformationWindowNewsDetailed(QWidget *parent, News news) :
    QWidget(parent),
    ui(new Ui::InformationWindowNewsDetailed)
{
    ui->setupUi(this);
    this->news=news;
    ui->NewsContents->setText(news.Contents);
    ui->NewsTitle->setText(news.Headline);

}

void InformationWindowNewsDetailed::showEvent(QShowEvent*){
    displayNewsThumbnail();

}

//Copy of displaynewsthumbnail on informationwindownews literally does the same thing
void InformationWindowNewsDetailed::displayNewsThumbnail(){

    QPixmap rawImage = QPixmap(":/Icons/ProgramIcons/PlaceholderArticle.jpg");
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(rawImage);
    scene->setSceneRect(rawImage.rect());
    scene->addItem(item);
    ui->NewsThumbnail->setScene(scene);
    ui->NewsThumbnail->fitInView(rawImage.rect(), Qt::KeepAspectRatioByExpanding);
    //loadedPictures.insert(-1,scene);

    QString thumbnailLocation = QStandardPaths::writableLocation((QStandardPaths::TempLocation)) + "/CaseStudy/newsthumbnails/" + news.ThumbnailFileName;
    if(QFile::exists(thumbnailLocation)){
        QPixmap rawImage = QPixmap(thumbnailLocation);
        QGraphicsScene* scene = new QGraphicsScene();
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(rawImage);
        scene->setSceneRect(rawImage.rect());
        scene->addItem(item);
        ui->NewsThumbnail->setScene(scene);
        ui->NewsThumbnail->fitInView(rawImage.rect(), Qt::KeepAspectRatioByExpanding);
        //loadedPictures.insert(NewsList[currentNewsIndex].newsID,scene);
    }
    else{
        QTimer *timer = new QTimer();
        timer->setInterval(2000);
        timer->start();
        StorageAccess storageAccess;
        storageAccess.downloadFile(news.ThumbnailFileName,"newsthumbnails");
        //ui->NewsThumbnail->setScene(loadedPictures[-1]);
        connect(timer, &QTimer::timeout,[this,thumbnailLocation,timer]{
            if(QFile::exists(thumbnailLocation)){
                displayNewsThumbnail();
                timer->stop();
            }
        });
    }
}

InformationWindowNewsDetailed::~InformationWindowNewsDetailed()
{
    delete ui;
}
