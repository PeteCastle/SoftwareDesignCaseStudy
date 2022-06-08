#ifndef INQUIRYWINDOWHOME_H
#define INQUIRYWINDOWHOME_H

#include <QWidget>

namespace Ui {
class InquiryWindowHome;
}

class InquiryWindowHome : public QWidget
{
    Q_OBJECT

public:
    explicit InquiryWindowHome(QWidget *parent = nullptr);
    ~InquiryWindowHome();

private:
    Ui::InquiryWindowHome *ui;
};

#endif // INQUIRYWINDOWHOME_H
