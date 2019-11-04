#ifndef ROBOTSETTINGWIDGET_H
#define ROBOTSETTINGWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>

#include "robotParameterSettingWidget.h"
#include "robotFrameSettingWidget.h"
#include "robotAdvanceSettingWidget.h"

#include "../normalFunc/macroDefine.h"

class QWidget;
class QTabWidget;
class RobotSettingWidget:public QWidget{
    Q_OBJECT
public:
    RobotSettingWidget(QWidget *parent = NULL);
    ~RobotSettingWidget();

    void passState(rclib::SYSPLAYSTATE,rclib::SWITCHSTATE);
    void updateSystemState();

    void updateRobotSetting();
    void updateAdvanceCurrentJoints();
    void updateRobotAdvanceSettingLight();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    rclib::SYSPLAYSTATE local_playStateFlag;
    rclib::SWITCHSTATE local_controlStateFlag;

private:
    QTabWidget* m_tabSetting;
    RobotParameterSettingWidget* m_widgetRobotParameterSetting;
    RobotFrameSettingWidget* m_widgetRobotFrameSetting;
    RobotAdvanceSettingWidget* m_widgetRobotAdvanceSetting;
};

#endif
