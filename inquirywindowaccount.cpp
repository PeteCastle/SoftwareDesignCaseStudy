#include "inquirywindowaccount.h"
#include "ui_inquirywindowaccount.h"
#include <QSqlQuery>

InquiryWindowAccount::InquiryWindowAccount(QWidget *parent, AccountCredentials userCredentials) :
    QWidget(parent),
    ui(new Ui::InquiryWindowAccount)
{
    ui->setupUi(this);
    this->UserCredentials = userCredentials;

    //Display relevant user values
    ui->HeaderFullName->setText(userCredentials.lastName.toUpper() + ", " + userCredentials.firstName + " " + userCredentials.middleName);
    if(userCredentials.accountType>0){
        ui->HeaderDepartment->setText(userCredentials.department);
    }
    else{
        ui->HeaderDepartment->setVisible(false);
    }
    ui->HeaderDepartment->setText(userCredentials.department);
    //Instantiate all menu

   //Set usertype
    if(userCredentials.accountType==-1){ui->HeaderAccountType->setText("Guest");} //Not possible
    else if(userCredentials.accountType==0){ui->HeaderAccountType->setText("Non-student Account");}
    else if(userCredentials.accountType==1){ui->HeaderAccountType->setText("Student Account");}
    else if(userCredentials.accountType==2){ui->HeaderAccountType->setText("Organizational Account");}
    else if(userCredentials.accountType==3){ui->HeaderAccountType->setText("Administrator");}
    else{ui->HeaderAccountType->setText("Unknown Account Type");}

    //Set profilepicture
    reshapeProfilePicture(":/Icons/ProgramIcons/DefaultProfilePicture.jpg",ui->ProfilePictureLabel,80); // Default profile pic
    if(userCredentials.accountType!=-1 || userCredentials.accountProfilePicture!=""){
        storageAccess.downloadFile(userCredentials.accountProfilePicture,"profilepictures");
        QString profilePicture = userCredentials.accountProfilePicture;
        connect(timer, &QTimer::timeout, [this,profilePicture]{
            if(reshapeProfilePicture(profilePicture, ui->HeaderProfilePicture,80) && reshapeProfilePicture(profilePicture, ui->ProfilePicture,150)){
                timer->stop();
            }
        });
        timer->start(1000);
    }

    QSqlQuery getCoursesList = getQuery("SELECT * FROM Courses");
    while(getCoursesList.next()){
        ui->DepartmentComboBox->addItem(getCoursesList.value(0).toString());
    }

    QStringList threadTagList;
    QSqlQuery getTagsList = getQuery("SELECT * FROM ThreadTags");
    while(getTagsList.next()){
        threadTagList.append(getTagsList.value(0).toString());
    }
    QStringListModel *threadTagModel = new QStringListModel(threadTagList);
    QSortFilterProxyModel *threadTagFilterModel = new QSortFilterProxyModel();
    threadTagFilterModel->setSourceModel(threadTagModel);
    threadTagFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    threadTagFilterModel->setFilterKeyColumn(0);

    connect(ui->SearchTagsLineEdit,&QLineEdit::textChanged,threadTagFilterModel,&QSortFilterProxyModel::setFilterFixedString);
    ui->SearchTagsList->setModel(threadTagFilterModel);
    ui->SearchTagsList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->ProfilePictureDefaultButton->setEnabled(false);

    on_EditDetails_toggled(0);
    resetValuesToDefault();
}

InquiryWindowAccount::~InquiryWindowAccount()
{
    delete ui;
}

