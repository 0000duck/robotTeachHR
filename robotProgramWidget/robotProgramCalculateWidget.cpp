#include "robotProgramCalculateWidget.h"
#include <QDebug>

using namespace std;
using namespace rclib;
using namespace Common;

extern bool tag_ADD;

RobotProgramCalculateWidget::RobotProgramCalculateWidget(QWidget *parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramCalculateWidget::~RobotProgramCalculateWidget(){

}

void RobotProgramCalculateWidget::passParameter(rclib::RobotInterpreter* p_file){
    m_fileSrc = p_file;
}

void RobotProgramCalculateWidget::setMode(OPERATIONMODE mode){
    m_mode = mode;

    updateComboList();

    vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();
    VECTOR_STRING tmp_Strentence=Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");

    if(m_mode == TEACH_MODIFY){
        int tmp_count;
        QString tmp_Qstring;

        if(find(m_comboVariableName,tmp_Strentence[1]) == -1){
            tmp_count = m_comboVariableName->count();
            tmp_Qstring = QString::fromStdString(tmp_Strentence[1]);
            m_comboVariableName->insertItem(tmp_count,tmp_Qstring);
            m_comboVariableName->setCurrentIndex(tmp_count);
        }else{
            m_comboVariableName->setCurrentIndex(find(m_comboVariableName,tmp_Strentence[1]));
        }

        if(find(m_comboVariableLeft,tmp_Strentence[3]) == -1){
            tmp_count = m_comboVariableLeft->count();
            tmp_Qstring = QString::fromStdString(tmp_Strentence[3]);
            m_comboVariableLeft->insertItem(tmp_count,tmp_Qstring);
            m_comboVariableLeft->setCurrentIndex(tmp_count);
        }else{
            m_comboVariableLeft->setCurrentIndex(find(m_comboVariableLeft,tmp_Strentence[3]));
        }

        if(tmp_Strentence.size() > 2){
            int index = 0;
            if(tmp_Strentence[4] == string("+")){
                index = 1;
            }
            else if(tmp_Strentence[4] == string("-")){
                index = 2;
            }
            else if(tmp_Strentence[4] == string("*")){
                index = 3;
            }
            else if(tmp_Strentence[4] == string("/")){
                index = 4;
            }
            else if(tmp_Strentence[4] == string("%")){
                index = 5;
            }

            m_comboOperator->setCurrentIndex(index);

            if(index != 0){
                if(find(m_comboVariableRight,tmp_Strentence[5]) == -1){
                    tmp_count = m_comboVariableRight->count();
                    tmp_Qstring = QString::fromStdString(tmp_Strentence[5]);
                    m_comboVariableRight->insertItem(tmp_count,tmp_Qstring);
                    m_comboVariableRight->setCurrentIndex(tmp_count);
                }else{
                    m_comboVariableRight->setCurrentIndex(find(m_comboVariableRight,tmp_Strentence[5]));
                }
                m_comboVariableRight->show();
            }
        }
    }
    else{
        clearState();
    }
}

void RobotProgramCalculateWidget::passSentenceIndex(int func, int line){
    m_func = func;
    m_line = line;
}

void RobotProgramCalculateWidget::initUI(){
    m_comboVariableName =new QComboBox();
    m_comboVariableName->setStyleSheet("QComboBox QAbstractItemView{"
                                           "font-size: 20px;"
                                       "}");
    m_comboVariableName->setView(new  QListView());
    m_comboVariableName->setEditable(true);
    m_comboVariableName->setMaxVisibleItems(7);
    m_comboVariableName->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_comboVariableLeft =new QComboBox();
    m_comboVariableLeft->setStyleSheet("QComboBox QAbstractItemView{"
                                           "font-size: 18px;"
                                       "}");
    m_comboVariableLeft->setView(new  QListView());
    m_comboVariableLeft->setEditable(true);
    m_comboVariableLeft->setMaxVisibleItems(7);
    m_comboVariableLeft->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_comboVariableRight =new QComboBox();
    m_comboVariableRight->setStyleSheet("QComboBox QAbstractItemView{"
                                           "font-size: 18px;"
                                       "}");
    m_comboVariableRight->setView(new  QListView());
    m_comboVariableRight->setEditable(true);
    m_comboVariableRight->setMaxVisibleItems(7);
    m_comboVariableRight->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QLabel* labelVariableEqual = new QLabel(" = ");
    m_comboOperator = new ComboBox();
    m_comboOperator->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutFormula = new QHBoxLayout();
    layoutFormula->setObjectName("group");
    layoutFormula->addWidget(m_comboVariableName,3);
    layoutFormula->addWidget(labelVariableEqual,1,Qt::AlignCenter);
    layoutFormula->addWidget(m_comboVariableLeft,3);
    layoutFormula->addWidget(m_comboOperator,3);
    layoutFormula->addWidget(m_comboVariableRight,3);
    QGroupBox* groupFormula = new QGroupBox(tr("计算"));
    groupFormula->setLayout(layoutFormula);

    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    m_buttonYes->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutSure = new QVBoxLayout();
    layoutSure->addWidget(m_buttonYes);
    layoutSure->addWidget(m_buttonNo);
    QGroupBox* groupSure = new QGroupBox();
    groupSure->setLayout(layoutSure);

    QVBoxLayout* layoutCalculate = new QVBoxLayout(this);
    layoutCalculate->addWidget(groupFormula);
    layoutCalculate->addWidget(groupSure, 0, Qt::AlignRight);
    layoutCalculate->setStretchFactor(groupFormula, 10);
    layoutCalculate->setStretchFactor(groupSure, 1);
}

void RobotProgramCalculateWidget::initState(){
    m_comboOperator->insertItem(0, QString("空"));
    m_comboOperator->insertItem(1, QString("+"));
    m_comboOperator->insertItem(2, QString("-"));
    m_comboOperator->insertItem(3, QString("*"));
    m_comboOperator->insertItem(4, QString("/"));
    m_comboOperator->insertItem(5, QString("%"));

    clearState();
}

void RobotProgramCalculateWidget::connectSignalSlot(){
    QObject::connect(m_comboOperator, SIGNAL(activated(int)), this, SLOT(comboOpeatorChanged(int)));

    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));
}

