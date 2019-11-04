#include "robotProgramSubFuncWidget.h"

#include <QDebug>

using namespace std;
using namespace rclib;

extern bool tag_ADD;

RobotProgramSubFuncWidget::RobotProgramSubFuncWidget(QWidget* parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramSubFuncWidget::~RobotProgramSubFuncWidget(){

}

void RobotProgramSubFuncWidget::passParameter(RobotInterpreter* p_file){
    m_fileSrc = p_file;
}

void RobotProgramSubFuncWidget::setMode(OPERATIONMODE mode){
    m_mode = mode;

    vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();

    if(m_mode == TEACH_MODIFY){
        VECTOR_STRING tmp_Strentence=Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");
        m_editFuncName->setText(QString::fromStdString(tmp_Strentence[1]));
    }
    else{
        clearState();
    }
}

void RobotProgramSubFuncWidget::passSentenceIndex(int func, int line){
    m_func = func;
    m_line = line;
}

void RobotProgramSubFuncWidget::initUI(){
    QLabel* labelFuncName = new QLabel(tr("子函数名:"));
    m_editFuncName = new QLineEdit();
    m_editFuncName->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutFuncName = new QHBoxLayout();
    layoutFuncName->addWidget(labelFuncName);
    layoutFuncName->addWidget(m_editFuncName);
    QGroupBox* groupFuncName = new QGroupBox();
    groupFuncName->setLayout(layoutFuncName);

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
    layoutSubFunc->addWidget(groupFuncName);
    layoutSubFunc->addWidget(groupSure, 0, Qt::AlignRight);
    layoutSubFunc->setStretchFactor(groupFuncName, 10);
    layoutSubFunc->setStretchFactor(groupSure, 1);
}

void RobotProgramSubFuncWidget::initState(){
    clearState();
}

void RobotProgramSubFuncWidget::connectSignalSlot(){
    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));
}

void RobotProgramSubFuncWidget::clearState(){
    m_editFuncName->clear();
}

bool RobotProgramSubFuncWidget::grammarCheck(){
    if(m_editFuncName->text().isEmpty()){
        QMessageBox::question(this, tr("提示"),
                              tr("子函数名不能为空!"),
                              QMessageBox::Yes);
        return false;
    }

    if(m_editFuncName->text().indexOf(QRegExp("^[A-Za-z_][A-Za-z0-9_]*$")) == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("子函数名格式错误!"),
                              QMessageBox::Yes);
        return false;
    }

    vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();

    string name = m_editFuncName->text().toStdString();
    for(int i=1; i<tmp_FuncTable.size(); i++){
        VECTOR_STRING tmp_Strentence=Common::ToVectors(tmp_FuncTable[i][0].strSentence," ");
        if(name == tmp_Strentence[1]){
            QMessageBox::question(this, tr("提示"),
                                  tr("该子函数已存在!"),
                                  QMessageBox::Yes);
            return false;
        }
    }

    return true;
}

void RobotProgramSubFuncWidget::buttonYesClicked(){
    if(!grammarCheck()){
        return;
    }

    string tempSentence;
    tempSentence=tempSentence+"FUNC"+" "+m_editFuncName->text().toStdString();

    ProgramPointer tmp_Pointer;
    tmp_Pointer.nFunction=m_func;
    tmp_Pointer.nSentence=m_line;

    ProgramSentence tmp_strentence;
    tmp_strentence.strSentence=tempSentence;

    if(m_mode == TEACH_MODIFY){
        m_fileSrc->modifyProgramSentence(tmp_Pointer,tmp_strentence);
        tag_ADD = false;

        emit programOperationFinishedSignal(true);
    }
    else{
        tmp_Pointer.nSentence=m_line+1;
        int tmp_size=m_fileSrc->getFuncIndex().size();
        tmp_Pointer.nFunction=tmp_size;
        tmp_Pointer.nSentence=0;

        m_fileSrc->addProgramSentence(tmp_Pointer,tmp_strentence);

        string tempSentence1;
        tempSentence1=tempSentence1+"ENDFUNC";

        tmp_Pointer.nSentence=1;

        tmp_strentence.strSentence=tempSentence1;

        m_fileSrc->addProgramSentence(tmp_Pointer,tmp_strentence);
        tag_ADD = true;

        emit editSubSignal();
    }
}

void RobotProgramSubFuncWidget::buttonNoClicked(){
    emit programOperationFinishedSignal(false);
}