void InquiryWindowAccount::resetValuesToDefault(){
    TempUserCredentials = UserCredentials;
    ui->UsernameLineEdit->setText(UserCredentials.Username);
    ui->OldPasswordLineEdit->setText(UserCredentials.Password);
    //ui->NewPasswordLineEdit->setText("");
    ui->FirstNameLineEdit->setText(UserCredentials.firstName);
    ui->MiddleNameLineEdit->setText(UserCredentials.middleName);
    ui->LastNameLineEdit->setText(UserCredentials.lastName);
    ui->PersonalEmailLineEdit->setText(UserCredentials.personalEmail);
    ui->AcademicEmaiLineEdit->setText(UserCredentials.academicEmail);
    ui->DepartmentComboBox->setCurrentText(UserCredentials.department);
    ui->ContactNumberLineEdit->setText(UserCredentials.contactNumber);
    ui->YourTagsList->model()->deleteLater();
    ui->YourTagsList->setModel(new QStringListModel(TempUserCredentials.accountTags));

    if(UserCredentials.gender=="Male"){
        ui->GenderMaleButton->setChecked(true);
        ui->GenderMaleButton->setChecked(false);
    }
    else if(UserCredentials.gender=="Female"){
        ui->GenderMaleButton->setChecked(false);
        ui->GenderMaleButton->setChecked(true);
    }
    else{
        ui->GenderMaleButton->setChecked(false);
        ui->GenderMaleButton->setChecked(false);
    }

}

void InquiryWindowAccount::on_EditDetails_toggled(bool checked)
{
    //ui->NewPasswordLineEdit->setVisible(checked);
    //ui->NewPasswordLabel->setVisible(checked);
    ui->ChangePassword->setVisible(checked);
    ui->ProfilePictureDefaultButton->setVisible(checked);

    ui->SaveButton->setVisible(checked);

    ui->ProfilePictureButton->setVisible(checked);

    if(UserCredentials.accountType>0){
        ui->DepartmentComboBox->setEnabled(checked);
        ui->GenderMaleButton->setEnabled(checked);
        ui->GenderFemaleButton->setEnabled(checked);
        ui->AcademicEmaiLineEdit->setReadOnly(!checked);
    }
    else
    {
        ui->DepartmentComboBox->setVisible(false);
        ui->DepartmentLabl->setVisible(false);
        ui->GenderMaleButton->setVisible(false);
        ui->GenderFemaleButton->setVisible(false);
        ui->AcademicEmaiLineEdit->setVisible(false);
        ui->AcademicEmailLabel->setVisible(false);
        ui->GenderLabel->setVisible(false);
    }

    if(UserCredentials.accountType>1){
        ui->SearchTagsLabel->setVisible(checked);
        ui->SearchTagsLineEdit->setVisible(checked);
        ui->SearchTagsList->setVisible(checked);
        ui->YourTagsRemoveButton->setVisible(checked);
    }
    else{
        ui->SearchTagsLabel->setVisible(false);
        ui->SearchTagsLineEdit->setVisible(false);
        ui->SearchTagsList->setVisible(false);
        ui->YourTagsRemoveButton->setVisible(false);
        ui->YourTagsList->setVisible(false);
        ui->YourTagsLabel->setVisible(false);
    }
    ui->UsernameLineEdit->setReadOnly(!checked);
    ui->OldPasswordLineEdit->setReadOnly(!checked);
    ui->FirstNameLineEdit->setReadOnly(!checked);
    ui->MiddleNameLineEdit->setReadOnly(!checked);
    ui->LastNameLineEdit->setReadOnly(!checked);;
    ui->PersonalEmailLineEdit->setReadOnly(!checked);
    ui->ContactNumberLineEdit->setReadOnly(!checked);


    if(checked){
        ui->OldPasswordLineEdit->setText("");
        ui->OldPasswordLabel->setText("Enter your old password");
    }
    if(!checked){
        ui->OldPasswordLabel->setText("Password");
        resetValuesToDefault();
    }
}



