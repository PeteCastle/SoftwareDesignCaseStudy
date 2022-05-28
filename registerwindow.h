#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include "global.h"

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private:
    Ui::RegisterWindow *ui;
    QStringList coursesList;
    QStringList departmentsList;
    QStringList threadTagList;
    QStringList selectedTagList;

private slots:
    void modifyFieldsVisibility(bool isVisible);
    void on_AccountTypeStudentButton_clicked();
    void on_AccountTypeFacultyButton_clicked();
    void on_AccountTypeGuestButton_clicked();
    void on_CreateAccountButton_clicked();
    QString toNameCase(const QString& s);
    void on_AddTagButton_clicked();
    void on_RemoveYourTag_clicked();
};

#endif // REGISTERWINDOW_H
