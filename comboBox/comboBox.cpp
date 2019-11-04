#include "comboBox.h"

using namespace std;

ComboBox::ComboBox(QWidget *parent){
    m_menu = new QMenu(this);
    m_menu->setStyleSheet("QMenu::item{color: rgb(0,0,0); height: 30px; margin: 10px;}");
    this->setMenu(m_menu);

    m_index = 0;
}

ComboBox::~ComboBox(){

}

void ComboBox::insertItem(int index, QString text){
    for(int i=m_action.size(); i<=index; i++){
        Action* action = new Action(m_menu);
        action->setIndex(i);
        m_action.push_back(action);
        m_menu->addAction(m_action[i]);
        m_menu->addSeparator();
        QObject::connect(m_action[i], SIGNAL(triggeredIndex(int)), this, SLOT(actionSlot(int)));
    }
    m_action[index]->setText(text);
    m_index = 0;
    this->setText(m_action[0]->text());
}

void ComboBox::clear(){
    for(int i=0; i<m_action.size(); i++){
        QObject::disconnect(m_action[i], SIGNAL(triggeredIndex(int)), this, SLOT(actionSlot(int)));
        m_menu->removeAction(m_action[i]);
        delete m_action[i];
    }
    m_action.clear();
}

int ComboBox::currentIndex(){
    return m_index;
}

void ComboBox::setCurrentIndex(int index){
    m_index = index;
    this->setText(m_action[m_index]->text());
}

QString ComboBox::currentText(){
    if(m_action.size() == 0){
        return QString("");
    }
    QString temp=m_action[m_index]->text().section("\n",0,0);
    return temp;
}

void ComboBox::setCurrentText(QString text){
    for(int i=0; i<m_action.size(); i++){
        if(text == m_action[i]->text()){
            m_index = i;
            this->setText(text);
            break;
        }
    }
}

void ComboBox::setEnable(bool state){
    setWidgetEnabled(this, state);
}

void ComboBox::actionSlot(int index){
    m_index = index;
    this->setText(m_action[m_index]->text());
    emit activated(m_index);
}
