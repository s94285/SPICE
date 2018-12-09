#-------------------------------------------------
#
# Project created by QtCreator 2018-11-24T16:02:26
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = SPICE
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    circuitsimulation.cpp \
    node.cpp \
    basiccomponent.cpp \
    linearcomponent.cpp \
    source.cpp \
    capacitor.cpp \
    inductor.cpp \
    workspace.cpp \
    ground.cpp \
    resistor.cpp \
    editdialog.cpp \
    sourcedialog.cpp

HEADERS += \
        mainwindow.h \
    circuitsimulation.h \
    node.h \
    basiccomponent.h \
    linearcomponent.h \
    source.h \
    capacitor.h \
    inductor.h \
    workspace.h \
    ground.h \
    resistor.h \
    editdialog.h \
    sourcedialog.h

FORMS += \
        mainwindow.ui \
    editdialog.ui \
    sourcedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