void RobotProgramCalculateWidget::clearState(){
    m_comboOperator->setCurrentIndex(0);
    comboOpeatorChanged(0);
}

bool RobotProgramCalculateWidget::grammarCheck(){
    if(m_comboVariableName->currentText().toStdString() == "" || m_comboVariableLeft->currentText().toStdString() == ""){
        QMessageBox::question(this, tr("提示"),
                              tr("等式未填写完整!"),
                              QMessageBox::Yes);
        return false;
    }

    if(m_comboOperator->currentIndex()){
        if(m_comboVariableRight->currentText().toStdString() == ""){
            QMessageBox::question(this, tr("提示"),
                                  tr("等式未填写完整!"),
                                  QMessageBox::Yes);
            return false;
        }
    }

    return true;
}

void RobotProgramCalculateWidget::updateComboList()
{
    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType= m_fileSrc->getVariableMap();
    map<string, PROGRAM_KEY_WORD>::iterator it_varType = tmp_VarType.begin();

    QString tmp;

    m_comboVariableName->clear();
    m_comboVariableLeft->clear();
    m_comboVariableRight->clear();

    if(tmp_VarType.size() == 0){
        tmp = QString("");
        m_comboVariableName->insertItem(0,tmp);
        m_comboVariableLeft->insertItem(0,tmp);
        m_comboVariableRight->insertItem(0,tmp);
    }

    int i = 0;
    while(it_varType != tmp_VarType.end()){
        string variableName = it_varType->first;
        tmp = QString::fromStdString(variableName);

        m_comboVariableName->insertItem(i,tmp);
        m_comboVariableLeft->insertItem(i,tmp);
        m_comboVariableRight->insertItem(i,tmp);
        it_varType++;
        i++;
    }

    m_comboVariableName->setCurrentIndex(0);
    m_comboVariableLeft->setCurrentIndex(0);
    m_comboVariableRight->setCurrentIndex(0);
}

int RobotProgramCalculateWidget::find(QComboBox *combox, string tmp)
{
    int count = combox->count();
    int i = 0;
    for(;i < count;i++){
        if(tmp == combox->itemText(i).toStdString())
            return i;
    }
    if(i >= combox->count())
        return -1;
}

void RobotProgramCalculateWidget::buttonYesClicked(){
    if(!grammarCheck()){
        return;
    }

    std::string tempOperator[5] = {"+", "-", "*", "/", "%"};

    string tempSentence;
    tempSentence=tempSentence+"CALCU"+" "+m_comboVariableName->currentText().toStdString()+" "+"="+" "+m_comboVariableLeft->currentText().toStdString();

    if(m_comboOperator->currentIndex()){      
        tempSentence=tempSentence+" "+tempOperator[m_comboOperator->currentIndex()-1]+" "+m_comboVariableRight->currentText().toStdString();
    }

    ProgramSentence tmp_Sentence;
    tmp_Sentence.strSentence=tempSentence;

    ProgramPointer tmp_Pointer;
    tmp_Pointer.nFunction=m_func;
    tmp_Pointer.nSentence=m_line;

    if(m_mode == TEACH_MODIFY){
        m_fileSrc->modifyProgramSentence(tmp_Pointer,tmp_Sentence);
        tag_ADD = false;
    }
    else{
        tmp_Pointer.nSentence=m_line+1;
        m_fileSrc->addProgramSentence(tmp_Pointer,tmp_Sentence);
        tag_ADD = true;
    }

    emit programOperationFinishedSignal(true);
}

void RobotProgramCalculateWidget::buttonNoClicked(){
    emit programOperationFinishedSignal(false);
}

void RobotProgramCalculateWidget::comboOpeatorChanged(int index){
    if(m_comboOperator->currentIndex() == 0){
        m_comboVariableRight->hide();
    }
    else{
        m_comboVariableRight->show();
    }
}
