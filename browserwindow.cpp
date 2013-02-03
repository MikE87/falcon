#include "browserwindow.h"
#include "keycodes.h"



BrowserWindow::BrowserWindow(QWidget *parent) :
    QWidget(parent)
{
    cursorEnabled = true;
    zoomFactor = 1.0;
    //textSizeMultipler = 1.0;
    addressBar = new AddressBar(this);
    progressBar = new QProgressBar(this);
    webView = new QWebView(this);
    historyView = new HistoryView(this, webView->history());
    settingsView = new SettingsView(this);


    progressBar->setFocusPolicy(Qt::NoFocus);
    progressBar->setValue(0);
    //progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setVisible(false);
    historyView->setVisible(false);
    settingsView->setVisible(false);

    mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(addressBar);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(webView);
    mainLayout->addWidget(historyView);
    mainLayout->addWidget(settingsView);

    this->setLayout(mainLayout);
    this->loadSettings();
    this->setupActions();
}

BrowserWindow::~BrowserWindow()
{
    QWebSettings::globalSettings()->clearMemoryCaches();

    delete menu;
    delete optionsAction;
    delete back_stopAction;
    delete webView;
    delete addressBar;
    delete progressBar;
    delete historyView;
    delete settingsView;
    delete mainLayout;
}

void BrowserWindow::openUrl(QUrl url)
{
    webView->load(url);
}

void BrowserWindow::setHomeUrl(QUrl url)
{
    home = addressBar->ParseAddress(url.toString());
}

void BrowserWindow::goHome()
{
    webView->load(home);
}

void BrowserWindow::enableCursor()
{
    QApplication::setNavigationMode(Qt::NavigationModeCursorAuto);
    cursorEnabled = true;
}

void BrowserWindow::disableCursor()
{
    QApplication::setNavigationMode(Qt::NavigationModeKeypadDirectional);
    cursorEnabled = false;
}

void BrowserWindow::toggleCursor()
{
    if(cursorEnabled)
        this->disableCursor();
    else
        this->enableCursor();
}

void BrowserWindow::zoomIn()
{
    zoomFactor += 0.1;
    webView->setZoomFactor(zoomFactor);
}

void BrowserWindow::zoomOut()
{
    zoomFactor -= 0.1;
    webView->setZoomFactor(zoomFactor);
}

void BrowserWindow::resetZoom()
{
    zoomFactor = 1.0;
    webView->setZoomFactor(zoomFactor);
}

/*
void BrowserWindow::increaseTextSize()
{
    textSizeMultipler += 0.1;
    webView->setTextSizeMultiplier(textSizeMultipler);
    webView->settings()->setAttribute(QWebSettings::ZoomTextOnly, false);
}

void BrowserWindow::decreaseTextSize()
{
    textSizeMultipler -= 0.1;
    webView->setTextSizeMultiplier(textSizeMultipler);
    webView->settings()->setAttribute(QWebSettings::ZoomTextOnly, false);
}

void BrowserWindow::resetTextSize()
{
    textSizeMultipler = 1.0;
    webView->setTextSizeMultiplier(textSizeMultipler);;
    webView->settings()->setAttribute(QWebSettings::ZoomTextOnly, false);
}
*/

void BrowserWindow::showHistory()
{
    historyView->showHistory();

    if(cursorEnabled)
        this->disableCursor();

    addressBar->hide();
    progressBar->hide();
    webView->hide();
}

void BrowserWindow::hideHistory()
{
    webView->show();
    webView->setFocus();

    historyView->hide();
}

void BrowserWindow::showSettings()
{
    this->disableCursor();

    settingsView->showSettings();

    addressBar->hide();
    progressBar->hide();
    webView->hide();
}

void BrowserWindow::hideSettings()
{
    webView->show();
    webView->setFocus();

    settingsView->hide();
}

