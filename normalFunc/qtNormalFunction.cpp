#include "qtNormalFunction.h"

void qt_sleep_ms(unsigned int msec){
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void setWidgetEnabled(QWidget* widget, bool state){
    widget->setEnabled(state);
    widget->setAutoFillBackground(true);
    QPalette pe;
    if(state){
        pe.setColor(QPalette::ButtonText, TEXTCOLOR);
    }
    else{
        pe.setColor(QPalette::ButtonText, BACKGROUNDCOLOR);
    }
    widget->setPalette(pe);
}

void setButtonEnabled(QPushButton* button, bool state){
    if(state){
        button->setStyleSheet(TEXTCOLOR);
    }
    else{
        button->setStyleSheet(BACKGROUNDCOLOR);
    }
}

UpdateStateThread::UpdateStateThread(QWidget* parent){

}

UpdateStateThread::~UpdateStateThread(){
    quit();
    wait();
}

void UpdateStateThread::run(){
    while(1){
        qt_sleep_ms(100);
        emit updateStateSignal();
    }
}
