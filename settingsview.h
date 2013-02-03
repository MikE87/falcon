#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <QtGui>

class SettingsView : public QWidget
{
    Q_OBJECT

public:
    SettingsView(QWidget *parent = 0);
    ~SettingsView();

    void loadSettings();

private:
    QVBoxLayout *mainLayout;
    QCheckBox *enableJavaScript;
    QCheckBox *loadImages;
    QCheckBox *autoWrap;
    QLineEdit *homeUrl;

    void setupLayout();
    void setupActions();
    void saveSettings();
    void keyPressEvent(QKeyEvent *);

signals:
    void leftSoftKeyPressed();
    void rightSoftKeyPressed();
    void aboutToHide();
    void toggledJavaScript(bool);
    void toggledImages(bool);
    void toggledAutoWrap(bool);
    void changedHomeUrl(QUrl);

private slots:
    void goBack();
    void setJavaScript(int);
    void setImages(int);
    void setAutoWrap(int);

public slots:
    void showSettings();

};

#endif // SETTINGSVIEW_H
