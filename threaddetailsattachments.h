#ifndef THREADDETAILSATTACHMENTS_H
#define THREADDETAILSATTACHMENTS_H

#include <QWidget>

namespace Ui {
class ThreadDetailsAttachments;
}

class ThreadDetailsAttachments : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadDetailsAttachments(QWidget *parent = nullptr);
    ~ThreadDetailsAttachments();

private:
    Ui::ThreadDetailsAttachments *ui;
};

#endif // THREADDETAILSATTACHMENTS_H
