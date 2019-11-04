#include "action.h"

Action::Action(QWidget *parent){
    connectSignalSlot();
}

Action::~Action(){

}

void Action::setIndex(int index){
    m_index = index;
}

void Action::connectSignalSlot(){
    QObject::connect(this, SIGNAL(triggered()), this, SLOT(actionSlot()));
}

void Action::actionSlot(){
    emit triggeredIndex(m_index);
}
