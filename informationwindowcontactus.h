#ifndef INFORMATIONWINDOWCONTACTUS_H
#define INFORMATIONWINDOWCONTACTUS_H

#include <QWidget>

namespace Ui {
class InformationWindowContactUs;
}

class InformationWindowContactUs : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindowContactUs(QWidget *parent = nullptr);
    ~InformationWindowContactUs();

private:
    Ui::InformationWindowContactUs *ui;
};

#endif // INFORMATIONWINDOWCONTACTUS_H
