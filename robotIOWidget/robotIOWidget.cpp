#include"robotIOWidget.h"

#include <QDebug>

using namespace std;
using namespace rclib;

RobotIOWidget::RobotIOWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Button, BACKGROUNDCOLOR);
    pe.setColor(QPalette::Background, BACKGROUNDCOLOR);
    this->setPalette(pe);

    initUI();
    initState();
    connectSignalSlot();
}

RobotIOWidget::~RobotIOWidget(){

}

void RobotIOWidget::passState(rclib::SYSPLAYSTATE playStateFlag,SWITCHSTATE controlStateFlag)
{
    local_playStateFlag = playStateFlag;
    local_controlStateFlag = controlStateFlag;
}

void RobotIOWidget::updateSystemState()
{
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_TEACH){
        m_groupDigitalInput->setEnabled(true);
        m_groupDigitalOutput->setEnabled(true);
        m_groupAnalogInput->setEnabled(true);
        m_groupAnalogOutput->setEnabled(true);
        m_groupModbusInput->setEnabled(true);
        m_groupModbusOutput->setEnabled(true);
    }
    else{
        m_groupDigitalInput->setEnabled(false);
        m_groupDigitalOutput->setEnabled(false);
        m_groupAnalogInput->setEnabled(false);
        m_groupAnalogOutput->setEnabled(false);
        m_groupModbusInput->setEnabled(false);
        m_groupModbusOutput->setEnabled(false);
    }
}

