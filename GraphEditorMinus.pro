QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editwindow.cpp \
    insertfromtext.cpp \
    main.cpp \
    mainwindow.cpp \
    myedge.cpp \
    mynode.cpp \
    myscene.cpp

HEADERS += \
    editwindow.h \
    insertfromtext.h \
    mainwindow.h \
    myedge.h \
    mynode.h \
    myscene.h

FORMS += \
    insertfromtext.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    GraphEditorMinus.qrc

#添加exe图标
RC_ICONS = pictures/APP_ICON.ico
