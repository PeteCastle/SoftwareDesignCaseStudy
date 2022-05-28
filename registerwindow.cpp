#include "registerwindow.h"
#include "ui_registerwindow.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QButtonGroup>
#include <QSqlQuery>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QMap>


RegisterWindow::RegisterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);



    QButtonGroup accountTypeButtonGroup;
    accountTypeButtonGroup.addButton(ui->AccountTypeFacultyButton);
    accountTypeButtonGroup.addButton(ui->AccountTypeGuestButton);
    accountTypeButtonGroup.addButton(ui->AccountTypeStudentButton);

    QButtonGroup genderButtonGroup;
    genderButtonGroup.addButton(ui->GenderFemaleButton);
    genderButtonGroup.addButton(ui->GenderMaleButton);

    modifyFieldsVisibility(0);


    QSqlQuery getCoursesList = getQuery("SELECT * FROM Courses");
    while(getCoursesList.next()){
        coursesList.append(getCoursesList.value(0).toString());
    }
    qDebug() << coursesList;

    QSqlQuery getDepartmentsList = getQuery("SELECT * FROM Departments");
    while(getDepartmentsList.next()){
        departmentsList.append(getDepartmentsList.value(0).toString());
    }

    QSqlQuery getTagsList = getQuery("SELECT * FROM ThreadTags");
    while(getTagsList.next()){
        threadTagList.append(getTagsList.value(0).toString());
    }

    QStringListModel *threadTagModel = new QStringListModel(threadTagList);

    QSortFilterProxyModel *threadTagFilterModel = new QSortFilterProxyModel();
    threadTagFilterModel->setSourceModel(threadTagModel);
    threadTagFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    threadTagFilterModel->setFilterKeyColumn(0);

    connect(ui->AvailableTagsSearch,&QLineEdit::textChanged,threadTagFilterModel,&QSortFilterProxyModel::setFilterFixedString);
    ui->AvailableTagsList->setModel(threadTagFilterModel);
    ui->AvailableTagsList->setEditTriggers(QAbstractItemView::NoEditTriggers);



}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::modifyFieldsVisibility(bool isVisible){
    ui->YourTagsLabel->setVisible(isVisible);
    ui->YourTagsList->setVisible(isVisible);
    ui->RemoveYourTag->setVisible(isVisible);
    ui->AvailableTagsList->setVisible(isVisible);
    ui->AvailableTagsLabel->setVisible(isVisible);
    ui->AvailableTagsSearch->setVisible(isVisible);
    ui->AddTagButton->setVisible(isVisible);
    ui->GenderFemaleButton->setVisible(isVisible);
    ui->GenderMaleButton->setVisible(isVisible);
    ui->UsernameLabel->setVisible(isVisible);
    ui->UsernameLineEdit->setVisible(isVisible);
    ui->PersonalEmailLineEdit->setVisible(isVisible);
    ui->PersonalEmailLabel->setVisible(isVisible);
    ui->PasswordLineEdit->setVisible(isVisible);
    ui->PasswordLabel->setVisible(isVisible);
    ui->MiddleNameLineEdit->setVisible(isVisible);
    ui->MiddleNameLabel->setVisible(isVisible);
    ui->LastNameLineEdit->setVisible(isVisible);
    ui->LastNameLabel->setVisible(isVisible);
    ui->GenderLabel->setVisible(isVisible);
    ui->FirstNameLabel->setVisible(isVisible);
    ui->FirstNameLineEdit->setVisible(isVisible);
    ui->CourseLineEdit->setVisible(isVisible);
    ui->CourseLabel->setVisible(isVisible);
    ui->ContactNumberLineEdit->setVisible(isVisible);
    ui->ContactNumberLabel->setVisible(isVisible);
    ui->AcademicEmailLabel->setVisible(isVisible);
    ui->AcademicEmailLineEdit->setVisible(isVisible);
}

