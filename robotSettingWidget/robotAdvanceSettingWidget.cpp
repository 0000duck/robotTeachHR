#include "robotAdvanceSettingWidget.h"

#include <QDebug>

using namespace robsoft;
using namespace rclib;

RobotAdvanceSettingWidget::RobotAdvanceSettingWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, "#808080");
    this->setPalette(pe);

    initUI();
    initState();
    connectSignalSlot();
}

RobotAdvanceSettingWidget::~RobotAdvanceSettingWidget(){

}

void RobotAdvanceSettingWidget::initUI(){
    for(int i=0; i<6; i++){
        m_labelJointZero[i] = new QLabel(QString("J") + QString::number(i+1, 10) + QString(":"));
        m_editJointZero[i] = new QLineEdit();
        m_editJointZero[i]->setReadOnly(true);
        m_buttonJointZero[i] = new PushButton();
        m_buttonJointZero[i]->setIndex(i);
        m_buttonJointZero[i]->setText(tr("置零"));
        m_editJointZero[i]->setMinimumSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
        m_buttonJointZero[i]->setMinimumSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    }

    QHBoxLayout* layoutSingleJointZero[6];
    for(int i=0; i<6; i++){
        layoutSingleJointZero[i] = new QHBoxLayout();
        layoutSingleJointZero[i]->addWidget(m_labelJointZero[i]);
        layoutSingleJointZero[i]->addWidget(m_editJointZero[i]);
        layoutSingleJointZero[i]->addWidget(m_buttonJointZero[i]);
        layoutSingleJointZero[i]->setStretchFactor(m_labelJointZero[i], 1);
        layoutSingleJointZero[i]->setStretchFactor(m_editJointZero[i], 1);
        layoutSingleJointZero[i]->setStretchFactor(m_buttonJointZero[i], 1);
    }

    m_buttonWholeJointZero = new PushButton(tr("全部置零"));
    m_buttonWholeJointZero->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutJointZero = new QVBoxLayout();
    for(int i=0; i<6; i++){
        layoutJointZero->addLayout(layoutSingleJointZero[i]);
    }
    layoutJointZero->addWidget(m_buttonWholeJointZero);

    m_groupJointZero = new QGroupBox(tr("关节置零"));
    m_groupJointZero->setLayout(layoutJointZero);

    m_buttonCurrentJoint = new PushButton(tr("当前关节位置"));
    m_buttonCurrentJoint->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    for(int i=0; i<6; i++){
        m_labelInitJoint[i] = new QLabel(QString("J") + QString::number(i+1, 10) + QString(":"));
        m_editInitJoint[i] = new QLineEdit();
        m_editInitJoint[i]->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

        m_editInitJoint[i]->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    }

    QHBoxLayout* layoutSingleInitJoint[6];
    for(int i=0; i<6; i++){
        layoutSingleInitJoint[i] = new QHBoxLayout();
        layoutSingleInitJoint[i]->addWidget(m_labelInitJoint[i]);
        layoutSingleInitJoint[i]->addWidget(m_editInitJoint[i]);
        layoutSingleInitJoint[i]->setStretchFactor(m_labelInitJoint[i], 1);
        layoutSingleInitJoint[i]->setStretchFactor(m_editInitJoint[i], 1);
    }

    QVBoxLayout* layoutInitJoint = new QVBoxLayout();
    for(int i=0; i<6; i++){
        layoutInitJoint->addLayout(layoutSingleInitJoint[i]);
    }
    layoutInitJoint->addWidget(m_buttonCurrentJoint);

    m_groupInitJoint = new QGroupBox(tr("关节初始位置"));
    m_groupInitJoint->setLayout(layoutInitJoint);

    for(int i=0; i<6; i++){
        m_labelReturnZeroIndex[i] = new QLabel(QString(tr("回零序号")) + QString::number(i+1, 10) + QString(tr("\n轴号(1-6):")));
        m_editReturnZeroIndex[i] = new QLineEdit();
        m_editReturnZeroIndex[i]->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

        m_editReturnZeroIndex[i]->setValidator(new QIntValidator(-6, 6, this));
    }

    QHBoxLayout* layoutReturnZeroIndex[6];
    for(int i=0; i<6; i++){
        layoutReturnZeroIndex[i] = new QHBoxLayout();
        layoutReturnZeroIndex[i]->addWidget(m_labelReturnZeroIndex[i]);
        layoutReturnZeroIndex[i]->addWidget(m_editReturnZeroIndex[i]);
        layoutReturnZeroIndex[i]->setStretchFactor(m_labelReturnZeroIndex[i], 1);
        layoutReturnZeroIndex[i]->setStretchFactor(m_editReturnZeroIndex[i], 1);
    }

    m_buttonDefaultIndex = new PushButton(tr("默认顺序"));
    m_buttonDefaultIndex->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutReturnZero = new QVBoxLayout();
    for(int i=0; i<6; i++){
        layoutReturnZero->addLayout(layoutReturnZeroIndex[i]);
    }
    layoutReturnZero->addWidget(m_buttonDefaultIndex);

    m_groupReturnZero = new QGroupBox(tr("回零设置"));
    m_groupReturnZero->setLayout(layoutReturnZero);

    for(int i=0; i<6; i++){
        m_labelSetCurrentJoint[i] = new QLabel(QString("J") + QString::number(i+1, 10) + QString(":"));
        m_editSetCurrentJoint[i] = new QLineEdit();
        m_editSetCurrentJoint[i]->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
        m_editSetCurrentJoint[i]->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    }

    QHBoxLayout* layoutCurrentJoint[6];
    for(int i=0; i<6; i++){
        layoutCurrentJoint[i] = new QHBoxLayout();
        layoutCurrentJoint[i]->addWidget(m_labelSetCurrentJoint[i]);
        layoutCurrentJoint[i]->addWidget(m_editSetCurrentJoint[i]);
        layoutCurrentJoint[i]->setStretchFactor(m_labelSetCurrentJoint[i], 1);
        layoutCurrentJoint[i]->setStretchFactor(m_editSetCurrentJoint[i], 1);
    }

    m_buttonSetCurrentJoint = new PushButton(tr("当前关节位置"));
    m_buttonSetCurrentJoint->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutSetCurrentJoint = new QVBoxLayout();
    for(int i=0; i<6; i++){
        layoutSetCurrentJoint->addLayout(layoutCurrentJoint[i]);
    }
    layoutSetCurrentJoint->addWidget(m_buttonSetCurrentJoint);

    m_groupSetCurrentJoint = new QGroupBox(tr("零位补偿"));
    m_groupSetCurrentJoint->setLayout(layoutSetCurrentJoint);

    for(int i=0; i<6; i++){
        m_labelJointState[i] = new QLabel(QString("J") + QString::number(i+1, 10) + QString(":"));
        m_buttonStateLight[i] = new PushButton();
        m_buttonStateLight[i]->setMinimumSize(0.04*WINDOWWIDTH, 0.04*WINDOWWIDTH);
        m_buttonStateLight[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
    }

    m_buttonSoundSwitch = new PushButton(tr("蜂鸣器开关"));
    m_buttonSoundSwitch->setMinimumSize(0.04*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonSoundSwitch->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));

    m_comboxLanguageSwitch = new QComboBox();
    m_comboxLanguageSwitch->setMinimumSize(0.04*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboxLanguageSwitch->insertItem(0,"中文");
    m_comboxLanguageSwitch->insertItem(1,"English");


    QHBoxLayout* layouJointState[6];
    for(int i=0; i<6; i++){
        layouJointState[i] = new QHBoxLayout();
        layouJointState[i]->addWidget(m_labelJointState[i]);
        layouJointState[i]->addWidget(m_buttonStateLight[i]);
        layouJointState[i]->setStretchFactor(m_labelJointState[i], 1);
        layouJointState[i]->setStretchFactor(m_buttonStateLight[i], 1);
    }

    QVBoxLayout* layoutSetJointState = new QVBoxLayout();
    for(int i=0; i<6; i++){
        layoutSetJointState->addLayout(layouJointState[i]);
    }

    m_groupJointState = new QGroupBox(tr("传感器状态"));
    m_groupJointState->setLayout(layoutSetJointState);

    QVBoxLayout* layoutOther = new QVBoxLayout();
    layoutOther->addWidget(m_buttonSoundSwitch);
    layoutOther->addWidget(m_comboxLanguageSwitch);

    m_groupOther = new QGroupBox(tr("其它设置"));
    m_groupOther->setLayout(layoutOther);

    QVBoxLayout* layoutState = new QVBoxLayout();
    layoutState->addWidget(m_groupJointState);
    layoutState->addWidget(m_groupOther);
    layoutState->setStretchFactor(m_groupJointState, 7);
    layoutState->setStretchFactor(m_groupOther, 3);

    QHBoxLayout* layoutAdvanceSetting = new QHBoxLayout(this);
    layoutAdvanceSetting->addWidget(m_groupJointZero);
    layoutAdvanceSetting->addWidget(m_groupInitJoint);
    layoutAdvanceSetting->addWidget(m_groupSetCurrentJoint);
    layoutAdvanceSetting->addWidget(m_groupReturnZero);
    layoutAdvanceSetting->addLayout(layoutState);
}

void RobotAdvanceSettingWidget::initState(){
    m_robotProference=RobotClient::initance()->getRobotPreference();
    m_oldRobotProference=RobotClient::initance()->getRobotPreference();

    m_robotMotion = RobotClient::initance()->getRobotMotion();
    Joints joint = m_robotMotion.getCurrentJointPosition();
    for(int i=0; i<6; i++){
        m_editJointZero[i]->setText(QString::number(joint[static_cast<JOINTINDEX>(i)], 10, 4));
    }

    for(int i=0; i<6; i++){
        m_editJointZero[i]->setText(QString("%1").arg(m_robotProference.getInitJointPosition()[static_cast<JOINTINDEX>(i)]));
    }

    for(int i=0; i<6; i++){
        m_editInitJoint[i]->setText(QString("%1").arg(m_robotProference.getInitJointPosition()[static_cast<JOINTINDEX>(i)]));
    }

    for(int i=0; i<6; i++){
        m_editSetCurrentJoint[i]->setText(QString("%1").arg(m_robotProference.getJointCompensation()[static_cast<JOINTINDEX>(i)]));
    }

    for(int i=0; i<6; i++){
        m_editReturnZeroIndex[i]->setText(QString("%1").arg(m_robotProference.getJointReturnSequence()[static_cast<JOINTINDEX>(i)]));
    }

    for(int i=0; i<6; i++){
        if(RobotClient::initance()->m_getHomeState[i]==SWITCHON){
            m_buttonStateLight[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
            m_homeState[i] = SWITCHON;
        }
        else if(RobotClient::initance()->m_getHomeState[i] == SWITCHOFF){
            m_buttonStateLight[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
            m_homeState[i] = SWITCHOFF;
        }
    }

    QString sound;
    read_cache_sound(sound);
    if(sound == QString("true")){
        SOUND = true;
        m_buttonSoundSwitch->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
    }else if(sound == QString("false")){
        SOUND = false;
        m_buttonSoundSwitch->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
    }

    QString language;
    read_cache_language(language);
    if(language == QString("English")){
        m_comboxLanguageSwitch->setCurrentIndex(1);
    }
    else{
        m_comboxLanguageSwitch->setCurrentIndex(0);
    }
}

void RobotAdvanceSettingWidget::connectSignalSlot(){
    for(int i=0; i<6; i++){
        QObject::connect(m_editInitJoint[i], SIGNAL(editingFinished()), this, SLOT(initJointValueChanged()));
        QObject::connect(m_buttonJointZero[i], SIGNAL(pushButtonClicked(int)), this, SLOT(buttonJointZeroClicked(int)));
        QObject::connect(m_editReturnZeroIndex[i], SIGNAL(editingFinished()), this, SLOT(returnZeroValueChanged()));
        QObject::connect(m_editSetCurrentJoint[i], SIGNAL(editingFinished()), this, SLOT(setCurrentJointValueChanged()));
    }
    QObject::connect(m_buttonWholeJointZero, SIGNAL(clicked()), this, SLOT(buttonWholeJointZeroClicked()));
    QObject::connect(m_buttonCurrentJoint, SIGNAL(clicked()), this, SLOT(buttonCurrentJointClicked()));
    QObject::connect(m_buttonSetCurrentJoint, SIGNAL(clicked()), this, SLOT(buttonSetCurrentJointClicked()));
    QObject::connect(m_buttonDefaultIndex,SIGNAL(clicked()),this,SLOT(buttonSetDefaultIndexClicked()));
    QObject::connect(m_buttonSoundSwitch,SIGNAL(clicked()),this,SLOT(buttonSoundSwitchClicked()));

    QObject::connect(m_comboxLanguageSwitch,SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChanged()));
}

void RobotAdvanceSettingWidget::updateAdvanceSettingWidget(){
    m_robotProference=RobotClient::initance()->getRobotPreference();

    for(int i=0; i<6; i++){
        m_editInitJoint[i]->setText(QString("%1").arg(m_robotProference.getInitJointPosition()[static_cast<JOINTINDEX>(i)]));
    }

    for(int i=0; i<6; i++){
        m_editSetCurrentJoint[i]->setText(QString("%1").arg(m_robotProference.getJointCompensation()[static_cast<JOINTINDEX>(i)]));
    }

    for(int i=0; i<6; i++){
        m_editReturnZeroIndex[i]->setText(QString("%1").arg(m_robotProference.getJointReturnSequence()[static_cast<JOINTINDEX>(i)]));
    }
}

void RobotAdvanceSettingWidget::initJointValueChanged(){
    RobotPreference rp=RobotClient::initance()->getRobotPreference();
    for(int i=0; i<6; i++){
        m_initJoint[static_cast<JOINTINDEX>(i)] = m_editInitJoint[i]->text().toDouble();
    }
    rp.setInitJointPosition(m_initJoint);
    RobotClient::initance()->setRobotPreference(rp);
}

void RobotAdvanceSettingWidget::returnZeroValueChanged(){
    RobotPreference rp=RobotClient::initance()->getRobotPreference();

    for(int i=0; i<6; i++){
        m_zeroJointIndex[static_cast<JOINTINDEX>(i)] = m_editReturnZeroIndex[i]->text().toInt();
    }
   rp.setJointReturnSequence(m_zeroJointIndex);
   RobotClient::initance()->setRobotPreference(rp);
}

void RobotAdvanceSettingWidget::setCurrentJointValueChanged(){
    RobotPreference rp=RobotClient::initance()->getRobotPreference();

    for(int i=0; i<6; i++){
        m_setJoint[static_cast<JOINTINDEX>(i)] = m_editSetCurrentJoint[i]->text().toDouble();
    }
    rp.setJointCompensation(m_setJoint);
    RobotClient::initance()->setRobotPreference(rp);
}

void RobotAdvanceSettingWidget::buttonSoundSwitchClicked()
{
    QString sound;
    if(SOUND == true){
        SOUND = false;
        sound = QString("false");
    }
    else{
        SOUND = true;
        sound = QString("true");
    }
    write_cache_sound(sound);
}

void RobotAdvanceSettingWidget::comboxChanged()
{
    QMessageBox::question(this, tr("提示"),
                          tr("重启设备后语言才会更新!"),
                          QMessageBox::Yes);

    QString tmp = m_comboxLanguageSwitch->currentText();
    QString language;
    if(tmp == QString("English")){
        language = QString("English");
    }else{
        language = QString("Chinese");
    }
    write_cache_language(language);
}

void RobotAdvanceSettingWidget::buttonJointZeroClicked(int index){
    if(RobotClient::initance()->getRobotState().getServoState()){
        QMessageBox::question(this, tr("提示"),
                              tr("置零失败，请先关闭伺服状态!"),
                              QMessageBox::Yes);
        return;
    }

    RobotClient::initance()->setJointZero(JOINTINDEX(index));

    QMessageBox::question(this, tr("提示"),
                          tr("置零成功!"),
                          QMessageBox::Yes);
}

void RobotAdvanceSettingWidget::buttonWholeJointZeroClicked(){
    if(RobotClient::initance()->getRobotState().getServoState()){
        QMessageBox::question(this, tr("提示"),
                              tr("置零失败，请先关闭伺服状态!"),
                              QMessageBox::Yes);
        return;
    }

    RobotClient::initance()->setJointZero(JOINT_WHOLE);

    QMessageBox::question(this, tr("提示"),
                          tr("置零成功!"),
                          QMessageBox::Yes);
}

void RobotAdvanceSettingWidget::buttonCurrentJointClicked(){
    Joints joint;
    joint=RobotClient::initance()->getRobotMotion().getCurrentJointPosition();
    for(int i=0; i<6; i++){
        m_editInitJoint[i]->setText(QString("%1").arg(joint[static_cast<JOINTINDEX>(i)]));
    }
    initJointValueChanged();
}

void RobotAdvanceSettingWidget::updateGroupEnabled(bool state){
    m_groupJointZero->setEnabled(state);
    m_groupInitJoint->setEnabled(state);
    m_groupReturnZero->setEnabled(state);
    m_groupSetCurrentJoint->setEnabled(state);
    m_groupJointState->setEnabled(state);
}

void RobotAdvanceSettingWidget::updateSystemState(){
    RobotPreference rp=RobotClient::initance()->getRobotPreference();

    for(int i=0; i<6; i++){
        m_setJoint[static_cast<JOINTINDEX>(i)] = m_editSetCurrentJoint[i]->text().toDouble();
    }

    rp.setJointCompensation(m_setJoint);
    RobotClient::initance()->setRobotPreference(rp);
}

void RobotAdvanceSettingWidget::updateCurrentPosition()
{
    m_robotMotion = RobotClient::initance()->getRobotMotion();
    Joints joint=m_robotMotion.getCurrentJointPosition();
    for(int i=0; i<6; i++){
        m_editJointZero[i]->setText(QString::number(joint[static_cast<JOINTINDEX>(i)], 10, 4));
    }
}

void RobotAdvanceSettingWidget::updateRobotLight()
{
    for(int i=0; i<6; i++){
        SWITCHSTATE tmp = RobotClient::initance()->m_getHomeState[i];
        if(tmp == m_homeState[i])
            continue;
        if(tmp == SWITCHON){
            m_buttonStateLight[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
            m_homeState[i] = SWITCHON;
        }
        else if(tmp == SWITCHOFF){
            m_buttonStateLight[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
            m_homeState[i] = SWITCHOFF;
        }
    }

    if(SOUND == true){
        m_buttonSoundSwitch->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
    }
    else{
        m_buttonSoundSwitch->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
    }
}

void RobotAdvanceSettingWidget::buttonSetCurrentJointClicked(){
    Joints joint;
    joint=RobotClient::initance()->getRobotMotion().getCurrentJointPosition();
    for(int i=0; i<6; i++){
        m_editSetCurrentJoint[i]->setText(QString("%1").arg(joint[static_cast<JOINTINDEX>(i)]));
    }
    setCurrentJointValueChanged();
}

void RobotAdvanceSettingWidget::buttonSetDefaultIndexClicked()
{
    RobotPreference rp=RobotClient::initance()->getRobotPreference();

    for(int i=0; i<6; i++){
        m_editReturnZeroIndex[i]->setText(QString::number(i+1));
        m_zeroJointIndex[static_cast<JOINTINDEX>(i)] = m_editReturnZeroIndex[i]->text().toInt();;
    }
   rp.setJointReturnSequence(m_zeroJointIndex);
   RobotClient::initance()->setRobotPreference(rp);
}
