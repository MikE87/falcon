#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "browserwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadSettings();

private:
    BrowserWindow *browser;
    QAction *screenAction;
    QAction *exitAction;

    bool fullscreen;

signals:


public slots:
    void toggleFullScreen();
};

#endif // MAINWINDOW_H
