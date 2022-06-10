#ifndef INFORMATIONWINDOWABOUTUS_H
#define INFORMATIONWINDOWABOUTUS_H

#include <QWidget>

namespace Ui {
class InformationWindowAboutUs;
}

class InformationWindowAboutUs : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindowAboutUs(QWidget *parent = nullptr);
    ~InformationWindowAboutUs();

private:
    Ui::InformationWindowAboutUs *ui;
};

#endif // INFORMATIONWINDOWABOUTUS_H
