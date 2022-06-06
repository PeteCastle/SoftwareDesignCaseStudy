#include "threaddetailsattachments.h"
#include "ui_threaddetailsattachments.h"

ThreadDetailsAttachments::ThreadDetailsAttachments(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThreadDetailsAttachments)
{
    ui->setupUi(this);
}

ThreadDetailsAttachments::~ThreadDetailsAttachments()
{
    delete ui;
}
