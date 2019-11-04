#include "robotProgramVisionWidget.h"

#include <QDebug>

using namespace std;
using namespace rclib;

extern bool tag_ADD;

RobotProgramVisionWidget::RobotProgramVisionWidget(QWidget* parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramVisionWidget::~RobotProgramVisionWidget(){

}

void RobotProgramVisionWidget::passParameter(RobotInterpreter* p_file){
    m_fileSrc = p_file;
}

void RobotProgramVisionWidget::setMode(OPERATIONMODE mode){
    m_mode = mode;

    if(m_mode == TEACH_MODIFY){
        std::vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();
        VECTOR_STRING tmp_FuncStrentence=Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");
        string name=tmp_FuncStrentence[1];
        m_editVariableName->setText(QString::fromStdString(name));
    }
    else{
        clearState();
    }
}

void RobotProgramVisionWidget::passSentenceIndex(int func, int line){
    m_func = func;
    m_line = line;
}

void RobotProgramVisionWidget::initUI(){
    m_radioVisionLocate = new QRadioButton(tr("视觉定位"));

    QHBoxLayout* layoutVisionType = new QHBoxLayout();
    layoutVisionType->addWidget(m_radioVisionLocate);

    QGroupBox* groupVisionType = new QGroupBox(tr("视觉指令类型"));
    groupVisionType->setLayout(layoutVisionType);

    QVBoxLayout* layoutButtonVariableName = new QVBoxLayout();
    for(int i=0; i<3; i++){
        m_buttonVariableName[i] = new PushButton();
        m_buttonVariableName[i]->setIndex(i);
        m_buttonVariableName[i]->setText(QString("VPos")+QString::number(i+1, 10));
        m_buttonVariableName[i]->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
        layoutButtonVariableName->addWidget(m_buttonVariableName[i]);
    }

    m_labelVariableName = new QLabel(tr("视觉定位变量:"));
    m_editVariableName = new QLineEdit();
    m_editVariableName->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editVariableName->setReadOnly(true);

    QHBoxLayout* layoutVariableName = new QHBoxLayout();
    layoutVariableName->addWidget(m_labelVariableName);
    layoutVariableName->addWidget(m_editVariableName);
    layoutVariableName->addLayout(layoutButtonVariableName);

    QGroupBox* groupVariableName = new QGroupBox();
    groupVariableName->setLayout(layoutVariableName);

    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    m_buttonYes->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutSure = new QVBoxLayout();
    layoutSure->addWidget(m_buttonYes);
    layoutSure->addWidget(m_buttonNo);
    QGroupBox* groupSure = new QGroupBox();
    groupSure->setLayout(layoutSure);

    QVBoxLayout* layoutSubFunc = new QVBoxLayout(this);
    layoutSubFunc->addWidget(groupVisionType);
    layoutSubFunc->addWidget(groupVariableName);
    layoutSubFunc->addWidget(groupSure, 0, Qt::AlignRight);
    layoutSubFunc->setStretchFactor(groupVariableName, 10);
    layoutSubFunc->setStretchFactor(groupSure, 1);
}

void RobotProgramVisionWidget::initState(){
    clearState();
}

void RobotProgramVisionWidget::connectSignalSlot(){
    for(int i=0; i<3; i++){
        QObject::connect(m_buttonVariableName[i], SIGNAL(pushButtonClicked(int)), this, SLOT(buttonVariableNameClicked(int)));
    }

    QObject::connect(m_radioVisionLocate, SIGNAL(toggled(bool)), this, SLOT(radioVisionTypeChanged()));

    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));
}

void RobotProgramVisionWidget::clearState(){
    m_editVariableName->clear();
}

bool RobotProgramVisionWidget::grammarCheck(){
    if(m_editVariableName->text().isEmpty()){
        QMessageBox::question(this, tr("提示"),
                              tr("视觉定位变量名不能为空!"),
                              QMessageBox::Yes);
        return false;
    }

    return true;
}

void RobotProgramVisionWidget::buttonVariableNameClicked(int index){
    m_editVariableName->setText(m_buttonVariableName[index]->text());
}

void RobotProgramVisionWidget::buttonYesClicked(){
    if(!grammarCheck()){
        return;
    }

    string tmp_strentence;
    string tmp_name=m_editVariableName->text().toStdString();

    ProgramPointer tmp_pointer;
    tmp_pointer.nFunction=m_func;
    tmp_pointer.nSentence=m_line;
    tmp_strentence=tmp_strentence+"VLOCATE" + " "+tmp_name;

    ProgramSentence m_strentence;
    m_strentence.strSentence=tmp_strentence;

    if(m_mode == TEACH_MODIFY){
        m_fileSrc->modifyProgramSentence(tmp_pointer,m_strentence);
        tag_ADD = false;
    }
    else{
        tmp_pointer.nSentence=m_line+1;
        m_fileSrc->addProgramSentence(tmp_pointer,m_strentence);
        tag_ADD = true;
    }

    emit programOperationFinishedSignal(true);
}

void RobotProgramVisionWidget::buttonNoClicked(){
    emit programOperationFinishedSignal(false);
}

void RobotProgramVisionWidget::radioVisionTypeChanged(){

}
