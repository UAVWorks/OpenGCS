#-------------------------------------------------
#
# Project created by QtCreator 2014-08-29T09:54:57
#
#-------------------------------------------------

!equals(QT_MAJOR_VERSION, 5) | !greaterThan(QT_MINOR_VERSION, 1) {
    error("Unsupported Qt version, Qt 5.2+ is required")
}

###################################################################################################

CONFIG += qt thread
QT += network opengl svg sql declarative

LIBS += -L/home/xufeng/公共/Qt5/OPMapControl -lopmapwidget
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video
###################################################################################################

TARGET = OpenGCS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gcsmapwidget.cpp \
    pfdwidget.cpp \
    camerawidget.cpp \
    cameracapture.cpp \
    imagebuffer.cpp \
    imageprocessing.cpp \
    jswidget.cpp \
    ctrlcmd.cpp \
    joystick.cpp \
    sdljoystick.cpp

HEADERS  += mainwindow.h \
    gcsmapwidget.h \
    pfdwidget.h \
    camerawidget.h \
    cameracapture.h \
    imagebuffer.h \
    imageprocessing.h \
    jswidget.h \
    ctrlcmd.h \
    joystick.h \
    sdljoystick.h

FORMS    += mainwindow.ui \
    gcsmapwidget.ui \
    pfdwidget.ui \
    camerawidget.ui \
    jswidget.ui

RESOURCES += \
    files.qrc


###################################################################################################
include(libs/opmapcontrol/opmapcontrol.pri)
INCLUDEPATH += libs/opmapcontrol/src libs/opmapcontrol

include(./qesp/qextserialport.pri)
INCLUDEPATH += /usr/include/SDL
LIBS += -lSDL -lSDLmain
###################################################################################################
