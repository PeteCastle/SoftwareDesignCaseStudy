#include "informationwindowhome.h"
#include "ui_informationwindowhome.h"

InformationWindowHome::InformationWindowHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWindowHome)
{
    ui->setupUi(this);

    NewsList = getNews();

    WindowsList["Home"] = [this]{
        if(!ActiveWindowsList.contains("Home")){
            InformationWindowNews *newsWindow = new InformationWindowNews(this,NewsList);
            ui->Contents->addWidget(newsWindow);
            ui->Contents->setCurrentWidget(newsWindow);
            ActiveWindowsList["Home"] = newsWindow;
        }
        else{
            ui->Contents->setCurrentWidget(ActiveWindowsList["Home"]);
        }
    };


    WindowsList["News"] = [this]{
        if(!ActiveWindowsList.contains("News")){
            InformationWindowNewsList *newsListWindow = new InformationWindowNewsList(this, NewsList);
            ui->Contents->addWidget(newsListWindow);
            ui->Contents->setCurrentWidget(newsListWindow);
            ActiveWindowsList["News"] = newsListWindow;
        }
        else{
            ui->Contents->setCurrentWidget(ActiveWindowsList["News"]);
        }
    };

    WindowsList["Home"]();

    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    timer->start();
    connect(timer,&QTimer::timeout,[this]{
        ui->Date->setText("Today is " + QDateTime::currentDateTime().toString("dddd, MMMM d yyyy h:mm:ss ap"));
    });

}



InformationWindowHome::~InformationWindowHome()
{
    delete ui;
}

void InformationWindowHome::on_HomeButton_clicked()
{
    WindowsList["Home"]();
    uncheckAllButtons();
    ui->HomeButton->setChecked(true);
    ui->DateWidget->setVisible(true);
}


void InformationWindowHome::on_NewsButton_clicked()
{
    WindowsList["News"]();
    uncheckAllButtons();
    ui->NewsButton->setChecked(true);
    ui->DateWidget->setVisible(false);
}


void InformationWindowHome::on_AboutUsButton_clicked()
{
    uncheckAllButtons();
    ui->AboutUsButton->setChecked(true);
    ui->DateWidget->setVisible(false);
}


void InformationWindowHome::on_GalleryButton_clicked()
{

    uncheckAllButtons();
    ui->GalleryButton->setChecked(true);
    ui->DateWidget->setVisible(false);
}

void InformationWindowHome::uncheckAllButtons(){
    ui->HomeButton->setChecked(false);
    ui->NewsButton->setChecked(false);
    ui->AboutUsButton->setChecked(false);
    ui->GalleryButton->setChecked(false);

}



