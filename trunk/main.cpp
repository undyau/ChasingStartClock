#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QQuickItem>
#include <QQmlContext>
#include "crunner.h"
#include <QList>
#include <QTimer>
#include "clistupdater.h"
#include "cconfiguration.h"
#include <QTime>
#include <QUrl>



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    QQmlContext *ctxt = viewer.rootContext();

    CConfiguration config("FAKE");
    CListUpdater updater(ctxt, &config);

    //viewer.setMainQmlFile(QStringLiteral("qml/Countdown2/main.qml"));
   // QUrl url("qrc:qml/Countdown2/main.qml");

    //viewer.setMainQmlFile(QUrl("qrc:qml/Countdown2/main.qml");
    viewer.setSource(QUrl("qrc:qml/qml/Countdown2/main.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
   // viewer.showFullScreen();
    viewer.showExpanded();


    return app.exec();
}
