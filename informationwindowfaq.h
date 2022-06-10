#ifndef INFORMATIONWINDOWFAQ_H
#define INFORMATIONWINDOWFAQ_H

#include <QWidget>

namespace Ui {
class InformationWindowFAQ;
}

class InformationWindowFAQ : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindowFAQ(QWidget *parent = nullptr);
    ~InformationWindowFAQ();

private:
    Ui::InformationWindowFAQ *ui;
};

#endif // INFORMATIONWINDOWFAQ_H
