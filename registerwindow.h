#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include "global.h"
#include <QLabel>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();
    QString getUsername();
    QString getPassword();

private:
    Ui::RegisterWindow *ui;
    int accountType=0;
    QStringList coursesList;
    QStringList departmentsList;
    QStringList threadTagList;
    QStringList selectedTagList;
    QString username, password, firstName, middleName, lastName,
        course, gender, personalEmail, plmEmail, contactNumber, profilePictureFilePath="(Empty)";

private slots:
    void modifyFieldsVisibility(bool isVisible);
    void on_AccountTypeStudentButton_clicked();
    void on_AccountTypeFacultyButton_clicked();
    void on_AccountTypeGuestButton_clicked();
    void on_CreateAccountButton_clicked();
    QString toNameCase(const QString& s);
    void on_AddTagButton_clicked();
    void on_RemoveYourTag_clicked();
    QPixmap *reshapeProfilePicture(QString imageFilePath, QLabel *label, int labelSize);
    void on_ProfilePictureButton_clicked();
};

#endif // REGISTERWINDOW_H
