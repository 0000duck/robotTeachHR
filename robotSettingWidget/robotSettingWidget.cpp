#include "robotSettingWidget.h"

#include <QDebug>

using namespace rclib;

RobotSettingWidget::RobotSettingWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, "#808080");
    this->setPalette(pe);

    initUI();
    initState();
    connectSignalSlot();
}

RobotSettingWidget::~RobotSettingWidget(){

}

void RobotSettingWidget::passState(rclib::SYSPLAYSTATE playStateFlag,SWITCHSTATE controlStateFlag)
{
    local_playStateFlag = playStateFlag;
    local_controlStateFlag = controlStateFlag;
}

void RobotSettingWidget::initUI(){
    m_tabSetting = new QTabWidget();
    m_tabSetting->setStyleSheet("QTabBar::tab { height: 30px; width: 110px; }");

    QHBoxLayout* layoutSetting = new QHBoxLayout(this);
    layoutSetting->addWidget(m_tabSetting);

    m_widgetRobotParameterSetting = new RobotParameterSettingWidget(this);
    m_tabSetting->addTab(m_widgetRobotParameterSetting, tr("机器人设置"));
    m_widgetRobotFrameSetting = new RobotFrameSettingWidget(this);
    m_tabSetting->addTab(m_widgetRobotFrameSetting, tr("坐标系设置"));
    m_widgetRobotAdvanceSetting = new RobotAdvanceSettingWidget(this);
    m_tabSetting->addTab(m_widgetRobotAdvanceSetting, tr("高级设置"));
}

void RobotSettingWidget::initState(){

}

void RobotSettingWidget::connectSignalSlot(){

}

void RobotSettingWidget::updateRobotSetting()
{
    updateSystemState();

    if(!RobotClient::initance()->bSetFileUpdate())
        return;

    m_widgetRobotParameterSetting->updateRobotParameterWidget();
    m_widgetRobotFrameSetting->updateAllFrame();
    m_widgetRobotAdvanceSetting->updateAdvanceSettingWidget();
}

void RobotSettingWidget::updateAdvanceCurrentJoints()
{
    m_widgetRobotAdvanceSetting->updateCurrentPosition();
}

void RobotSettingWidget::updateRobotAdvanceSettingLight()
{
    m_widgetRobotAdvanceSetting->updateRobotLight();
}

void RobotSettingWidget::updateSystemState(){
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_TEACH){
        m_widgetRobotParameterSetting->updateOperationEnabled(true);
        m_widgetRobotFrameSetting->updateOperationEnabled(true);
        m_widgetRobotAdvanceSetting->updateGroupEnabled(true);
    }
    else{
        m_widgetRobotParameterSetting->updateOperationEnabled(false);
        m_widgetRobotFrameSetting->updateOperationEnabled(false);
        m_widgetRobotAdvanceSetting->updateGroupEnabled(false);
    }
}
