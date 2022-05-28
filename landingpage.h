#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>
#include "registerwindow.h"



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

private slots:
    void testSlot1();

    void on_SignUpButton_clicked();
};
#endif // LANDINGPAGE_H
