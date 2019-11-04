#include "robotProgramVariableWidget.h"

#include <QDebug>

using namespace std;
using namespace rclib;

extern bool tag_ADD;

RobotProgramVariableWidget::RobotProgramVariableWidget(QWidget *parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramVariableWidget::~RobotProgramVariableWidget(){

}

void RobotProgramVariableWidget::passParameter(RobotInterpreter* p_file){
    m_fileSrc = p_file;
}

void RobotProgramVariableWidget::setMode(OPERATIONMODE mode){
    m_mode = mode;

    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType=m_fileSrc->getVariableMap();

    if(m_mode == TEACH_MODIFY){
        if(tmp_VarType.find(m_varName) == tmp_VarType.end()){
            QMessageBox::question(this, tr("提示"),
                                  tr("该变量名不存在于变量列表中!"),
                                  QMessageBox::Yes);
            emit programOperationFinishedSignal(false);
            return;
        }

        PROGRAM_KEY_WORD tmp_KeyWord;
        VECTOR_DOUBLE tmp_Vaule;

        m_fileSrc->getVariable(m_varName,tmp_KeyWord,tmp_Vaule);

        m_editVariableName->setText(QString::fromStdString(m_varName));

        if(tmp_VarType[m_varName] == JOINT){
            m_radioJOINT->setChecked(true);

            for(int i=0; i<ROBOTDOF; i++){
                m_editVariablePositionValue[i]->setText(QString("%1").arg(tmp_Vaule[i]));
            }
        }
        else if(tmp_VarType[m_varName] == TERMINAL){
            m_radioTERMINAL->setChecked(true);

            for(int i=0; i<6; i++){
                m_editVariablePositionValue[i]->setText(QString("%1").arg(tmp_Vaule[i]));
            }
        }
        else if(tmp_VarType[m_varName] == rclib::INT){
            m_radioINT->setChecked(true);

            m_editVariableValue->setText(QString("%1").arg(tmp_Vaule[0]));
        }
        else if(tmp_VarType[m_varName] == rclib::DOUBLE){
            m_radioDOUBLE->setChecked(true);

            m_editVariableValue->setText(QString("%1").arg(tmp_Vaule[0]));
        }
        else if(tmp_VarType[m_varName] == rclib::BOOL){
            m_radioBOOL->setChecked(true);

            m_editVariableValue->setText(QString("%1").arg(tmp_Vaule[0]));
        }
        radioVariableTypeChanged();
        m_editVariableName->setReadOnly(true);
        groupVariableType->setEnabled(false);
    }
    else{
        clearState();
    }
}

void RobotProgramVariableWidget::passVariableName(string varName){
    m_varName = varName;
}

void RobotProgramVariableWidget::initUI(){
    m_radioJOINT = new QRadioButton("JOINT\n关节型");
    m_radioTERMINAL = new QRadioButton("TERMINAL\n末端型");
    m_radioINT = new QRadioButton("INT\n整型");
    m_radioDOUBLE = new QRadioButton("DOUBLE\n双精度型");
    m_radioBOOL = new QRadioButton("BOOL\n布尔型");

    QHBoxLayout* layoutVariableType = new QHBoxLayout();
    layoutVariableType->addWidget(m_radioJOINT);
    layoutVariableType->addWidget(m_radioTERMINAL);
    layoutVariableType->addWidget(m_radioINT);
    layoutVariableType->addWidget(m_radioDOUBLE);
    layoutVariableType->addWidget(m_radioBOOL);

    groupVariableType = new QGroupBox(tr("变量类型"));
    groupVariableType->setLayout(layoutVariableType);

    QLabel* labelVariableName = new QLabel(tr("变量\n名称:"));
    QLabel* labelVariableEqual = new QLabel(" = ");

    m_editVariableName = new QLineEdit();
    m_editVariableName->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    for(int i=0; i<ROBOTDOF; i++){
        m_editVariablePositionValue[i] = new QLineEdit();
        m_editVariablePositionValue[i]->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    }

    m_editVariableValue = new QLineEdit();
    m_editVariableValue->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_buttonBool = new PushButton("FALSE");
    m_buttonFrame = new PushButton(tr("基坐标取值"));
    m_buttonBool->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonFrame->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_buttonToolFrame = new PushButton(tr("插入当前工具坐标系"));
    m_buttonUserFrame = new PushButton(tr("插入当前工件坐标系"));
    m_buttonToolFrame->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonUserFrame->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_buttonMoveTo = new PushButton(tr("移动到当前位置"));
    m_buttonPosition = new PushButton(tr("插入当前位置"));
    m_buttonMoveTo->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonPosition->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    m_buttonYes->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutVariableName = new QHBoxLayout();
    layoutVariableName->addWidget(labelVariableName);
    layoutVariableName->addWidget(m_editVariableName);

    QVBoxLayout* layoutVariableValue = new QVBoxLayout();
    for(int i=0; i<ROBOTDOF; i++){
        layoutVariableValue->addWidget(m_editVariablePositionValue[i]);
    }
    layoutVariableValue->addWidget(m_buttonBool);
    layoutVariableValue->addWidget(m_editVariableValue);
    QGroupBox* groupVariableValue = new QGroupBox();
    groupVariableValue->setLayout(layoutVariableValue);

    QVBoxLayout* layoutVariableGetValue = new QVBoxLayout();
    layoutVariableGetValue->addWidget(m_buttonFrame);
    layoutVariableGetValue->addWidget(m_buttonPosition);
    layoutVariableGetValue->addWidget(m_buttonMoveTo);
    layoutVariableGetValue->addWidget(m_buttonToolFrame);
    layoutVariableGetValue->addWidget(m_buttonUserFrame);
    m_groupVariableGetValue = new QGroupBox();
    m_groupVariableGetValue->setLayout(layoutVariableGetValue);

    QVBoxLayout* layoutSure = new QVBoxLayout();
    layoutSure->addWidget(m_buttonYes);
    layoutSure->addWidget(m_buttonNo);
    QGroupBox* groupSure = new QGroupBox();
    groupSure->setLayout(layoutSure);

    QVBoxLayout* layoutOperation = new QVBoxLayout();
    layoutOperation->addWidget(m_groupVariableGetValue);
    layoutOperation->addWidget(groupSure);

    QHBoxLayout* layoutVariableValueOperation = new QHBoxLayout();
    layoutVariableValueOperation->addLayout(layoutVariableName);
    layoutVariableValueOperation->addWidget(labelVariableEqual);
    layoutVariableValueOperation->addWidget(groupVariableValue);
    layoutVariableValueOperation->addLayout(layoutOperation);
    layoutVariableValueOperation->setStretchFactor(layoutVariableName, 2);
    layoutVariableValueOperation->setStretchFactor(labelVariableEqual, 1);
    layoutVariableValueOperation->setStretchFactor(groupVariableValue, 2);
    layoutVariableValueOperation->setStretchFactor(layoutOperation, 3);

    QVBoxLayout* layoutVariable = new QVBoxLayout(this);
    layoutVariable->addWidget(groupVariableType);
    layoutVariable->addLayout(layoutVariableValueOperation);
    layoutVariable->setStretchFactor(groupVariableType, 1);
    layoutVariable->setStretchFactor(layoutVariableValueOperation, 6);
}

void RobotProgramVariableWidget::initState(){
    m_buttonBool->setStateSum(2);
    m_buttonFrame->setStateSum(2);

    for(int i=0; i<ROBOTDOF; i++){
        m_editVariablePositionValue[i]->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    }
    m_editVariableValue->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));

    clearState();
}

