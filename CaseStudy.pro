QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminsqlmanager.cpp \
    azurestorage.cpp \
    informationwindowhome.cpp \
    informationwindownews.cpp \
    informationwindownewsdetailed.cpp \
    informationwindownewslist.cpp \
    inquirywindow.cpp \
    inquirywindowaccount.cpp \
    inquirywindowhome.cpp \
    inquirywindowmenu.cpp \
    main.cpp \
    landingpage.cpp \
    mrichtextedit.cpp \
    mtextedit.cpp \
    newthreadswindow.cpp \
    passwordconfirmation.cpp \
    registerwindow.cpp \
    threaddetailsbasicinfo.cpp \
    threaddetailsparticipants.cpp \
    threadslistwidget.cpp \
    threadswindow.cpp \
    viewmessage.cpp \
    viewthread.cpp \
    viewthreadguest.cpp

HEADERS += \
    adminsqlmanager.h \
    azurestorage.h \
    cloudstorageservices.h \
    global.h \
    informationwindowhome.h \
    informationwindownews.h \
    informationwindownewsdetailed.h \
    informationwindownewslist.h \
    inquirywindow.h \
    inquirywindowaccount.h \
    inquirywindowhome.h \
    inquirywindowmenu.h \
    landingpage.h \
    mrichtextedit.h \
    mtextedit.h \
    newthreadswindow.h \
    passwordconfirmation.h \
    registerwindow.h \
    threaddetailsbasicinfo.h \
    threaddetailsparticipants.h \
    threadslistwidget.h \
    threadswindow.h \
    viewmessage.h \
    viewthread.h \
    viewthreadguest.h

FORMS += \
    adminsqlmanager.ui \
    informationwindowhome.ui \
    informationwindownews.ui \
    informationwindownewsdetailed.ui \
    informationwindownewslist.ui \
    inquirywindow.ui \
    inquirywindowaccount.ui \
    inquirywindowhome.ui \
    inquirywindowmenu.ui \
    landingpage.ui \
    mrichtextedit.ui \
    newthreadswindow.ui \
    passwordconfirmation.ui \
    registerwindow.ui \
    threaddetailsbasicinfo.ui \
    threaddetailsparticipants.ui \
    threadslistwidget.ui \
    threadswindow.ui \
    viewmessage.ui \
    viewthread.ui \
    viewthreadguest.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
