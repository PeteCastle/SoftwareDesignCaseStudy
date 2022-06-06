#include "threaddetailsbasicinfo.h"
#include "ui_threaddetailsbasicinfo.h"

ThreadDetailsBasicInfo::ThreadDetailsBasicInfo(QWidget *parent, ThreadDetails thread) :
    QWidget(parent),
    ui(new Ui::ThreadDetailsBasicInfo)
{
    ui->setupUi(this);
    ui->ThreadID->setText(thread.ThreadID);
    ui->ThreadSubject->setText(thread.ThreadSubject);
    ui->ThreadUserLabel->setText(thread.ThreadUser);
    ui->ThreadCreationDateLabel->setText(thread.ThreadCreationTime.toString("MMMM d h:mm ap"));

    foreach(QString tags, thread.ThreadTags){
        QLabel *tagLabel = new QLabel(tags);
        tagLabel->setAlignment(Qt::AlignCenter);
        ui->ThreadTags->layout()->addWidget(tagLabel);
    }

}

ThreadDetailsBasicInfo::~ThreadDetailsBasicInfo()
{
    delete ui;
}