void RobotProgramVariableWidget::connectSignalSlot(){
    QObject::connect(m_radioJOINT, SIGNAL(toggled(bool)), this, SLOT(radioVariableTypeChanged()));
    QObject::connect(m_radioTERMINAL, SIGNAL(toggled(bool)), this, SLOT(radioVariableTypeChanged()));
    QObject::connect(m_radioINT, SIGNAL(toggled(bool)), this, SLOT(radioVariableTypeChanged()));
    QObject::connect(m_radioDOUBLE, SIGNAL(toggled(bool)), this, SLOT(radioVariableTypeChanged()));
    QObject::connect(m_radioBOOL, SIGNAL(toggled(bool)), this, SLOT(radioVariableTypeChanged()));

    QObject::connect(m_buttonToolFrame, SIGNAL(clicked()), this, SLOT(buttonToolFrameClicked()));
    QObject::connect(m_buttonUserFrame, SIGNAL(clicked()), this, SLOT(buttonUserFrameClicked()));
    QObject::connect(m_buttonMoveTo, SIGNAL(clicked()), this, SLOT(buttonMoveToClicked()));
    QObject::connect(m_buttonPosition, SIGNAL(clicked()), this, SLOT(buttonPositionClicked()));

    QObject::connect(m_buttonBool, SIGNAL(clicked()), this, SLOT(buttonBoolClicked()));
    QObject::connect(m_buttonFrame, SIGNAL(clicked()), this, SLOT(buttonFrameClicked()));

    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));
}

