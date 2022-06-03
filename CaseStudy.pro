QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    azurestorage.cpp \
    inquirywindow.cpp \
    main.cpp \
    landingpage.cpp \
    mrichtextedit.cpp \
    mtextedit.cpp \
    newthreadswindow.cpp \
    registerwindow.cpp \
    threadslistwidget.cpp \
    threadswindow.cpp \
    viewmessage.cpp \
    viewthread.cpp

HEADERS += \
    azurestorage.h \
    cloudstorageservices.h \
    global.h \
    inquirywindow.h \
    landingpage.h \
    mrichtextedit.h \
    mtextedit.h \
    newthreadswindow.h \
    registerwindow.h \
    threadslistwidget.h \
    threadswindow.h \
    viewmessage.h \
    viewthread.h

FORMS += \
    inquirywindow.ui \
    landingpage.ui \
    mrichtextedit.ui \
    newthreadswindow.ui \
    registerwindow.ui \
    threadslistwidget.ui \
    threadswindow.ui \
    viewmessage.ui \
    viewthread.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
