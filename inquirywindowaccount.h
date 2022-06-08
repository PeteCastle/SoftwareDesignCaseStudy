#ifndef INQUIRYWINDOWACCOUNT_H
#define INQUIRYWINDOWACCOUNT_H

#include <QWidget>
#include "global.h"
#include "passwordconfirmation.h"
#include <QFileDialog>
#include <QSqlError>

namespace Ui {
class InquiryWindowAccount;
}

class InquiryWindowAccount : public QWidget
{
    Q_OBJECT

public:
    explicit InquiryWindowAccount(QWidget *parent = nullptr, AccountCredentials userCredentials={});
    ~InquiryWindowAccount();

private slots:
    void resetValuesToDefault();

    void on_EditDetails_toggled(bool checked);

    void on_SaveButton_clicked();


    void on_SearchTagsList_clicked(const QModelIndex &index);

    void on_YourTagsRemoveButton_clicked();

    void on_PersonalEmailLineEdit_editingFinished();

    void on_AcademicEmaiLineEdit_editingFinished();

    void on_UsernameLineEdit_editingFinished();

    void on_FirstNameLineEdit_editingFinished();

    void on_MiddleNameLineEdit_editingFinished();

    void on_LastNameLineEdit_editingFinished();

    void on_ContactNumberLineEdit_editingFinished();

    void on_ChangePassword_clicked();

    void on_ProfilePictureButton_clicked();

    void on_ProfilePictureDefaultButton_clicked();

private:
    Ui::InquiryWindowAccount *ui;
    AccountCredentials UserCredentials;
    AccountCredentials TempUserCredentials;
    QTimer *timer = new QTimer();
    QStringList tempTagsList;
    QString toNameCase(const QString& s);
    QString newProfilePictureFileName;
};

#endif // INQUIRYWINDOWACCOUNT_H