void RobotProgramVariableWidget::radioVariableTypeChanged(){
    if(m_radioJOINT->isChecked()){
        m_buttonBool->hide();
        m_editVariableValue->hide();
        for(int i=0; i<ROBOTDOF; i++){
            m_editVariablePositionValue[i]->show();
        }

        m_groupVariableGetValue->show();
        m_buttonFrame->hide();
        m_buttonPosition->show();
        m_buttonMoveTo->show();
        m_buttonToolFrame->hide();
        m_buttonUserFrame->hide();
    }
    if(m_radioTERMINAL->isChecked()){
        m_buttonBool->hide();
        m_editVariableValue->hide();
        for(int i=0; i<ROBOTDOF; i++){
            m_editVariablePositionValue[i]->show();
        }

        m_groupVariableGetValue->show();
        m_buttonFrame->show();
        m_buttonPosition->show();
        m_buttonMoveTo->show();
        m_buttonToolFrame->hide();
        m_buttonUserFrame->hide();
    }
    if(m_radioINT->isChecked()){
        for(int i=0; i<ROBOTDOF; i++){
            m_editVariablePositionValue[i]->hide();
        }
        m_buttonBool->hide();
        m_editVariableValue->show();

        m_groupVariableGetValue->show();
        m_buttonFrame->hide();
        m_buttonPosition->hide();
        m_buttonMoveTo->hide();
        m_buttonToolFrame->hide();
        m_buttonUserFrame->hide();
    }
    if(m_radioDOUBLE->isChecked()){
        for(int i=0; i<ROBOTDOF; i++){
            m_editVariablePositionValue[i]->hide();
        }
        m_buttonBool->hide();
        m_editVariableValue->show();

        m_groupVariableGetValue->show();
        m_buttonFrame->hide();
        m_buttonPosition->hide();
        m_buttonMoveTo->hide();
        m_buttonToolFrame->hide();
        m_buttonUserFrame->hide();
    }
    if(m_radioBOOL->isChecked()){
        for(int i=0; i<ROBOTDOF; i++){
            m_editVariablePositionValue[i]->hide();
        }
        m_buttonBool->show();
        m_editVariableValue->hide();

        m_groupVariableGetValue->show();
        m_buttonFrame->hide();
        m_buttonPosition->hide();
        m_buttonMoveTo->hide();
        m_buttonToolFrame->hide();
        m_buttonUserFrame->hide();
    }
}

void RobotProgramVariableWidget::buttonToolFrameClicked(){
    Terminal frame=RobotClient::initance()->getRobotMotion().getCurrentTerminal();

    for(int i=0; i<6; i++){
        m_editVariablePositionValue[i]->setText(QString("%1").arg(frame[static_cast<TERMINALINDEX>(i)]));
    }
}

void RobotProgramVariableWidget::buttonUserFrameClicked(){
    Terminal frame=RobotClient::initance()->getRobotMotion().getCurrentWorkTerminal();

    for(int i=0; i<6; i++){
        m_editVariablePositionValue[i]->setText(QString("%1").arg(frame[static_cast<TERMINALINDEX>(i)]));
    }
}

