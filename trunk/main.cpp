#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QQuickItem>
#include <QQmlContext>
#include "crunner.h"
#include <QList>
#include <QTimer>
#include "clistupdater.h"
#include <QTime>



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    QQmlContext *ctxt = viewer.rootContext();

    CListUpdater updater(ctxt, "C:\\Users\\OW\\Desktop\\StartList.xml");

    viewer.setMainQmlFile(QStringLiteral("qml/Countdown2/main.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
   // viewer.showFullScreen();
    viewer.showExpanded();


    return app.exec();
}