void RegisterWindow::on_AccountTypeStudentButton_clicked()
{
    modifyFieldsVisibility(0);
    ui->UsernameLabel->setVisible(true);
    ui->UsernameLineEdit->setVisible(true);
    ui->PasswordLineEdit->setVisible(true);
    ui->PasswordLabel->setVisible(true);
    ui->FirstNameLineEdit->setVisible(true);
    ui->FirstNameLabel->setVisible(true);
    ui->MiddleNameLineEdit->setVisible(true);
    ui->MiddleNameLabel->setVisible(true);
    ui->LastNameLineEdit->setVisible(true);
    ui->LastNameLabel->setVisible(true);
    ui->PersonalEmailLineEdit->setVisible(true);
    ui->PersonalEmailLabel->setVisible(true);
    ui->AcademicEmailLineEdit->setVisible(true);
    ui->AcademicEmailLabel->setVisible(true);
    ui->ContactNumberLineEdit->setVisible(true);
    ui->ContactNumberLabel->setVisible(true);
    ui->CourseLineEdit->setVisible(true);
    ui->CourseLabel->setVisible(true);
    ui->GenderLabel->setVisible(true);
    ui->GenderFemaleButton->setVisible(true);
    ui->GenderMaleButton->setVisible(true);
    ui->CourseLabel->setText("Course");
    ui->PersonalEmailLabel->setText("Personal Email");
    ui->ContactNumberLabel->setText("Contact Number");
    ui->CourseLineEdit->clear();
    ui->CourseLineEdit->addItems(coursesList);
}


void RegisterWindow::on_AccountTypeGuestButton_clicked()
{
    modifyFieldsVisibility(0);
    ui->UsernameLabel->setVisible(true);
    ui->UsernameLineEdit->setVisible(true);
    ui->PasswordLineEdit->setVisible(true);
    ui->PasswordLabel->setVisible(true);
    ui->FirstNameLineEdit->setVisible(true);
    ui->FirstNameLabel->setVisible(true);
    ui->MiddleNameLineEdit->setVisible(true);
    ui->MiddleNameLabel->setVisible(true);
    ui->LastNameLineEdit->setVisible(true);
    ui->LastNameLabel->setVisible(true);
    ui->PersonalEmailLineEdit->setVisible(true);
    ui->PersonalEmailLabel->setVisible(true);
    ui->ContactNumberLineEdit->setVisible(true);
    ui->ContactNumberLabel->setVisible(true);
    ui->PersonalEmailLabel->setText("Personal Email (Optional)");
    ui->ContactNumberLabel->setText("Contact Number (optional)");
}

void RegisterWindow::on_AccountTypeFacultyButton_clicked()
{
    modifyFieldsVisibility(1);
    ui->CourseLabel->setText("Department/Organization");
    ui->PersonalEmailLabel->setText("Personal Email");
    ui->ContactNumberLabel->setText("Contact Number");

    ui->CourseLineEdit->clear();
    ui->CourseLineEdit->addItems(departmentsList);
}


