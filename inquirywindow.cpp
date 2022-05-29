#include "inquirywindow.h"
#include "ui_inquirywindow.h"
#include <QStringListModel>

InquiryWindow::InquiryWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InquiryWindow)
{
    ui->setupUi(this);

   QStringList test = {"Create Thread","View Threads"};
   QStringListModel *mainMenuList = new QStringListModel(test,this);

   ui->MainMenu->setModel(mainMenuList);

   ThreadsWindow *threadsWindow = new ThreadsWindow(this);
   NewThreadsWindow *newThreadsWindow = new NewThreadsWindow(this);

   ui->MainStackedWidget->addWidget(newThreadsWindow);
   ui->MainStackedWidget->addWidget(threadsWindow);
}

InquiryWindow::~InquiryWindow()
{
    delete ui;
}

void InquiryWindow::on_MainMenu_clicked(const QModelIndex &index)
{
    ui->MainStackedWidget->setCurrentIndex(index.row());
}

