#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Qt::WindowFlags flags = w.windowFlags();
    flags |= Qt::WindowSoftkeysVisibleHint;
    flags |= Qt::WindowSoftkeysRespondHint;
    w.setWindowFlags(flags);

    w.setLocale(QLocale(QLocale::Polish, QLocale::Poland));

#if defined(Q_WS_S60)
    QWebSettings::globalSettings()->setObjectCacheCapacities(0, 0, 0);
    w.showFullScreen();
#else
    w.show();
#endif

    w.loadSettings();

#ifdef QT_KEYPAD_NAVIGATION
     a.setNavigationMode(Qt::NavigationModeCursorAuto);
#endif

    return a.exec();
}
