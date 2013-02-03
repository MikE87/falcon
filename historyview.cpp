#include "historyview.h"
#include "keycodes.h"

#include <QAction>
#include <QKeyEvent>
#include <QScrollBar>
#include <QLabel>


HistoryView::HistoryView(QWidget *parent, QWebHistory *webHistory) :
    QWidget(parent)
{
    history = webHistory;
    mainLayaut = new QVBoxLayout;
    historyList = new QListWidget(this);

    historyList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    historyList->setAutoScroll(true);
    historyList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    historyList->setSpacing(2);

    QLabel *title = new QLabel(QString("Historia odwiedzanych stron"), this);
    title->setFocusPolicy(Qt::NoFocus);
    title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    title->setFixedHeight(20);
    QFont titleFont;
    titleFont.setPointSize(8);
    title->setFont(titleFont);

    mainLayaut->setSpacing(0);
    mainLayaut->setMargin(0);
    mainLayaut->addWidget(title);
    mainLayaut->addWidget(historyList);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setLayout(mainLayaut);
    this->setupActions();

}

HistoryView::~HistoryView()
{
    delete historyList;
    delete mainLayaut;
}

void HistoryView::setupActions()
{
    QAction *selectAction = new QAction(QString("Wybierz"), this);
    selectAction->setSoftKeyRole(QAction::PositiveSoftKey);
    this->addAction(selectAction);

    QAction *backAction = new QAction(QString("Wstecz"), this);
    backAction->setSoftKeyRole(QAction::NegativeSoftKey);
    this->addAction(backAction);


    connect(selectAction, SIGNAL(triggered()), SLOT(selectItem()));
    connect(backAction, SIGNAL(triggered()), SLOT(goBack()));
    connect(historyList, SIGNAL(itemActivated(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));
    connect(this, SIGNAL(itemSelected(QUrl)), SLOT(goBack()));
    connect(this, SIGNAL(rightSoftKeyPressed()), SLOT(goBack()));
    connect(this, SIGNAL(leftSoftKeyPressed()), SLOT(selectItem()));
}

void HistoryView::loadList()
{
    foreach(QWebHistoryItem historyItem, history->items())
    {
        QListWidgetItem *listItem = new QListWidgetItem(historyItem.icon(), historyItem.title() +
                                                        "\n" + historyItem.url().toString());
        historyList->insertItem(0, listItem);
    }
}

void HistoryView::selectItem(QListWidgetItem *item)
{
    QStringList url;
    url = item->text().split("\n");
    emit itemSelected(QUrl(url.at(1)));
}

void HistoryView::selectItem()
{
    if(historyList->count() != 0)
        this->selectItem(historyList->currentItem());
}

void HistoryView::goBack()
{
    emit aboutToHide();
    historyList->clear();
}

void HistoryView::showHistory()
{
    this->loadList();
    this->show();
    historyList->setFocus();
}

void HistoryView::keyPressEvent(QKeyEvent *key)
{
    switch(key->nativeVirtualKey())
    {
    case KEY_LEFT_ARROW:
        sliderValueH = historyList->horizontalScrollBar()->value() - 50;
        historyList->horizontalScrollBar()->setValue(sliderValueH);
        break;

    case KEY_RIGHT_ARROW:
        sliderValueH = historyList->horizontalScrollBar()->value() + 50;
        historyList->horizontalScrollBar()->setValue(sliderValueH);
        break;

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
