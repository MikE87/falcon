#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QWebHistory>


class HistoryView : public QWidget
{
    Q_OBJECT

public:
    HistoryView(QWidget *parent = 0, QWebHistory* webHistory = 0);
    ~HistoryView();

private:
    QWebHistory *history;
    QListWidget *historyList;
    QVBoxLayout *mainLayaut;

    int sliderValueH;

    void keyPressEvent(QKeyEvent*);
    void setupActions();
    void loadList();

signals:
    void leftSoftKeyPressed();
    void rightSoftKeyPressed();
    void itemSelected(QUrl);
    void aboutToHide();

private slots:
    void selectItem(QListWidgetItem*);
    void selectItem();
    void goBack();

public slots:
    void showHistory();

};

#endif // HISTORYVIEW_H
