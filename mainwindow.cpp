#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    fullscreen = false;
    browser = new BrowserWindow(parent);

    screenAction = new QAction(QString("Pelny ekran"), this);
    exitAction = new QAction(QString("Zamknij"), this);
    browser->addActionToMenu(screenAction);
    browser->addActionToMenu(exitAction);

    this->setCentralWidget(browser);
    this->cursor().setPos(100, 100);

    connect(screenAction, SIGNAL(triggered()), SLOT(toggleFullScreen()));
    connect(exitAction,   SIGNAL(triggered()), SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete screenAction;
    delete exitAction;
    delete browser;
}

void MainWindow::loadSettings()
{
    QFile file(QString(QApplication::applicationDirPath() + "/settings.dat"));

    bool javaScriptEnabled;
    bool imagesEnabled;
    bool autoWrapEnabled;
    QString url;

    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in >> javaScriptEnabled >> imagesEnabled >> autoWrapEnabled >> url;
    }
    else
    {// Domyœlne ustawienia
        javaScriptEnabled = true;
        imagesEnabled = true;
        autoWrapEnabled = false;
        url = "www.google.pl";

        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << (bool)javaScriptEnabled;
        out << (bool)imagesEnabled;
        out << (bool)autoWrapEnabled;
        out << (QString)url;
    }

    browser->setJavaScriptEnabled(javaScriptEnabled);
    browser->setLoadImages(imagesEnabled);
    browser->setAutoWrapText(autoWrapEnabled);
    browser->setHomeUrl(QUrl(url));

    browser->goHome();
}

void MainWindow::toggleFullScreen()
{
    Qt::WindowFlags flags = this->windowFlags();

    if(fullscreen)
    {
        flags |= Qt::WindowSoftkeysVisibleHint;
        screenAction->setText(QString("Pelny ekran"));
        fullscreen = false;
    }
    else
    {
        flags &= ~Qt::WindowSoftkeysVisibleHint;
        screenAction->setText(QString("Normalny ekran"));
        fullscreen = true;
    }

    this->setWindowFlags(flags);
    this->showFullScreen();
}





