#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QtGui>
#include <QUrl>

class AddressBar : public QWidget
{
    Q_OBJECT

public:
    AddressBar(QWidget *parent = 0);
    ~AddressBar();

    QUrl ParseAddress(QString);

private:
    QLineEdit *address;
    QHBoxLayout *mainLayout;
    QAction *clearAction;
    QAction *gotoAction;

    void setupActions();
    void keyPressEvent(QKeyEvent*);

signals:
    void addressEntered(QUrl);
    void returnsFocus();
    void leftSoftKeyPressed();
    void rightSoftKeyPressed();

public slots:
    void enterAddress();
    void updateUrl(QUrl);
    void setFocus();
};
#endif // ADDRESSBAR_H