void RobotProgramVariableWidget::buttonPositionClicked(){
    if(m_radioJOINT->isChecked()){
        Joints joint=RobotClient::initance()->getRobotMotion().getCurrentJointPosition();

        for(int i=0; i<ROBOTDOF; i++){
            m_editVariablePositionValue[i]->setText(QString("%1").arg(joint[static_cast<JOINTINDEX>(i)]));
        }
    }
    else if(m_radioTERMINAL->isChecked()){
        if(m_buttonFrame->returnState() == 0){
            Terminal frame=RobotClient::initance()->getRobotMotion().getCurrentTerminal();

            for(int i=0; i<6; i++){
                m_editVariablePositionValue[i]->setText(QString("%1").arg(frame[static_cast<TERMINALINDEX>(i)]));
            }
        }
        else if(m_buttonFrame->returnState() == 1){
            Terminal frame=RobotClient::initance()->getRobotMotion().getCurrentWorkTerminal();

            for(int i=0; i<6; i++){
                m_editVariablePositionValue[i]->setText(QString("%1").arg(frame[static_cast<TERMINALINDEX>(i)]));
            }
        }
        else{
            Terminal frame=RobotClient::initance()->getRobotMotion().getCurrentTerminal();

            for(int i=0; i<6; i++){
                m_editVariablePositionValue[i]->setText(QString("%1").arg(frame[static_cast<TERMINALINDEX>(i)]));
            }
        }
    }
}

void RobotProgramVariableWidget::buttonMoveToClicked(){
    double vel = getGlobalVel();
    if(m_radioJOINT->isChecked()){
        Joints joint;

        for(int i=0; i<ROBOTDOF; i++){
            joint[static_cast<JOINTINDEX>(i)] = m_editVariablePositionValue[i]->text().toDouble();
        }

        RobotClient::initance()->moveABSJoint(joint, vel);
    }
    else if(m_radioTERMINAL->isChecked()){
        if(m_buttonFrame->returnState() == 0){  //基坐标系
            Terminal terminal;

            for(int i=0; i<6; i++){
                terminal[static_cast<TERMINALINDEX>(i)] = m_editVariablePositionValue[i]->text().toDouble();
            }

            RobotClient::initance()->moveJoint(terminal, vel,COORDINATE_BASE);
        }
        else if(m_buttonFrame->returnState() == 1){
            Terminal terminal;

            for(int i=0; i<6; i++){
                terminal[static_cast<TERMINALINDEX>(i)] = m_editVariablePositionValue[i]->text().toDouble();
            }

            RobotClient::initance()->moveJoint(terminal, vel, COORDINATE_WORK);
        }
    }
}

void RobotProgramVariableWidget::clearState(){
    m_radioJOINT->setChecked(true);
    radioVariableTypeChanged();

    m_editVariableName->clear();
    for(int i=0; i<ROBOTDOF; i++){
        m_editVariablePositionValue[i]->clear();
        m_editVariablePositionValue[i]->setText("0");
    }
    m_editVariableValue->clear();
    m_editVariableValue->setText("0");

    m_editVariableName->setReadOnly(false);
    groupVariableType->setEnabled(true);
}

