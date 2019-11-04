#include "pushButton.h"
#include <QDebug>

bool SOUND;

PushButton::PushButton(QWidget *parent){
    this->setFocusPolicy(Qt::ClickFocus);
}

PushButton::PushButton(const QString &text, QWidget *parent){
    this->setAutoFillBackground(true);
    this->setText(text);
    this->setIndex(0);
}

PushButton::~PushButton(){

}

void PushButton::setIndex(int index){
    m_index = index;
    connectSignalSlot();
}

void PushButton::connectSignalSlot(){
    QObject::connect(this, SIGNAL(pressed()), this, SLOT(pushButtonSound()));
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
    QObject::connect(this, SIGNAL(pressed()), this, SLOT(pushButtonPressed()));
    QObject::connect(this, SIGNAL(released()), this, SLOT(pushButtonReleased()));
}

void PushButton::pushButtonSound(){
    if(SOUND){
       rclib::RobotClient::initance()->sendBEEP(0.05);
    }
}

void PushButton::pushButtonClicked(){
    emit pushButtonClicked(m_index);
}

void PushButton::pushButtonPressed(){
    emit pushButtonPressed(m_index);
}

void PushButton::pushButtonReleased(){
    emit pushButtonReleased(m_index);
}

void PushButton::setEnable(bool state){
    setWidgetEnabled(this, state);
}

void PushButton::setStateSum(int sum){
    m_sum = sum;
}

void PushButton::setState(int state){
    m_state = state;
    m_state = m_state % m_sum;
    if(m_state < 0){
        m_state = m_sum-1;
    }
}

void PushButton::incState(){
    m_state++;
    m_state = m_state % m_sum;
}

void PushButton::decState(){
    m_state--;
    if(m_state < 0){
        m_state = m_sum-1;
    }
}

int PushButton::returnState(){
   return m_state;
}
