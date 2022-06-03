#include "threadslistwidget.h"
#include "ui_threadslistwidget.h"

ThreadsListWidget::ThreadsListWidget(QWidget *parent, QString ProfilePicture, QString ThreadSubject, QStringList ThreadTags, QString ThreadCreator) :
    QWidget(parent),
    ui(new Ui::ThreadsListWidget)
{
    ui->setupUi(this);
    //qDebug() << ThreadSubject << ThreadTags << ThreadCreator;
    ThreadSubject.truncate(40);
    ui->ThreadSubjectLabel->setText(ThreadSubject);
    ui->ThreadTagsLabel->setText(ThreadTags.join(','));
    ui->ThreadCreatorName->setText("By: " + ThreadCreator);

    QTimer *timer = new QTimer();
    timer->setInterval(2000);
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


