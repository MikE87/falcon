#include "addressbar.h"
#include "keycodes.h"


AddressBar::AddressBar(QWidget *parent) :
    QWidget(parent)
{
    address = new QLineEdit(this);
    mainLayout = new QHBoxLayout;

    QFont urlFont;
    urlFont.setPointSize(6);

    address->setFont(urlFont);
    address->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    address->setFixedHeight(16);

    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(address);

    this->setLayout(mainLayout);
    this->setupActions();

    connect(address, SIGNAL(returnPressed()), SLOT(enterAddress()));
}

AddressBar::~AddressBar()
{
    delete mainLayout;
    delete address;
    delete clearAction;
    delete gotoAction;
}

void AddressBar::enterAddress()
{
    emit addressEntered(ParseAddress(address->text()));
}

void AddressBar::updateUrl(QUrl url)
{
    address->setText(url.toString());
}

void AddressBar::setFocus()
{
    address->setFocus();
    address->selectAll();
}

QUrl AddressBar::ParseAddress(QString urlAddress)
{
    if(!urlAddress.isEmpty())
    {
        bool match = true;
        QString pat = "http://";
        int i = 0;

        while((i<7)&&match)
        {
            if(urlAddress.at(i) != pat.at(i))
                match = false;
            ++i;
        }

        if(match)
        {
            return QUrl(urlAddress);
        }
        else
        {
            if(urlAddress.contains('.'))
            {
                pat.append(urlAddress);
                return QUrl(pat);
            }
            else
            {
                return QUrl(QString("http://www.google.com/search?q=") + urlAddress);
            }
        }
    }
    else
    {
        return QUrl("http://www.google.pl");
    }
}

void AddressBar::setupActions()
{
    clearAction = new QAction(QString("Wyczysc"), this);
    clearAction->setSoftKeyRole(QAction::NegativeSoftKey);
    connect(clearAction, SIGNAL(triggered()), address, SLOT(clear()));
    connect(this, SIGNAL(rightSoftKeyPressed()), clearAction, SLOT(trigger()));
    this->addAction(clearAction);

    gotoAction = new QAction(QString("Idz do"), this);
    gotoAction->setSoftKeyRole(QAction::PositiveSoftKey);
    connect(gotoAction, SIGNAL(triggered()), SLOT(enterAddress()));
    connect(this, SIGNAL(leftSoftKeyPressed()), gotoAction, SLOT(trigger()));
    this->addAction(gotoAction);
}

void AddressBar::keyPressEvent(QKeyEvent *key)
{
    switch(key->nativeVirtualKey())
    {
    case KEY_CTRL_A:
        address->selectAll();
        break;

    case KEY_CTRL_C:
        address->copy();
        break;

    case KEY_CTRL_V:
        address->paste();
        break;

    case KEY_CTRL_X:
        address->cut();
        break;

    case KEY_LEFT_SOFTKEY:
        emit leftSoftKeyPressed();
        break;

    case KEY_RIGHT_SOFTKEY:
        emit rightSoftKeyPressed();
        break;

    case KEY_DIAL:
        emit returnsFocus();
        break;

    default:
        break;
    }
}