void BrowserWindow::updateBackStopAction()
{
    if(back_stopAction->objectName() == QString("w"))
    {
        back_stopAction->setText(QString("Zatrzymaj"));
        back_stopAction->setObjectName(QString("z"));
        disconnect(back_stopAction, SIGNAL(triggered()), webView, SLOT(back()));
        connect(back_stopAction, SIGNAL(triggered()), webView, SLOT(stop()));
    }
    else
    {
        back_stopAction->setText(QString("Wstecz"));
        back_stopAction->setObjectName(QString("w"));
        disconnect(back_stopAction, SIGNAL(triggered()), webView, SLOT(stop()));
        connect(back_stopAction, SIGNAL(triggered()), webView, SLOT(back()));
    }
}

void BrowserWindow::enterAddress()
{
    emit inputAddress();
}

void BrowserWindow::wrapTextParagraph()
{
    QWebFrame *webFrame = webView->page()->mainFrame();

    QWebElement webElement = webFrame->documentElement();

    QWebElementCollection elements = webElement.findAll(QString("p"));

    foreach (QWebElement element, elements)
    {
        element.setAttribute("style", "width: 300px;");
    }
}

void BrowserWindow::setJavaScriptEnabled(bool enabled)
{
    webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, enabled);
}

void BrowserWindow::setLoadImages(bool enabled)
{
    webView->settings()->setAttribute(QWebSettings::AutoLoadImages, enabled);
}

void BrowserWindow::setAutoWrapText(bool enabled)
{
    if(enabled)
        connect(webView, SIGNAL(loadFinished(bool)), SLOT(wrapTextParagraph()));
    else
        disconnect(webView, SIGNAL(loadFinished(bool)), this, SLOT(wrapTextParagraph()));
}

void BrowserWindow::addActionToMenu(QAction *a)
{
    menu->insertAction(menu->actions().last(), a);
}

void BrowserWindow::setupActions()
{
    menu = new QMenu(this);

    optionsAction = new QAction(tr("Opcje"), this);
    optionsAction->setSoftKeyRole(QAction::PositiveSoftKey);
    optionsAction->setMenu(menu);

    this->addAction(optionsAction);

    menu->addAction(QString("Wprowadz adres"), this,    SLOT(enterAddress()));
    menu->addAction(QString("Odswiez"),        webView, SLOT(reload()));
    menu->addAction(QString("Strona domowa"),  this,    SLOT(goHome()));
    menu->addAction(QString("Historia"),       this,    SLOT(showHistory()));
    menu->addAction(QString("Ustawienia"),     this,    SLOT(showSettings()));
    menu->addAction(QString("Anuluj"),         menu,    SLOT(close()));

    back_stopAction = new QAction(tr("Wstecz"), this);
    back_stopAction->setSoftKeyRole(QAction::NegativeSoftKey);
    back_stopAction->setObjectName(QString("w"));

    this->addAction(back_stopAction);


    connect(menu, SIGNAL(aboutToShow()), SLOT(disableCursor()));
    connect(menu, SIGNAL(aboutToHide()), SLOT(enableCursor()));
    connect(back_stopAction, SIGNAL(triggered()), webView, SLOT(back()));
    connect(settingsView, SIGNAL(aboutToHide()), SLOT(hideSettings()));
    connect(settingsView, SIGNAL(toggledJavaScript(bool)), SLOT(setJavaScriptEnabled(bool)));
    connect(settingsView, SIGNAL(toggledImages(bool)), SLOT(setLoadImages(bool)));
    connect(settingsView, SIGNAL(toggledAutoWrap(bool)), SLOT(setAutoWrapText(bool)));
    connect(settingsView, SIGNAL(changedHomeUrl(QUrl)), SLOT(setHomeUrl(QUrl)));
    connect(historyView, SIGNAL(aboutToHide()), SLOT(hideHistory()));
    connect(historyView, SIGNAL(itemSelected(QUrl)), SLOT(openUrl(QUrl)));
    connect(addressBar, SIGNAL(addressEntered(QUrl)), SLOT(openUrl(QUrl)));
    connect(addressBar, SIGNAL(returnsFocus()), addressBar, SLOT(hide()));
    connect(addressBar, SIGNAL(returnsFocus()), webView, SLOT(setFocus()));
    connect(webView, SIGNAL(urlChanged(QUrl)), addressBar, SLOT(updateUrl(QUrl)));
    connect(webView, SIGNAL(loadProgress(int)), progressBar, SLOT(setValue(int)));
    connect(webView, SIGNAL(loadFinished(bool)),progressBar, SLOT(setHidden(bool)));
    connect(webView, SIGNAL(loadFinished(bool)), SLOT(updateBackStopAction()));
    connect(webView, SIGNAL(loadStarted()), progressBar, SLOT(show()));
    connect(webView, SIGNAL(loadStarted()), SLOT(updateBackStopAction()));
    connect(this, SIGNAL(inputAddress()), addressBar, SLOT(setFocus()));
    connect(this, SIGNAL(inputAddress()), addressBar, SLOT(show()));
    connect(this, SIGNAL(rightSoftKeyPressed()), back_stopAction, SLOT(trigger()));
}