void InquiryWindowAccount::on_SaveButton_clicked()
{
    if(ui->DepartmentComboBox->currentText()=="(Select your course or department)" && TempUserCredentials.accountType>0){
        QMessageBox::warning(this,"Invalid department selection","Please enter a department and try again.");
        return;
    }
    if(TempUserCredentials.accountTags.isEmpty()&& TempUserCredentials.accountType>1){
        QMessageBox::warning(this,"Empty account tags","Please enter a tag and try again.");
        return;
    }

    PasswordConfirmation *confirmPassword = new PasswordConfirmation(this,"To continue, please enter your current password:",UserCredentials.firstName + " " + UserCredentials.lastName,1,UserCredentials.UserID);
    confirmPassword->exec();
    if(!confirmPassword->getPasswordAuthenticated()){
       QMessageBox::information(this,"Not authenticated","Please enter your password before continuing.");
       return;
    }

    QStringList updateAccountPlaceholder;
    updateAccountPlaceholder.append(TempUserCredentials.Username);
    updateAccountPlaceholder.append(TempUserCredentials.firstName);
    updateAccountPlaceholder.append(TempUserCredentials.middleName);
    updateAccountPlaceholder.append(TempUserCredentials.lastName);
    updateAccountPlaceholder.append(TempUserCredentials.academicEmail);
    updateAccountPlaceholder.append(TempUserCredentials.personalEmail);
    updateAccountPlaceholder.append(TempUserCredentials.contactNumber);

    if(ui->GenderMaleButton->isChecked()){
        TempUserCredentials.gender = "Female";
        updateAccountPlaceholder.append(TempUserCredentials.gender);
    }
    else if(ui->GenderMaleButton->isChecked()){
        TempUserCredentials.gender = "Male";
        updateAccountPlaceholder.append(TempUserCredentials.gender);
    }
    else{
        TempUserCredentials.gender = "Unknown";
        updateAccountPlaceholder.append(TempUserCredentials.gender);
    }

    //QString newImageKey;
    if(newProfilePictureFileName!=""){
        QString newImageKey = generateAlphaNumericString();
        storageAccess.uploadFile(newProfilePictureFileName,newImageKey,"profilepictures");

        //New Values
        QStringList imageplaceholder;
        imageplaceholder.append(newImageKey);
        QFile file(newProfilePictureFileName);
        QFileInfo fileInfo(file);
        imageplaceholder.append(fileInfo.fileName());

        //Old Values
        //imageplaceholder.append(UserCredentials.accountProfilePicture);
        //MAJOR ISSUE: SQL SOMETIMES DOESNT UPDATE TABLE; REVERTING TO ADD ROW INSTEAD OF UPDATING

        /*QString imageUploadQueryText = "UPDATE FileDictionary "
                                       "SET fileID = ? ,"
                                       "    fileName = ? "
                                       "WHERE fileID = ? ;";*/
        QString imageUploadQueryText = "INSERT INTO FileDictionary(fileID,fileContainer,fileName) "
                                       "VALUES(?,'profilepictures',?);";
        QSqlQuery imageUploadQuery = getQuery(imageUploadQueryText,imageplaceholder);
        if(imageUploadQuery.lastError().text()!=""){
            QMessageBox::warning(this,"Update details error","An error has occured.  Please send the following details to the developer.\n\n" + imageUploadQuery.lastError().text());
            return;
        }
        updateAccountPlaceholder.append(newImageKey);
    }
    else{
        updateAccountPlaceholder.append(UserCredentials.accountProfilePicture);
    }

    updateAccountPlaceholder.append(TempUserCredentials.accountTags.join(','));
    updateAccountPlaceholder.append(ui->DepartmentComboBox->currentText());

    //WHERE VALUES
    updateAccountPlaceholder.append(QString::number(UserCredentials.UserID));
    updateAccountPlaceholder.append(UserCredentials.Username);

    QString updateAccountQuery = "UPDATE Accounts "
                                 "SET Username = ?, "
                                 "  FirstName = ?, "
                                 "  MiddleName = ?, "
                                 "  LastName = ?, "
                                 "  AcademicEmail = ?, "
                                 "  PersonalEmail = ?, "
                                 "  ContactNumber =?, "
                                 "  Gender = ?, "
                                 "  AccountProfilePicture = ?, "
                                 "  AccountTags = ?, "
                                 "  Department = ? "
                                 "WHERE UserID = ? AND Username = ?;";

    QSqlQuery updateAccount = getQuery(updateAccountQuery,updateAccountPlaceholder);
    if(updateAccount.lastError().text()==""){
        QMessageBox::information(this,"Update information successful","You need to login again for changes to take into effect.");
    }
    else{
        QMessageBox::information(this,"An error has occured","Please see the developer and send the following error: " + updateAccount.lastError().text());
    }

    //storageAccess.deleteFile("ThreadTags.csv","profilepictures");
}





