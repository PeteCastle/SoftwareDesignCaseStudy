#ifndef PASSWORDCONFIRMATION_H
#define PASSWORDCONFIRMATION_H

#include <QDialog>
#include "global.h"

namespace Ui {
class PasswordConfirmation;
}

class PasswordConfirmation : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordConfirmation(QWidget *parent = nullptr, QString labelContents="", QString userFullName="", int buttonBehavior=-1,  int userID=-1);
    ~PasswordConfirmation();
    bool getPasswordAuthenticated();
    QString getPasswordInput();

private slots:
    void on_Cancel_clicked();

    void on_ConfirmPassword_clicked();

private:
    Ui::PasswordConfirmation *ui;
    void triggerButton(int buttonBehavior, int userID);

    bool isAuthenticated=0;
};

#endif // PASSWORDCONFIRMATION_H