void RegisterWindow::on_CreateAccountButton_clicked()
{
    int accountType=0;
    if(ui->AccountTypeGuestButton->isChecked()){
        accountType = 0;
    }
    else if(ui->AccountTypeStudentButton->isChecked()){
        accountType = 1;
    }
    else if(ui->AccountTypeFacultyButton->isChecked()){
        accountType = 2;
    }
    else{
        QMessageBox::warning(this,"Incomplete input","Please enter your account type.");
        return;
    }

    //Declare all fields regardless of permission level
    QString username, password, firstName, middleName, lastName, course, gender, personalEmail, plmEmail, contactNumber;

    switch(accountType){
        case 2:     //All fields exclusive to faculty ONLY
            if(selectedTagList.isEmpty()){
                QMessageBox::warning(this,"Incomplete input","Please enter at least one tag available.");
                return;
            }
        case 1: {   //All fields exclusive to faculty AND student
            course = ui->CourseLineEdit->currentText();
            if(course == "(Select your course or department)"  && accountType!=0){
                QMessageBox::warning(this,"Incomplete input","Please enter your course/department.");
                return;
            }
            if(ui->GenderMaleButton->isChecked()  && accountType!=0){
                gender = "Male";
            }
            else if(ui->GenderFemaleButton->isChecked()  && accountType!=0){
                gender = "Female";
            }
            else if(accountType!=0){
                QMessageBox::warning(this,"Incomplete input","Please input your gender.");
                return;
            }
            QRegularExpression plmEmailRegex("[a-zA-Z0-9]+@plm.edu.ph");
            plmEmail = ui->AcademicEmailLineEdit->text().toLower().trimmed();

            if(!plmEmailRegex.match(plmEmail).hasMatch()){
                QMessageBox::warning(this,"Invalid email","Please enter your PLM email.");
                return;
            }
        }
        case 0:{ //All fields exclusive to faculty, student, AND guest
            username = ui->UsernameLineEdit->text().trimmed().toLower();
            if(username.isEmpty()){
                QMessageBox::warning(this,"Incomplete input","Please enter your username.");
                return;
            }
            password = ui->PasswordLineEdit->text();
            if(password.isEmpty()){
                QMessageBox::warning(this,"Incomplete input","Please enter your password.");
                return;
            }
            firstName = toNameCase(ui->FirstNameLineEdit->text()).trimmed();
            if(firstName.isEmpty()){
                QMessageBox::warning(this,"Incomplete input","Please enter your first name.");
                return;
            }
            middleName = toNameCase(ui->MiddleNameLineEdit->text()).trimmed();
            if(middleName.isEmpty()){
                QMessageBox::warning(this,"Incomplete input","Please enter your middle name.");
                return;
            }
            lastName = toNameCase(ui->LastNameLineEdit->text()).trimmed();
            if(lastName.isEmpty()){
                QMessageBox::warning(this,"Incomplete input","Please enter your last name.");
                return;
            }
            personalEmail = ui->PersonalEmailLineEdit->text().toLower().trimmed();
            QRegularExpression personalEmailRegex("[a-z0-9]+@[a-z]+.[a-z0-9A-Z]{2,3}");
            if(!personalEmailRegex.match(personalEmail).hasMatch()){
                if(accountType==0 && personalEmail.isEmpty()){
                }
                else{
                    QMessageBox::warning(this,"Invalid email","Please enter your personal email.");
                    return;
                }

            }
            contactNumber = ui->ContactNumberLineEdit->text().trimmed();
            QRegularExpression contactNumberRegex("[0-9]|[+()]");
            if(!contactNumberRegex.match(contactNumber).hasMatch()&& !personalEmail.isEmpty()){
                if(accountType==0 && contactNumber.isEmpty()){
                }
                else{
                    QMessageBox::warning(this,"Invalid input","Please enter your contact number.");
                    return;
                }
            }
        break;
        }
    }

    qDebug() <<  username << password << firstName<< middleName<< lastName<< course<< gender<< personalEmail<< plmEmail<< contactNumber << selectedTagList;

    //Verifies if the username and/or PLM email address already exists
    QString getAccountAvailabilityQuery = "SELECT COUNT(*) "
                                          "FROM Accounts "
                                          "WHERE LOWER(Username)='" + username + "' OR LOWER(AcademicEmail) = '" + plmEmail + "';";
    QSqlQuery getAccountAvailability = getQuery(getAccountAvailabilityQuery);
    getAccountAvailability.next();
    const int count = getAccountAvailability.value(0).toInt();

    if(count!=0){
        QMessageBox::warning(this,"Account already taken", "Username and/or PLM email (for students) are already taken.");
        return;
    }
    qDebug() << count;







}

QString RegisterWindow::toNameCase(const QString& s)
{
    QStringList parts = s.split(' ', Qt::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join(" ");
}


void RegisterWindow::on_AddTagButton_clicked()
{
    QString selected = ui->AvailableTagsList->currentIndex().data(Qt::DisplayRole).toString();
    if(selectedTagList.indexOf(selected)==-1){
        selectedTagList.append(selected);
    }

    QStringListModel *selectedTags = new QStringListModel(selectedTagList);
    ui->YourTagsList->setModel(selectedTags);
}

void RegisterWindow::on_RemoveYourTag_clicked()
{
    int index = ui->YourTagsList->currentIndex().row();
    if(index!=-1){
        selectedTagList.remove(index);
        QStringListModel *selectedTags = new QStringListModel(selectedTagList);
        ui->YourTagsList->setModel(selectedTags);
    }

}