void InquiryWindowAccount::on_SearchTagsList_clicked(const QModelIndex &index)
{

    QString selected = ui->SearchTagsList->currentIndex().data(Qt::DisplayRole).toString();
    if(TempUserCredentials.accountTags.indexOf(selected)==-1){
        TempUserCredentials.accountTags.append(selected);
    }
    ui->YourTagsList->model()->deleteLater();
    ui->YourTagsList->setModel(new QStringListModel(TempUserCredentials.accountTags));

}


void InquiryWindowAccount::on_YourTagsRemoveButton_clicked()
{
    int index = ui->YourTagsList->currentIndex().row();
    if(index!=-1){
        TempUserCredentials.accountTags.remove(index);
        ui->YourTagsList->model()->deleteLater();
        ui->YourTagsList->setModel(new QStringListModel(TempUserCredentials.accountTags));
    }
}


void InquiryWindowAccount::on_PersonalEmailLineEdit_editingFinished()
{
    QString text = ui->PersonalEmailLineEdit->text();
    QRegularExpression personalEmailRegex("[a-z0-9]+@[a-z]+.[a-z0-9A-Z]{2,3}");
    if(!personalEmailRegex.match(text).hasMatch()){
        QMessageBox::warning(this,"Invalid email","Please enter your personal email.");
        ui->PersonalEmailLineEdit->setText(UserCredentials.personalEmail);
    }
    else{
        TempUserCredentials.personalEmail = text;
    }
}


void InquiryWindowAccount::on_AcademicEmaiLineEdit_editingFinished()
{
    QString text = ui->AcademicEmaiLineEdit->text().toLower().trimmed();
    QRegularExpression plmEmailRegex("[a-zA-Z0-9]+@plm.edu.ph");
    if(!plmEmailRegex.match(text).hasMatch()){
        QMessageBox::warning(this,"Invalid email","Please enter your PLM email.");
        ui->AcademicEmaiLineEdit->setText(UserCredentials.academicEmail);
    }
    else{
        TempUserCredentials.academicEmail = text;
    }
}


void InquiryWindowAccount::on_UsernameLineEdit_editingFinished()
{
    QString username = ui->UsernameLineEdit->text().trimmed().toLower();
    if(username.isEmpty()|| username.contains(' ')){
        QMessageBox::warning(this,"Incomplete input","Please enter your username.  Note that username must NOT contain any spaces.");
        ui->UsernameLineEdit->setText(UserCredentials.Username);
        return;
    }
    QString getAccountAvailabilityQuery = "SELECT 1 "
                                          "FROM Accounts "
                                          "WHERE LOWER(Username)= ? ;";
    QSqlQuery getAccountAvailability = getQuery(getAccountAvailabilityQuery, QStringList(username));
    getAccountAvailability.next();
    if(getAccountAvailability.value(0).toBool() && ui->UsernameLineEdit->text()!=UserCredentials.Username){
        QMessageBox::information(this,"Username already taken", "Please enter another username and try again.");
        ui->UsernameLineEdit->setText(UserCredentials.Username);
        return;
    }
    else{
        ui->UsernameLineEdit->setText(username);
        TempUserCredentials.Username = username;
    }


}


void InquiryWindowAccount::on_FirstNameLineEdit_editingFinished()
{
    QString firstName = toNameCase(ui->FirstNameLineEdit->text()).trimmed();
    if(firstName.isEmpty()){
        QMessageBox::warning(this,"Incomplete input","Please enter your first name.");
        ui->FirstNameLineEdit->setText(UserCredentials.firstName);
    }
    else{
        ui->FirstNameLineEdit->setText(firstName);
        TempUserCredentials.firstName = firstName;
    }
}


