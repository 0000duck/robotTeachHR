#ifndef QTNORMALFUNCTION_H
#define QTNORMALFUNCTION_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QThread>

#include "macroDefine.h"

class QWidget;
class QThread;
class UpdateStateThread : public QThread{
    Q_OBJECT
public:
    UpdateStateThread(QWidget* parent = NULL);
    ~UpdateStateThread();

protected:
    virtual void run();

signals:
    void updateStateSignal();
};

#include <QCoreApplication>
#include <QDateTime>
#include <QEventLoop>

class QCoreApplication;
class QDateTime;
class QEventLoop;
void qt_sleep_ms(unsigned int msec);

class QWidget;
class QPushButton;
void setWidgetEnabled(QWidget* widget, bool state);
void setButtonEnabled(QPushButton* button, bool state);

#endif
