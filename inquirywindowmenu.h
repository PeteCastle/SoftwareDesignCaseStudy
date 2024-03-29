#ifndef INQUIRYWINDOWMENU_H
#define INQUIRYWINDOWMENU_H

#include <QWidget>

namespace Ui {
class InquiryWindowMenu;
}

class InquiryWindowMenu : public QWidget
{
    Q_OBJECT

public:
    explicit InquiryWindowMenu(QWidget *parent = nullptr, QString menuName="", QString menuIconLocation="");
    ~InquiryWindowMenu();

private:
    Ui::InquiryWindowMenu *ui;
};

#endif // INQUIRYWINDOWMENU_H
