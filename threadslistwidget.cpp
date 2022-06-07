#include "threadslistwidget.h"
#include "ui_threadslistwidget.h"

ThreadsListWidget::ThreadsListWidget(QWidget *parent, QString ProfilePicture, QString ThreadSubject, QStringList ThreadTags, QString ThreadCreator) :
    QWidget(parent),
    ui(new Ui::ThreadsListWidget)
{
    ui->setupUi(this);
    if(ThreadSubject.isEmpty()){
        ui->ProfilePictureLabel->setVisible(false);
        ui->ThreadSubjectLabel->setText(ThreadCreator);
        ui->ThreadSubjectLabel->setAlignment(Qt::AlignCenter);
        ui->ThreadTagsLabel->setVisible(false);
        ui->ThreadCreatorName->setVisible(false);
        return;
    }


    //qDebug() << ThreadSubject << ThreadTags << ThreadCreator;
    ThreadSubject.truncate(40);
    ui->ThreadSubjectLabel->setText(ThreadSubject);
    ui->ThreadTagsLabel->setText(ThreadTags.join(','));
    ui->ThreadCreatorName->setText("By: " + ThreadCreator);
    //Set default
    reshapeProfilePicture("Default",ui->ProfilePictureLabel,50);

    QTimer *timer = new QTimer();
    timer->setInterval(500);
    timer->start();
    QObject::connect(timer, &QTimer::timeout, [this, ProfilePicture, timer]{
        if(reshapeProfilePicture(ProfilePicture,ui->ProfilePictureLabel,50)){
            timer->stop();
        }
    });



}

ThreadsListWidget::~ThreadsListWidget()
{
    delete ui;
}


