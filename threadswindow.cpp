#include "threadswindow.h"
#include "ui_threadswindow.h"

ThreadsWindow::ThreadsWindow(QWidget *parent, AccountCredentials userCredentials) :
    QWidget(parent),
    ui(new Ui::ThreadsWindow)
{
    ui->setupUi(this);


}

ThreadsWindow::~ThreadsWindow()
{
    delete ui;
}
