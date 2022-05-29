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
    newthreadswindow.cpp \
    registerwindow.cpp \
    threadswindow.cpp

HEADERS += \
    azurestorage.h \
    cloudstorageservices.h \
    global.h \
    inquirywindow.h \
    landingpage.h \
    newthreadswindow.h \
    registerwindow.h \
    threadswindow.h

FORMS += \
    inquirywindow.ui \
    landingpage.ui \
    newthreadswindow.ui \
    registerwindow.ui \
    threadswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
