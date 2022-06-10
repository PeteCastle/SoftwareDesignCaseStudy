#ifndef INFORMATIONWINDOWGALLERY_H
#define INFORMATIONWINDOWGALLERY_H

#include <QWidget>

namespace Ui {
class InformationWindowGallery;
}

class InformationWindowGallery : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindowGallery(QWidget *parent = nullptr);
    ~InformationWindowGallery();

private:
    Ui::InformationWindowGallery *ui;
};

#endif // INFORMATIONWINDOWGALLERY_H
