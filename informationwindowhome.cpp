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
    WindowsList["AboutUs"] = [this]{
        if(!ActiveWindowsList.contains("AboutUs")){
            InformationWindowAboutUs *aboutUsWindow = new InformationWindowAboutUs(this);
            ui->Contents->addWidget(aboutUsWindow);
            ui->Contents->setCurrentWidget(aboutUsWindow);
            ActiveWindowsList["AboutUs"] = aboutUsWindow;
        }
        else{
            ui->Contents->setCurrentWidget(ActiveWindowsList["AboutUs"]);
        }
    };
    WindowsList["FrequentlyAskQuestions"] = [this]{
        if(!ActiveWindowsList.contains("FrequentlyAskQuestions")){
            InformationWindowFAQ *faqWindow = new InformationWindowFAQ(this);
            ui->Contents->addWidget(faqWindow);
            ui->Contents->setCurrentWidget(faqWindow);
            ActiveWindowsList["FrequentlyAskQuestions"] = faqWindow;
        }
        else{
            ui->Contents->setCurrentWidget(ActiveWindowsList["FrequentlyAskQuestions"]);
        }
    };
    WindowsList["ContactUs"] = [this]{
        if(!ActiveWindowsList.contains("ContactUs")){
            InformationWindowContactUs *contactUsWindow = new InformationWindowContactUs(this);
            ui->Contents->addWidget(contactUsWindow);
            ui->Contents->setCurrentWidget(contactUsWindow);
            ActiveWindowsList["ContactUs"] = contactUsWindow;
        }
        else{
            ui->Contents->setCurrentWidget(ActiveWindowsList["ContactUs"]);
        }
    };
    WindowsList["Gallery"] = [this]{
        if(!ActiveWindowsList.contains("Gallery")){
            InformationWindowGallery *galleryWindow = new InformationWindowGallery(this);
            ui->Contents->addWidget(galleryWindow);
            ui->Contents->setCurrentWidget(galleryWindow);
            ActiveWindowsList["Gallery"] = galleryWindow;
        }
        else{
            ui->Contents->setCurrentWidget(ActiveWindowsList["Gallery"]);
        }
    };


    WindowsList["Home"]();
    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    timer->start();
    connect(timer,&QTimer::timeout,[this]{
        ui->Date->setText("Today is " + QDateTime::currentDateTime().toString("dddd, MMMM d yyyy h:mm:ss ap"));
    });

    ui->HomeButton->setChecked(true);

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
    WindowsList["AboutUs"]();
    uncheckAllButtons();
    ui->AboutUsButton->setChecked(true);
    ui->DateWidget->setVisible(false);
}


void InformationWindowHome::on_GalleryButton_clicked()
{
    WindowsList["Gallery"]();
    uncheckAllButtons();
    ui->GalleryButton->setChecked(true);
    ui->DateWidget->setVisible(false);
}

void InformationWindowHome::uncheckAllButtons(){
    ui->HomeButton->setChecked(false);
    ui->NewsButton->setChecked(false);
    ui->AboutUsButton->setChecked(false);
    ui->GalleryButton->setChecked(false);
    ui->ContactUsbutton->setChecked(false);
    ui->FAQButton->setChecked(false);
    ui->GalleryButton->setChecked(false);
    ui->DateWidget->setVisible(false);
}

void InformationWindowHome::on_ContactUsbutton_clicked()
{
    WindowsList["ContactUs"]();
    uncheckAllButtons();
    ui->ContactUsbutton->setChecked(true);
    ui->DateWidget->setVisible(false);
}


void InformationWindowHome::on_FAQButton_clicked()
{
    WindowsList["FrequentlyAskQuestions"]();
    uncheckAllButtons();
    ui->FAQButton->setChecked(true);
    ui->DateWidget->setVisible(false);
}

