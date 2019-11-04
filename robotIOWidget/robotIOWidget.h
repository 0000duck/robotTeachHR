#ifndef ROBOTIOWIDGET_H
#define ROBOTIOWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QTimer>

#include <iostream>
#include <vector>
#include <cmath>

#include "../normalFunc/macroDefine.h"

#include "../pushButton/pushButton.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotStructure/robotStructure.h"

class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QLabel;
class PushButton;
class QLineEdit;
class QSlider;
class RobotIOWidget:public QWidget{
    Q_OBJECT
public:
    RobotIOWidget(QWidget *parent = NULL);
    ~RobotIOWidget();

    void passState(rclib::SYSPLAYSTATE,rclib::SWITCHSTATE);
    void updateSystemState();
    void updateRobotIOWidget();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

private:
    PushButton* m_buttonDigitalInput[DIGITAL_INPUTPORT_NUM];
    QLabel* m_labelDigitalInput[DIGITAL_INPUTPORT_NUM];
    QGroupBox* m_groupDigitalInput;

    PushButton* m_buttonDigitalOutput[DIGITAL_OUTPUTPORT_NUM];
    QLabel* m_labelDigitalOutput[DIGITAL_OUTPUTPORT_NUM];
    QGroupBox* m_groupDigitalOutput;

    QLabel* m_labelAnalogInput[ANALOG_INPUT_NUM];
    QSlider* m_sliderAnalogInput[ANALOG_INPUT_NUM];
    QLineEdit* m_editAnalogInput[ANALOG_INPUT_NUM];
    QGroupBox* m_groupAnalogInput;

    QLabel* m_labelAnalogOutput[ANALOG_OUTPUT_NUM];
    QSlider* m_sliderAnalogOutput[ANALOG_OUTPUT_NUM];
    QLineEdit* m_editAnalogOutput[ANALOG_OUTPUT_NUM];
    QGroupBox* m_groupAnalogOutput;

    QLineEdit* m_editModbusInput;
    QGroupBox* m_groupModbusInput;

    QLineEdit* m_editModbusOutput;
    QGroupBox* m_groupModbusOutput;

    rclib::RobotIO m_rio;

    rclib::SYSPLAYSTATE local_playStateFlag;
    rclib::SWITCHSTATE local_controlStateFlag;
private slots:
    void buttonDigitalInputClicked();
    void buttonDigitalOutputClicked(int);

    void sliderAnalogInputChanged(int);
    void sliderAnalogOutputChanged(int);
};

#endif