void RobotIOWidget::initUI(){
    for(int i=0; i<DIGITAL_INPUTPORT_NUM; i++){
        m_buttonDigitalInput[i] = new PushButton();
        m_buttonDigitalInput[i]->setIndex(i);
        m_buttonDigitalInput[i]->setFixedSize(50, 50);

        m_labelDigitalInput[i] = new QLabel();
        if(i < 9){
            m_labelDigitalInput[i]->setText(QString("DI0") + QString::number(i+1, 10) + QString(":"));
        }
        else{
            m_labelDigitalInput[i]->setText(QString("DI") + QString::number(i+1, 10) + QString(":"));
        }
    }

    QHBoxLayout* layoutDigitalInputH[DIGITAL_INPUTPORT_NUM/2];
    for(int i=0; i<DIGITAL_INPUTPORT_NUM/2; i++){
        layoutDigitalInputH[i] = new QHBoxLayout();
        layoutDigitalInputH[i]->addWidget(m_labelDigitalInput[i]);
        layoutDigitalInputH[i]->addWidget(m_buttonDigitalInput[i]);
        layoutDigitalInputH[i]->addWidget(m_labelDigitalInput[i + DIGITAL_INPUTPORT_NUM/2]);
        layoutDigitalInputH[i]->addWidget(m_buttonDigitalInput[i + DIGITAL_INPUTPORT_NUM/2]);
    }

    QVBoxLayout* layoutDigitalInput = new QVBoxLayout();
    for(int i=0; i<DIGITAL_INPUTPORT_NUM/2; i++){
        layoutDigitalInput->addLayout(layoutDigitalInputH[i]);
    }

    m_groupDigitalInput = new QGroupBox(tr("数字输入"));
    m_groupDigitalInput->setLayout(layoutDigitalInput);

    for(int i=0; i<DIGITAL_OUTPUTPORT_NUM; i++){
        m_buttonDigitalOutput[i] = new PushButton();
        m_buttonDigitalOutput[i]->setIndex(i);
        m_buttonDigitalOutput[i]->setFixedSize(50, 50);

        m_labelDigitalOutput[i] = new QLabel();
        if(i < 9){
            m_labelDigitalOutput[i]->setText(QString("DO0") + QString::number(i+1, 10) + QString(":"));
        }
        else{
            m_labelDigitalOutput[i]->setText(QString("DO") + QString::number(i+1, 10) + QString(":"));
        }
    }

    QHBoxLayout* layoutDigitalOutputH[DIGITAL_OUTPUTPORT_NUM/2];
    for(int i=0; i<DIGITAL_OUTPUTPORT_NUM/2; i++){
        layoutDigitalOutputH[i] = new QHBoxLayout();
        layoutDigitalOutputH[i]->addWidget(m_labelDigitalOutput[i]);
        layoutDigitalOutputH[i]->addWidget(m_buttonDigitalOutput[i]);
        layoutDigitalOutputH[i]->addWidget(m_labelDigitalOutput[i + DIGITAL_OUTPUTPORT_NUM/2]);
        layoutDigitalOutputH[i]->addWidget(m_buttonDigitalOutput[i + DIGITAL_OUTPUTPORT_NUM/2]);
    }

    QVBoxLayout* layoutDigitalOutput = new QVBoxLayout();
    for(int i=0; i<DIGITAL_INPUTPORT_NUM/2; i++){
        layoutDigitalOutput->addLayout(layoutDigitalOutputH[i]);
    }

    m_groupDigitalOutput = new QGroupBox(tr("数字输出"));
    m_groupDigitalOutput->setLayout(layoutDigitalOutput);

    for(int i=0; i<ANALOG_INPUT_NUM; i++){
        m_labelAnalogInput[i] = new QLabel();
        m_labelAnalogInput[i]->setText(QString("AI") + QString::number(i+1, 10) + QString(":"));

        m_sliderAnalogInput[i] = new QSlider(Qt::Horizontal);
        m_sliderAnalogInput[i]->setMaximumHeight(40);
        m_sliderAnalogInput[i]->setRange(0, 1000);
        m_sliderAnalogInput[i]->setEnabled(false);
        m_sliderAnalogInput[i]->setStyleSheet(
        "QSlider::groove:horizontal {border:none; margin-top: 0px; margin-bottom: 0px; height: 5px;}"
        "QSlider::sub-page {background: lightgray;}"
        "QSlider::add-page {background: lightgray;}"
        "QSlider::handle {background: white; border: 5px solid gray; width: 25px; margin: -16px 0;}"
        );

        m_editAnalogInput[i] = new QLineEdit();
        m_editAnalogInput[i]->setFixedSize(40, 40);
        m_editAnalogInput[i]->setText("0.00");
        m_editAnalogInput[i]->setReadOnly(true);
    }

    QHBoxLayout* layoutAnalogInputH[ANALOG_INPUT_NUM];
    for(int i=0; i<ANALOG_INPUT_NUM; i++){
        layoutAnalogInputH[i] = new QHBoxLayout();
        layoutAnalogInputH[i]->addWidget(m_labelAnalogInput[i]);
        layoutAnalogInputH[i]->addWidget(m_sliderAnalogInput[i]);
        layoutAnalogInputH[i]->addWidget(m_editAnalogInput[i]);
    }

    QVBoxLayout* layoutAnalogInput = new QVBoxLayout();
    for(int i=0; i<ANALOG_INPUT_NUM; i++){
        layoutAnalogInput->addLayout(layoutAnalogInputH[i]);
    }

    m_groupAnalogInput = new QGroupBox(tr("模拟输入"));
    m_groupAnalogInput->setLayout(layoutAnalogInput);

    for(int i=0; i<ANALOG_INPUT_NUM; i++){
        m_labelAnalogOutput[i] = new QLabel();
        m_labelAnalogOutput[i]->setText(QString("AO") + QString::number(i+1, 10) + QString(":"));

        m_sliderAnalogOutput[i] = new QSlider(Qt::Horizontal);
        m_sliderAnalogOutput[i]->setMaximumHeight(40);
        m_sliderAnalogOutput[i]->setRange(0, 1000);
        m_sliderAnalogOutput[i]->setStyleSheet(
        "QSlider::groove:horizontal {border:none; margin-top: 0px; margin-bottom: 0px; height: 5px;}"
        "QSlider::sub-page {background: lightgray;}"
        "QSlider::add-page {background: lightgray;}"
        "QSlider::handle {background: white; border: 5px solid gray; width: 25px; margin: -16px 0;}"
        );

        m_editAnalogOutput[i] = new QLineEdit();
        m_editAnalogOutput[i]->setFixedSize(40, 40);
        m_editAnalogOutput[i]->setText("0.00");
        m_editAnalogOutput[i]->setReadOnly(true);
    }

    QHBoxLayout* layoutAnalogOutputH[ANALOG_OUTPUT_NUM];
    for(int i=0; i<ANALOG_OUTPUT_NUM; i++){
        layoutAnalogOutputH[i] = new QHBoxLayout();
        layoutAnalogOutputH[i]->addWidget(m_labelAnalogOutput[i]);
        layoutAnalogOutputH[i]->addWidget(m_sliderAnalogOutput[i]);
        layoutAnalogOutputH[i]->addWidget(m_editAnalogOutput[i]);
    }

    QVBoxLayout* layoutAnalogOutput = new QVBoxLayout();
    for(int i=0; i<ANALOG_OUTPUT_NUM; i++){
        layoutAnalogOutput->addLayout(layoutAnalogOutputH[i]);
    }

    m_groupAnalogOutput = new QGroupBox(tr("模拟输出"));
    m_groupAnalogOutput->setLayout(layoutAnalogOutput);

    QVBoxLayout* layoutAnalog = new QVBoxLayout();
    layoutAnalog->addWidget(m_groupAnalogInput);
    layoutAnalog->addWidget(m_groupAnalogOutput);

    m_editModbusInput = new QLineEdit();

    QVBoxLayout* layoutModbusInput = new QVBoxLayout();
    layoutModbusInput->addWidget(m_editModbusInput);

    m_groupModbusInput = new QGroupBox(tr("Modbus输入"));
    m_groupModbusInput->setLayout(layoutModbusInput);

    m_editModbusOutput = new QLineEdit();

    QVBoxLayout* layoutModbusOutput = new QVBoxLayout();
    layoutModbusOutput->addWidget(m_editModbusOutput);

    m_groupModbusOutput = new QGroupBox(tr("Modbus输出"));
    m_groupModbusOutput->setLayout(layoutModbusOutput);

    QVBoxLayout* layoutModbus = new QVBoxLayout();
    layoutModbus->addWidget(m_groupModbusInput);
    layoutModbus->addWidget(m_groupModbusOutput);

    QHBoxLayout* layoutIO = new QHBoxLayout(this);
    layoutIO->addWidget(m_groupDigitalInput);
    layoutIO->addWidget(m_groupDigitalOutput);
    layoutIO->addLayout(layoutAnalog);
    layoutIO->addLayout(layoutModbus);
}