void InquiryWindowAccount::on_MiddleNameLineEdit_editingFinished()
{
    QString middleName = toNameCase(ui->MiddleNameLineEdit->text()).trimmed();
    if(middleName.isEmpty()){
        QMessageBox::warning(this,"Incomplete input","Please enter your middle name.");
        ui->MiddleNameLineEdit->setText(UserCredentials.middleName);
    }
    else{
        ui->FirstNameLineEdit->setText(middleName);
        TempUserCredentials.middleName = middleName;
    }
}


void InquiryWindowAccount::on_LastNameLineEdit_editingFinished()
{
    QString lastName = toNameCase(ui->LastNameLineEdit->text()).trimmed();
    if(lastName.isEmpty()){
        QMessageBox::warning(this,"Incomplete input","Please enter your middle name.");
        ui->LastNameLineEdit->setText(UserCredentials.lastName);
    }
    else{
        ui->LastNameLineEdit->setText(lastName);
        TempUserCredentials.lastName = lastName;
    }
}

QString InquiryWindowAccount::toNameCase(const QString& s)
{
    QStringList parts = s.split(' ', Qt::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join(" ");
}

void InquiryWindowAccount::on_ContactNumberLineEdit_editingFinished()
{
    QString contactNumber = ui->ContactNumberLineEdit->text().trimmed();
    QRegularExpression contactNumberRegex("[0-9]|[+()]");
    if(!contactNumberRegex.match(contactNumber).hasMatch()){
        QMessageBox::warning(this,"Invalid input","Please enter your contact number.");
        ui->ContactNumberLineEdit->setText(UserCredentials.contactNumber);
    }
    else{
        TempUserCredentials.contactNumber = contactNumber;
    }
}


void InquiryWindowAccount::on_ChangePassword_clicked()
{
    PasswordConfirmation *confirmPassword = new PasswordConfirmation(this,"Enter your old password:",UserCredentials.firstName + " " + UserCredentials.lastName,1,UserCredentials.UserID);
    confirmPassword->exec();
    if(confirmPassword->getPasswordAuthenticated()){
        PasswordConfirmation *newPassword = new PasswordConfirmation(this,"Enter your new password:",UserCredentials.firstName + " " + UserCredentials.lastName,2,UserCredentials.UserID);
        newPassword->exec();
        if(newPassword->getPasswordAuthenticated()){
            QString password = newPassword->getPasswordInput();

            QString newPasswordQuery = "DECLARE @hashVarchar varchar(128) = ?; "
                                           "UPDATE Accounts "
                                           "SET Userpass = HASHBYTES('SHA2_256',@hashVarchar) "
                                           "WHERE UserID = ? AND Username = ?;";
            getQuery(newPasswordQuery, QStringList{password,QString::number(UserCredentials.UserID),UserCredentials.Username});
            QMessageBox::information(this,"Success!","Successfuly changed your password.  You may need to restart this program for changes to take effect.");
        }
        else{
            qDebug() << "Input new password cancelled by user.";
        }
    }
    else{
        qDebug() << "Input old password cancelled by user.";
    }
}


void InquiryWindowAccount::on_ProfilePictureButton_clicked()
{
    newProfilePictureFileName = QFileDialog::getOpenFileName(this, tr("Select Image"),QStandardPaths::writableLocation((QStandardPaths::DownloadLocation)),tr("Image Files (*.png *.jpg *.jpeg )"));
    QFile file(newProfilePictureFileName);
    if(file.size()>5000000){
        QMessageBox::information(this,"File too large","Due to storage limitations, file uploads are only limited up to 5MB.  Sorry!");
        return;
    }

    if(newProfilePictureFileName!=""){
        reshapeProfilePicture(newProfilePictureFileName, ui->ProfilePicture,150,1);
        ui->ProfilePictureDefaultButton->setEnabled(true);
    }
}


void InquiryWindowAccount::on_ProfilePictureDefaultButton_clicked()
{
    reshapeProfilePicture(UserCredentials.accountProfilePicture, ui->ProfilePicture,150);
    ui->ProfilePictureDefaultButton->setEnabled(false);
    newProfilePictureFileName = "";

}

