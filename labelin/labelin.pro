#-------------------------------------------------
#
# Project created by QtCreator 2018-07-20T03:31:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labelin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:/opencv/mybuild/install/include
LIBS += -LC:/opencv/mybuild/install/x86/mingw/bin\
    -lopencv_core310 \
    -lopencv_highgui310 \
    -lopencv_imgproc310 \
    -lopencv_imgcodecs310 \
    -lopencv_features2d310 \
    -lopencv_calib3d310 \

#unix {
#    CONFIG += link_pkgconfig
#    PKGCONFIG += opencv
#}
