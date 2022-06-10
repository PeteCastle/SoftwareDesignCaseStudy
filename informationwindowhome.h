#ifndef INFORMATIONWINDOWHOME_H
#define INFORMATIONWINDOWHOME_H

#include <QWidget>
#include "informationwindownews.h"
#include "informationwindownewslist.h"
#include "informationwindowaboutus.h"
#include "informationwindowgallery.h"
#include "informationwindowcontactus.h"
#include "informationwindowfaq.h"


namespace Ui {
class InformationWindowHome;
}

class InformationWindowHome : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindowHome(QWidget *parent = nullptr);
    ~InformationWindowHome();

private slots:
    void on_HomeButton_clicked();

    void on_NewsButton_clicked();

    void on_AboutUsButton_clicked();

    void on_GalleryButton_clicked();

    void on_ContactUsbutton_clicked();

    void on_FAQButton_clicked();

private:
    Ui::InformationWindowHome *ui;
    QMap<QString,QWidget*> ActiveWindowsList;
    QMap<QString, std::function<void()>> WindowsList;
    void uncheckAllButtons();
    QVector<News> NewsList;

};

#endif // INFORMATIONWINDOWHOME_H
