#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>
#include "registerwindow.h"
#include "inquirywindow.h"
#include "informationwindowhome.h"


QT_BEGIN_NAMESPACE
namespace Ui { class LandingPage; }
QT_END_NAMESPACE

class LandingPage : public QMainWindow
{
    Q_OBJECT

public:
    LandingPage(QWidget *parent = nullptr);
    ~LandingPage();

private:
    Ui::LandingPage *ui;
    RegisterWindow *registerWindow;
    InquiryWindow *inquiryWindow;
    InformationWindowHome *informationWidget = new InformationWindowHome(this);


private slots:
    void on_SignUpButton_clicked();
    void on_LoginButton_clicked();

    void on_GuestButton_clicked();
};
#endif // LANDINGPAGE_H
