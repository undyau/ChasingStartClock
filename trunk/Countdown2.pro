# Add more folders to ship with the application, here
folder_01.source = qml/Countdown2
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

VERSION = 1.3.0.0
QMAKE_TARGET_COMPANY = "undy"
QMAKE_TARGET_PRODUCT = ChasingStartClock
QMAKE_TARGET_DESCRIPTION = "Start Clock for Orienteering"
QMAKE_TARGET_COPYRIGHT = "Copyright © 2014 Andy Simpson"

QT += xml

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    crunner.cpp \
    clistupdater.cpp \
    ciof3xmlcontenthandler.cpp \
    cconfiguration.cpp \
    calert.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    crunner.h \
    clistupdater.h \
    ciof3xmlcontenthandler.h \
    cconfiguration.h \
    calert.h

RESOURCES += \
    resource.qrc
