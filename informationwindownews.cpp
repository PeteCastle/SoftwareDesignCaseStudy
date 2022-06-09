#include "informationwindownews.h"
#include "ui_informationwindownews.h"



InformationWindowNews::InformationWindowNews(QWidget *parent, QVector<News> NewsList) :
    QWidget(parent),
    ui(new Ui::InformationWindowNews)
{
    ui->setupUi(this);

    this->NewsList = NewsList;

    //rawImage = rawImage.scaled(ui->HeadlineBackground->size(),Qt::KeepAspectRatio);

    /*
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap("C:/Users/Francis Mark Cayco/Downloads/leni.jpg"));
    scene->setSceneRect(rawImage.rect());
    scene->addItem(item);
    ui->HeadlineBackground->setScene(scene);*/


    connect(ui->Headline,&ClickableLabel::clicked, [this]{qDebug() << "TEST";});
}

InformationWindowNews::~InformationWindowNews()
{
    delete ui;
}

void InformationWindowNews::displayNews(){
    ui->Headline->setText(NewsList[currentNewsIndex].Headline);
    ui->Subheadline->setText(NewsList[currentNewsIndex].Subheadline);
    displayNewsThumbnail(0);
}

void InformationWindowNews::displayNewsThumbnail(bool defaultThumbnail){
    if(defaultThumbnail){
            QPixmap rawImage = QPixmap(":/Icons/ProgramIcons/PlaceholderArticle.jpg");
            QGraphicsScene* scene = new QGraphicsScene();
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(rawImage);
            scene->setSceneRect(rawImage.rect());
            scene->addItem(item);
            ui->HeadlineBackground->setScene(scene);
            ui->HeadlineBackground->fitInView(rawImage.rect(), Qt::KeepAspectRatioByExpanding);
            loadedPictures.insert(-1,scene);
    }
    else if(!loadedPictures.contains(NewsList[currentNewsIndex].newsID)){
        QString thumbnailLocation = QStandardPaths::writableLocation((QStandardPaths::TempLocation)) + "/CaseStudy/newsthumbnails/" + NewsList[currentNewsIndex].ThumbnailFileName;
        if(QFile::exists(thumbnailLocation)){
            QPixmap rawImage = QPixmap(thumbnailLocation);
            QGraphicsScene* scene = new QGraphicsScene();
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(rawImage);
            scene->setSceneRect(rawImage.rect());
            scene->addItem(item);
            ui->HeadlineBackground->setScene(scene);
            ui->HeadlineBackground->fitInView(rawImage.rect(), Qt::KeepAspectRatioByExpanding);
            loadedPictures.insert(NewsList[currentNewsIndex].newsID,scene);
        }
        else{
            QTimer *timer = new QTimer();
            timer->setInterval(2000);
            timer->start();
            StorageAccess storageAccess;
            storageAccess.downloadFile(NewsList[currentNewsIndex].ThumbnailFileName,"newsthumbnails");
            ui->HeadlineBackground->setScene(loadedPictures[-1]);
            connect(timer, &QTimer::timeout,[this,thumbnailLocation,timer]{
                if(QFile::exists(thumbnailLocation)){
                    displayNewsThumbnail();
                    timer->stop();
                }
            });
        }
    }
    else{
        ui->HeadlineBackground->setScene(loadedPictures[NewsList[currentNewsIndex].newsID]);
        ui->HeadlineBackground->fitInView(loadedPictures[NewsList[currentNewsIndex].newsID]->sceneRect(),Qt::KeepAspectRatioByExpanding);
    }
}
void InformationWindowNews::showEvent(QShowEvent*){
    displayNewsThumbnail(1);
    displayNews();
}

void InformationWindowNews::on_NextButton_clicked()
{
    if(currentNewsIndex==NewsList.size()-1){
        currentNewsIndex = 0;
    }
    else{
       currentNewsIndex +=1;
    }
    displayNews();
}


void InformationWindowNews::on_PreviousButton_clicked()
{
    if(currentNewsIndex==0){
        currentNewsIndex = NewsList.size()-1;
    }
    else{
       currentNewsIndex -=1;
    }
    displayNews();
}

