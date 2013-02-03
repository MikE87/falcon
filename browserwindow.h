#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QtWebKit>

#include "addressbar.h"
#include "historyview.h"
#include "settingsview.h"

class BrowserWindow : public QWidget
{
    Q_OBJECT

public:
    BrowserWindow(QWidget *parent = 0);
    ~BrowserWindow();

    void addActionToMenu(QAction*);

private:
    QWebView *webView;
    QProgressBar *progressBar;
    AddressBar *addressBar;
    HistoryView *historyView;
    SettingsView *settingsView;
    QVBoxLayout *mainLayout;
    QMenu *menu;
    QAction *optionsAction;
    QAction *back_stopAction;
    QUrl home;
    qreal zoomFactor;
    //qreal textSizeMultipler;
    bool cursorEnabled;
    int sliderValueH;
    int sliderValueV;

    void setupActions();
    void loadSettings();
    void keyPressEvent(QKeyEvent*);

signals:
    void leftSoftKeyPressed();
    void rightSoftKeyPressed();
    void inputAddress();

public slots:
    void openUrl(QUrl);
    void setHomeUrl(QUrl);
    void setJavaScriptEnabled(bool);
    void setLoadImages(bool);
    void setAutoWrapText(bool);
    void goHome();
    void enableCursor();
    void disableCursor();
    void toggleCursor();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    //void increaseTextSize();
    //void decreaseTextSize();
    //void resetTextSize();
    void showHistory();
    void hideHistory();
    void showSettings();
    void hideSettings();

private slots:
    void updateBackStopAction();
    void enterAddress();
    void wrapTextParagraph();

};
#endif // BROWSERWINDOW_H
