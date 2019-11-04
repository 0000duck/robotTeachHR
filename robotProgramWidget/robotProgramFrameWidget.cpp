#include "robotProgramFrameWidget.h"

using namespace std;
using namespace rclib;
using namespace robsoft;

extern bool tag_ADD;

RobotProgramFrameWidget::RobotProgramFrameWidget(QWidget *parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramFrameWidget::~RobotProgramFrameWidget(){

}

void RobotProgramFrameWidget::passParameter(RobotInterpreter* p_file){
    m_fileSrc = p_file;
}

void RobotProgramFrameWidget::setMode(OPERATIONMODE mode){
    m_mode = mode;

    vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();

    if(m_mode == TEACH_MODIFY){
        VECTOR_STRING tmp_Frame=Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");

        m_RobotFrame=RobotClient::initance()->getRobotFrame();

        m_toolFrame = m_RobotFrame.getToolFrameList();
        m_userFrame = m_RobotFrame.getWorkFrameList();

        std::map<std::string, robsoft::Terminal> tmp_FrameList;

        if(tmp_Frame[0] == string("TOOL")){
            m_radioToolFrame->setChecked(true);
            tmp_FrameList = m_toolFrame;
        }
        else if(tmp_Frame[0] == string("USER")){
            m_radioUserFrame->setChecked(true);
            tmp_FrameList = m_userFrame;
        }

        string frameName = tmp_Frame[1];

        if(tmp_FrameList.find(frameName) == tmp_FrameList.end()){
            QMessageBox::question(this, tr("提示"),
                                  tr("该坐标系变量名不存在于变量列表中!"),
                                  QMessageBox::Yes);
            emit programOperationFinishedSignal(false);
            return;
        }

        int I = 0;
        int currentFrame = 0;

        m_comboFrameName->clear();
        for(std::map<std::string, Terminal>::iterator it = tmp_FrameList.begin(); it != tmp_FrameList.end(); it++){
            for(int i=0; i<1; i++){
                string tmp = it->first;
                if(tmp == frameName)
                    currentFrame = I;
                m_comboFrameName->insertItem(I,tmp.c_str());
                I++;
            }
        }

        m_comboFrameName->setCurrentIndex(currentFrame);

        groupFrameType->setEnabled(false);
    }
    else{
        updateFrameList();
    }
}

void RobotProgramFrameWidget::passSentenceIndex(int func, int line){
    m_func = func;
    m_line = line;
}

void RobotProgramFrameWidget::initUI(){
    m_radioToolFrame = new QRadioButton(tr("工具坐标系"));
    m_radioUserFrame = new QRadioButton(tr("工件坐标系"));
    QHBoxLayout* layoutFrameType = new QHBoxLayout();
    layoutFrameType->addWidget(m_radioToolFrame);
    layoutFrameType->addWidget(m_radioUserFrame);
    groupFrameType = new QGroupBox(tr("坐标系类型"));
    groupFrameType->setLayout(layoutFrameType);

    QHBoxLayout* layoutFrameChoose = new QHBoxLayout();
    layoutFrameChoose->addWidget(groupFrameType);

    QLabel* labelFrameName = new QLabel(tr("坐标系\n名称:"));
    m_comboFrameName = new ComboBox();
    m_comboFrameName->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    m_buttonYes->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonYes->setMaximumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setMaximumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutSure = new QVBoxLayout();
    layoutSure->addWidget(m_buttonYes);
    layoutSure->addWidget(m_buttonNo);
    QGroupBox* groupSure = new QGroupBox();
    groupSure->setLayout(layoutSure);

    QVBoxLayout* layoutOperation = new QVBoxLayout();
    layoutOperation->addStretch();
    layoutOperation->addWidget(groupSure, 0, Qt::AlignRight);

    QHBoxLayout* layoutFrameOperation = new QHBoxLayout();
    layoutFrameOperation->addWidget(labelFrameName);
    layoutFrameOperation->addWidget(m_comboFrameName, 0, Qt::AlignLeft);
    layoutFrameOperation->addLayout(layoutOperation);


    QVBoxLayout* layoutFrame = new QVBoxLayout(this);
    layoutFrame->addLayout(layoutFrameChoose);
    layoutFrame->addLayout(layoutFrameOperation);
    layoutFrame->setStretchFactor(layoutFrameChoose, 1);
    layoutFrame->setStretchFactor(layoutFrameOperation, 3);
}

void RobotProgramFrameWidget::initState(){
    clearState();

    updateFrameList();
}

void RobotProgramFrameWidget::connectSignalSlot(){
    QObject::connect(m_radioToolFrame, SIGNAL(toggled(bool)), this, SLOT(radioFrameTypeChanged()));
    QObject::connect(m_radioUserFrame, SIGNAL(toggled(bool)), this, SLOT(radioFrameTypeChanged()));

    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));
}