void RobotIOWidget::initState(){
    m_rio = RobotClient::initance()->getRobotIO();
    for(int i=0; i<DIGITAL_INPUTPORT_NUM; i++){
            if(m_rio.getDigitalInputState(static_cast<PORTINDEX>(i)) == SWITCHON)
                m_buttonDigitalInput[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
            else
                m_buttonDigitalInput[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
    }

    for(int i=0; i<DIGITAL_OUTPUTPORT_NUM; i++){
            if(m_rio.getDigitalOutputState(static_cast<PORTINDEX>(i)) == SWITCHON)
                m_buttonDigitalOutput[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
            else
                m_buttonDigitalOutput[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
    }

    for(int i=0; i<ANALOG_INPUT_NUM; i++){
        double value = m_rio.getAnalogInputState()[static_cast<PORTINDEX>(i)];
        m_sliderAnalogInput[i]->setValue(value*100);
        m_editAnalogInput[i]->setText(QString::number(value, 10, 2));
    }

    for(int i=0; i<ANALOG_OUTPUT_NUM; i++){
        double value = m_rio.getAnalogOutputState()[static_cast<PORTINDEX>(i)];
        m_sliderAnalogOutput[i]->setValue(value*100);
        m_editAnalogOutput[i]->setText(QString::number(value, 10, 2));
    }
}

void RobotIOWidget::connectSignalSlot(){
    for(int i=0; i<DIGITAL_INPUTPORT_NUM; i++){
        QObject::connect(m_buttonDigitalInput[i], SIGNAL(pushButtonClicked(int)), this, SLOT(buttonDigitalInputClicked()));
    }
    for(int i=0; i<DIGITAL_OUTPUTPORT_NUM; i++){
        QObject::connect(m_buttonDigitalOutput[i], SIGNAL(pushButtonClicked(int)), this, SLOT(buttonDigitalOutputClicked(int)));
    }
    for(int i=0; i<ANALOG_INPUT_NUM; i++){
        QObject::connect(m_sliderAnalogInput[i], SIGNAL(valueChanged(int)), this, SLOT(sliderAnalogInputChanged(int)));
    }
    for(int i=0; i<ANALOG_OUTPUT_NUM; i++){
        QObject::connect(m_sliderAnalogOutput[i], SIGNAL(valueChanged(int)), this, SLOT(sliderAnalogOutputChanged(int)));
    }
}

void RobotIOWidget::updateRobotIOWidget(){
    updateSystemState();

    RobotIO tmp_robotIO = RobotClient::initance()->getRobotIO();

    for(int i=0; i<DIGITAL_INPUTPORT_NUM; i++){
        if(tmp_robotIO.getDigitalInputState(static_cast<PORTINDEX>(i)) !=  m_rio.getDigitalInputState()[static_cast<PORTINDEX>(i)]){
            if(tmp_robotIO.getDigitalInputState(static_cast<PORTINDEX>(i))==SWITCHON)
                m_buttonDigitalInput[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
            else
                m_buttonDigitalInput[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
        }
    }

    for(int i=0; i<DIGITAL_OUTPUTPORT_NUM; i++){
        if(tmp_robotIO.getDigitalOutputState(static_cast<PORTINDEX>(i)) !=  m_rio.getDigitalOutputState()[static_cast<PORTINDEX>(i)]){
            if(tmp_robotIO.getDigitalOutputState(static_cast<PORTINDEX>(i))==SWITCHON)
                m_buttonDigitalOutput[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
            else
                m_buttonDigitalOutput[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
        }
    }

    for(int i=0; i<ANALOG_INPUT_NUM; i++){
        if(fabs(tmp_robotIO.getAnalogInputState()[static_cast<PORTINDEX>(i)] - m_rio.getAnalogInputState()[static_cast<PORTINDEX>(i)]) > 0.01){
            m_sliderAnalogInput[i]->setValue(tmp_robotIO.getAnalogInputState()[static_cast<PORTINDEX>(i)]*100);
        }
    }

    if(local_controlStateFlag == SWITCHOFF || local_playStateFlag != SYSPLAY_TEACH){
        for(int i=0; i<ANALOG_OUTPUT_NUM; i++){
            if(fabs(tmp_robotIO.getAnalogOutputState()[static_cast<PORTINDEX>(i)] - m_rio.getAnalogOutputState()[static_cast<PORTINDEX>(i)]) > 0.01){
                m_sliderAnalogOutput[i]->setValue(tmp_robotIO.getAnalogOutputState()[static_cast<PORTINDEX>(i)]*100);
            }
        }
    }

     m_rio = tmp_robotIO;
}

void RobotIOWidget::buttonDigitalInputClicked(){
    RobotClient::initance()->getDigitalInput(PORT_1);
}

void RobotIOWidget::buttonDigitalOutputClicked(int index){
    SWITCHSTATE state = RobotClient::initance()->getDigitalOutput(PORTINDEX(index));
    if(state==SWITCHON){
        RobotClient::initance()->setDigitalOutput(static_cast<PORTINDEX>(index), SWITCHOFF);
    }
    else{
        RobotClient::initance()->setDigitalOutput(static_cast<PORTINDEX>(index), SWITCHON);
    }
}

void RobotIOWidget::sliderAnalogInputChanged(int j){
    QSlider* slider=qobject_cast<QSlider*>(sender());
    for(int i=0; i<ANALOG_INPUT_NUM; i++){
        if(slider==m_sliderAnalogInput[i])
        {
            m_editAnalogInput[i]->setText(QString::number(j / 100.0, 10, 2));
        }
    }
}

void RobotIOWidget::sliderAnalogOutputChanged(int j){
    QSlider* slider=qobject_cast<QSlider*>(sender());
    for(int i=0; i<ANALOG_OUTPUT_NUM; i++){
        if(slider==m_sliderAnalogOutput[i])
        {
            m_editAnalogOutput[i]->setText(QString::number(j / 100.0, 10, 2));
            RobotClient::initance()->setAnalogOutput(static_cast<PORTINDEX>(i),j / 100.0);
        }
    }
}