void BrowserWindow::loadSettings()
{
    settingsView->loadSettings();

    webView->settings()->setAttribute(QWebSettings::PluginsEnabled, false);
    webView->settings()->setAttribute(QWebSettings::SpatialNavigationEnabled, false);
    webView->settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, false);
    webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    webView->settings()->setAttribute(QWebSettings::FrameFlatteningEnabled, true);
    webView->settings()->setAttribute(QWebSettings::ZoomTextOnly, false);

    webView->settings()->setLocalStoragePath(QString(QApplication::applicationDirPath() + "/"));
    webView->settings()->setIconDatabasePath(QString(QApplication::applicationDirPath() + "/"));
}

void BrowserWindow::keyPressEvent(QKeyEvent *key)
{
    switch(key->nativeVirtualKey())
    {
    // Copy
    case KEY_CTRL_C:
        webView->page()->triggerAction(QWebPage::Copy);
        break;

    // Cut
    case KEY_CTRL_X:
        webView->page()->triggerAction(QWebPage::Cut);
        break;

    // Paste
    case KEY_CTRL_V:
        webView->page()->triggerAction(QWebPage::Paste);
        break;

    // Add to bookmarks
    case KEY_CTRL_D:
        break;

    // Search
    case KEY_F:
        break;

    // Wrap text
    case KEY_G:
        this->wrapTextParagraph();
        break;

    // Show history
    case KEY_H:
        this->showHistory();
        break;

    // Load home page
    case KEY_I:
        this->goHome();
        break;

    // Zoom out
    case KEY_J:
        this->zoomOut();
        break;

    // Zoom in
    case KEY_U:
        this->zoomIn();
        break;

    // Reset zoom
    case KEY_M:
        this->resetZoom();
        break;

    // Toggle cursor on/off
    case KEY_T:
        this->toggleCursor();
        break;

    // Slide left
    case KEY_A:
        sliderValueH = webView->page()->mainFrame()->scrollBarValue(Qt::Horizontal) - 9;
        webView->page()->mainFrame()->setScrollBarValue(Qt::Horizontal, sliderValueH);
        break;

    // Slide right
    case KEY_D:
        sliderValueH = webView->page()->mainFrame()->scrollBarValue(Qt::Horizontal) + 9;
        webView->page()->mainFrame()->setScrollBarValue(Qt::Horizontal, sliderValueH);
        break;

    // Slide up
    case KEY_W:
        sliderValueV = webView->page()->mainFrame()->scrollBarValue(Qt::Vertical) - 9;
        webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, sliderValueV);
        break;

    // Slide down
    case KEY_S:
        sliderValueV = webView->page()->mainFrame()->scrollBarValue(Qt::Vertical) + 9;
        webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, sliderValueV);
        break;

    // Show menu
    case KEY_LEFT_SOFTKEY:
        menu->exec();
        //emit this->leftSoftKeyPressed();
        break;

    // Cancel / back
    case KEY_RIGHT_SOFTKEY:
        emit rightSoftKeyPressed();
        break;

    // Toggle address bar on/off
    case KEY_DIAL:
        emit inputAddress();
        break;

    // Do nothing :)
    default:
        break;
    }
}