void RobotProgramFrameWidget::clearState(){
    m_radioToolFrame->setChecked(true);
    groupFrameType->setEnabled(true);
}

void RobotProgramFrameWidget::updateFrameList()
{
    m_RobotFrame=RobotClient::initance()->getRobotFrame();

    m_toolFrame = m_RobotFrame.getToolFrameList();
    m_userFrame = m_RobotFrame.getWorkFrameList();

    m_comboFrameName->clear();

    int I = 0;
    if(m_radioToolFrame->isChecked()){
        for(std::map<std::string, Terminal>::iterator it = m_toolFrame.begin(); it != m_toolFrame.end(); it++){
            for(int i=0; i<1; i++){
                string tmp = it->first;
                m_comboFrameName->insertItem(I,tmp.c_str());
                I++;
            }
        }
    }else if(m_radioUserFrame->isChecked()){
        for(std::map<std::string, Terminal>::iterator it = m_toolFrame.begin(); it != m_toolFrame.end(); it++){
            for(int i=0; i<1; i++){
                string tmp = it->first;
                m_comboFrameName->insertItem(I,tmp.c_str());
                I++;
            }
        }
    }
}

void RobotProgramFrameWidget::radioFrameTypeChanged()
{
    m_comboFrameName->setEnabled(true);

    int I = 0;

    m_RobotFrame=RobotClient::initance()->getRobotFrame();

    m_toolFrame = m_RobotFrame.getToolFrameList();
    m_userFrame = m_RobotFrame.getWorkFrameList();

    if(m_radioToolFrame->isChecked()){
        for(std::map<std::string, Terminal>::iterator it = m_toolFrame.begin(); it != m_toolFrame.end(); it++){
            for(int i=0; i<1; i++){
                string tmp = it->first;
                m_comboFrameName->insertItem(I,tmp.c_str());
                I++;
            }
        }
    }
    if(m_radioUserFrame->isChecked()){
        for(std::map<std::string, Terminal>::iterator it = m_userFrame.begin(); it != m_userFrame.end(); it++){
            for(int i=0; i<1; i++){
                string tmp = it->first;
                m_comboFrameName->insertItem(I,tmp.c_str());
                I++;
            }
        }
    }

    m_comboFrameName->setCurrentIndex(0);
}

void RobotProgramFrameWidget::buttonYesClicked(){
    string frameName = m_comboFrameName->currentText().toStdString();

    ProgramPointer tmp_pointer;

    tmp_pointer.nFunction = m_func;
    tmp_pointer.nSentence = m_line;

    string tmp_sentence;
    if(m_radioUserFrame->isChecked())
    {
        tmp_sentence = tmp_sentence + "WORK" + " " + frameName;
    }
    else
    {
        tmp_sentence = tmp_sentence + "TOOL" + " " + frameName;
    }

    ProgramSentence m_strentence;
    m_strentence.strSentence = tmp_sentence;

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

void RobotProgramFrameWidget::buttonNoClicked(){
    emit programOperationFinishedSignal(false);
}