bool RobotProgramVariableWidget::grammarCheck(){
    if(m_editVariableName->text().isEmpty()){
        QMessageBox::question(this, tr("提示"),
                              tr("变量名不能为空!"),
                              QMessageBox::Yes);
        return false;
    }

    if(m_editVariableName->text().indexOf(QRegExp("^[A-Za-z_][A-Za-z0-9_]*$")) == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("变量名格式错误!"),
                              QMessageBox::Yes);
        return false;
    }

    if(m_radioJOINT->isChecked() || m_radioTERMINAL->isChecked() /*|| m_radioFRAME->isChecked()*/){
        for(int i=0; i<ROBOTDOF; i++){
            if(m_editVariablePositionValue[i]->text().isEmpty()){
                QMessageBox::question(this, tr("提示"),
                                      tr("变量值不能为空!"),
                                      QMessageBox::Yes);
                return false;
            }
        }
    }
    else if(m_radioINT->isChecked() || m_radioDOUBLE->isChecked()){
        if(m_editVariableValue->text().isEmpty()){
            QMessageBox::question(this, tr("提示"),
                                  tr("变量值不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }
    }

    string name = m_editVariableName->text().toStdString();

    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType=m_fileSrc->getVariableMap();
    if(!(tmp_VarType.find(name) == tmp_VarType.end())){
        if(m_mode == TEACH_ADD){
            QMessageBox::question(this, tr("提示"),
                                  tr("该变量名已存在于变量列表中!"),
                                  QMessageBox::Yes);
            return false;
        }
    }

    return true;
}

void RobotProgramVariableWidget::buttonYesClicked(){
    if(!grammarCheck()){
        return;
    }

    string tmp_strentence;
    string name = m_editVariableName->text().toStdString();

    if(m_radioJOINT->isChecked()){
        tmp_strentence=tmp_strentence+"JOINT" + " "+name+" "+"=";
        for(int i=0; i<ROBOTDOF; i++){
            tmp_strentence=tmp_strentence+" "+m_editVariablePositionValue[i]->text().toStdString();
        }
        if(m_mode==TEACH_ADD)
        {
            m_fileSrc->addVariable(tmp_strentence);
        }
        else
        {
            m_fileSrc->modifyVariable(m_varName,tmp_strentence);
        }
    }
    else if(m_radioTERMINAL->isChecked()){
        tmp_strentence=tmp_strentence+"TERMINAL" + " "+name+" "+"=";
        for(int i=0; i<ROBOTDOF; i++){
            tmp_strentence=tmp_strentence+" "+m_editVariablePositionValue[i]->text().toStdString();
        }
        if(m_mode==TEACH_ADD)
        {
            m_fileSrc->addVariable(tmp_strentence);
        }
        else
        {
            m_fileSrc->modifyVariable(m_varName,tmp_strentence);
        }
    }
    else if(m_radioINT->isChecked()){
        tmp_strentence=tmp_strentence+"INT" + " "+name+" "+"=";
        tmp_strentence=tmp_strentence+" "+m_editVariableValue->text().toStdString();

        if(m_mode==TEACH_ADD)
        {
            m_fileSrc->addVariable(tmp_strentence);
        }
        else
        {
            m_fileSrc->modifyVariable(m_varName,tmp_strentence);
        }
    }
    else if(m_radioDOUBLE->isChecked()){
        tmp_strentence=tmp_strentence+"DOUBLE" + " "+name+" "+"=";
        tmp_strentence=tmp_strentence+" "+m_editVariableValue->text().toStdString();
        if(m_mode==TEACH_ADD)
        {
            m_fileSrc->addVariable(tmp_strentence);
        }
        else
        {
            m_fileSrc->modifyVariable(m_varName,tmp_strentence);
        }
    }
    else if(m_radioBOOL->isChecked()){
        tmp_strentence=tmp_strentence+"BOOL" + " "+name+" "+"=";
        tmp_strentence=tmp_strentence+" "+QString::number(m_buttonBool->returnState()).toStdString();
        if(m_mode==TEACH_ADD)
        {
            m_fileSrc->addVariable(tmp_strentence);
        }
        else
        {
            m_fileSrc->modifyVariable(m_varName,tmp_strentence);
        }
    }

    emit programOperationFinishedSignal(true);
}

void RobotProgramVariableWidget::buttonNoClicked(){
    emit programOperationFinishedSignal(false);
}

void RobotProgramVariableWidget::buttonBoolClicked(){
    m_buttonBool->incState();
    switch (m_buttonBool->returnState()) {
    case 0:
        m_buttonBool->setText("FALSE");
        break;
    case 1:
        m_buttonBool->setText("TRUE");
        break;
    default:
        break;
    }
}

void RobotProgramVariableWidget::buttonFrameClicked(){
    m_buttonFrame->incState();
    switch (m_buttonFrame->returnState()) {
    case 0:
        m_buttonFrame->setText(tr("基坐标取值"));
        break;
    case 1:
        m_buttonFrame->setText(tr("工件坐标取值"));
        break;
    default:
        break;
    }
}
