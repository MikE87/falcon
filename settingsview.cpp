#include "settingsview.h"
#include "keycodes.h"

#include <QFile>
#include <QDataStream>


SettingsView::SettingsView(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QVBoxLayout;
    enableJavaScript = new QCheckBox(this);
    loadImages = new QCheckBox(this);
    autoWrap = new QCheckBox(this);
    homeUrl = new QLineEdit(this);

    setupLayout();
    setupActions();
    loadSettings();

}

SettingsView::~SettingsView()
{
    delete mainLayout;
    delete enableJavaScript;
    delete loadImages;
    delete autoWrap;
    delete homeUrl;
}

void SettingsView::setupLayout()
{
    QLabel *title = new QLabel(QString("Ustawienia aplikacji"), this);
    title->setFocusPolicy(Qt::NoFocus);
    title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    title->setFixedHeight(25);

    QFont titleFont;
    titleFont.setPointSize(10);
    title->setFont(titleFont);

    QFont contentFont;
    contentFont.setPointSize(8);

    QLabel *homeUrlLabel = new QLabel(QString("Strona startowa:"), this);
    homeUrlLabel->setFocusPolicy(Qt::NoFocus);
    homeUrlLabel->setFont(contentFont);

    homeUrl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    enableJavaScript->setFont(contentFont);
    enableJavaScript->setText(QString("JavaScript"));
    enableJavaScript->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    loadImages->setFont(contentFont);
    loadImages->setText(QString("Wczytuj obrazki"));
    loadImages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    autoWrap->setFont(contentFont);
    autoWrap->setText(QString("Automatycznie dopasuj tekst na\n kazdej stronie"));
    autoWrap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->setSpacing(5);
    mainLayout->setMargin(5);
    mainLayout->addWidget(title);
    mainLayout->addWidget(homeUrlLabel);
    mainLayout->addWidget(homeUrl);
    mainLayout->addWidget(enableJavaScript);
    mainLayout->addWidget(loadImages);
    mainLayout->addWidget(autoWrap);

    this->setLayout(mainLayout);
}

void SettingsView::setupActions()
{
    QAction *backAction = new QAction(QString("Wstecz"), this);
    backAction->setSoftKeyRole(QAction::NegativeSoftKey);
    this->addAction(backAction);

    connect(backAction,       SIGNAL(triggered()),           SLOT(goBack()));
    connect(this,             SIGNAL(rightSoftKeyPressed()), SLOT(goBack()));
    connect(enableJavaScript, SIGNAL(stateChanged(int)),     SLOT(setJavaScript(int)));
    connect(loadImages,       SIGNAL(stateChanged(int)),     SLOT(setImages(int)));
    connect(autoWrap,         SIGNAL(stateChanged(int)),     SLOT(setAutoWrap(int)));
}

void SettingsView::saveSettings()
{
    QFile file(QString(QApplication::applicationDirPath() + "/settings.dat"));

    file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << (bool)enableJavaScript->isChecked();
        out << (bool)loadImages->isChecked();
        out << (bool)autoWrap->isChecked();
        out << (QString)homeUrl->text();

    file.close();
}

void SettingsView::goBack()
{
    emit aboutToHide();
    emit changedHomeUrl(QUrl(homeUrl->text()));

    this->saveSettings();
}

void SettingsView::showSettings()
{
    this->show();
    homeUrl->setFocus();
}

void SettingsView::setJavaScript(int state)
{
    switch(state)
    {
    case 0:
        emit toggledJavaScript(false);
        break;

    case 2:
        emit toggledJavaScript(true);
        break;

    default:
        break;
    }
}

void SettingsView::setImages(int state)
{
    switch(state)
    {
    case 0:
        emit toggledImages(false);
        break;

    case 2:
        emit toggledImages(true);
        break;

    default:
        break;
    }
}

void SettingsView::setAutoWrap(int state)
{
    switch(state)
    {
    case 0:
        emit toggledAutoWrap(false);
        break;

    case 2:
        emit toggledAutoWrap(true);
        break;

    default:
        break;
    }
}

void SettingsView::loadSettings()
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

    enableJavaScript->setChecked(javaScriptEnabled);
    loadImages->setChecked(imagesEnabled);
    autoWrap->setChecked(autoWrapEnabled);
    homeUrl->setText(url);
/*
    emit toggledJavaScript(javaScriptEnabled);
    emit toggledImages(imagesEnabled);
    emit toggledAutoWrap(autoWrapEnabled);
    emit changedHomeUrl(QUrl(url));
*/
    file.close();
}

void SettingsView::keyPressEvent(QKeyEvent *key)
{
    switch(key->nativeVirtualKey())
    {
    case KEY_LEFT_SOFTKEY:
        emit leftSoftKeyPressed();
        break;

    case KEY_RIGHT_SOFTKEY:
        emit rightSoftKeyPressed();
        break;

    default:
        break;
    }
}
