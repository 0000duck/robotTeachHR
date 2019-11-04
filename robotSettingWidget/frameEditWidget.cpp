#include "frameEditWidget.h"


#include <QDebug>

using namespace std;
using namespace rclib;
using namespace robsoft;

FrameEditWidget::FrameEditWidget(QWidget *parent){
    initUI();
    initState();
    connectSignalSlot();
}

FrameEditWidget::~FrameEditWidget(){

}

void FrameEditWidget::setMode(int type, int frameType){
    m_type = type;
    m_frameType = frameType;

    groupJoint->show();
    groupPoint->show();
    m_buttonPointX->show();
    m_buttonPointY->show();
    m_buttonPointZ->show();
    m_buttonCalibrate->show();
    m_labelPre->show();
    m_editPre->show();

    m_editName->setReadOnly(false);
    m_editFrameX->setReadOnly(false);
    m_editFrameY->setReadOnly(false);
    m_editFrameZ->setReadOnly(false);
    m_editFrameA->setReadOnly(false);
    m_editFrameB->setReadOnly(false);
    m_editFrameC->setReadOnly(false);

    for(int i=0; i<10; i++){
        m_buttonJointPoint[i]->setIcon(QIcon());
        m_buttonJointPointFlag[i] = 0;
    }
    m_buttonPointOrigin->setIcon(QIcon());
    m_buttonPointOriginFlag = 0;
    m_buttonPointX->setIcon(QIcon());
    m_buttonPointXFlag = 0;
    m_buttonPointY->setIcon(QIcon());
    m_buttonPointYFlag = 0;
    m_buttonPointZ->setIcon(QIcon());
    m_buttonPointZFlag = 0;

    QString frameTypeStr;
    switch(m_frameType){
    case 0: frameTypeStr = tr("工具坐标系");
        break;
    case 1: frameTypeStr = tr("工件坐标系");
        break;
    default:
        break;
    }

    switch(m_type){
    case 0:{
        m_radioFrameMode[0]->setText(QString(tr("添加")) + frameTypeStr);
        m_radioFrameMode[0]->show();
        m_radioFrameMode[1]->hide();
        m_radioFrameMode[2]->hide();
        setRadioFrameModeChecked(0);

        groupJoint->hide();
        groupPoint->hide();
        m_buttonCalibrate->hide();
        m_labelPre->hide();
        m_editPre->hide();

        m_editName->setText("");

        m_editFrameX->setText("0");
        m_editFrameY->setText("0");
        m_editFrameZ->setText("0");
        m_editFrameA->setText("0");
        m_editFrameB->setText("0");
        m_editFrameC->setText("0");
    }
        break;
    case 1:{
        m_radioFrameMode[0]->setText(QString(tr("修改")) + frameTypeStr);
        m_radioFrameMode[0]->show();
        m_radioFrameMode[1]->hide();
        m_radioFrameMode[2]->hide();
        setRadioFrameModeChecked(0);

        m_editName->setReadOnly(true);
        groupJoint->hide();
        groupPoint->hide();
        m_buttonCalibrate->hide();
        m_labelPre->hide();
        m_editPre->hide();

        m_editName->setText(QString::fromStdString(m_FrameName));

        m_editFrameX->setText(QString("%1").arg((m_temp)[m_FrameName][TERMINAL_X]));
        m_editFrameY->setText(QString("%1").arg((m_temp)[m_FrameName][TERMINAL_Y]));
        m_editFrameZ->setText(QString("%1").arg((m_temp)[m_FrameName][TERMINAL_Z]));
        m_editFrameA->setText(QString("%1").arg((m_temp)[m_FrameName][TERMINAL_A]));
        m_editFrameB->setText(QString("%1").arg((m_temp)[m_FrameName][TERMINAL_B]));
        m_editFrameC->setText(QString("%1").arg((m_temp)[m_FrameName][TERMINAL_C]));
    }
        break;
    case 2:{
        switch(m_frameType){
        case 0:{
            m_radioFrameMode[0]->setText(QString(tr("TCP标定")));
            m_radioFrameMode[1]->setText(QString(tr("TCP+Z标定")));
            m_radioFrameMode[2]->setText(QString(tr("TCP+Z/X标定")));
            m_radioFrameMode[0]->show();
            m_radioFrameMode[1]->show();
            m_radioFrameMode[2]->show();
            setRadioFrameModeChecked(0);
            radioFrameModeChanged();

            m_editFrameX->setReadOnly(true);
            m_editFrameY->setReadOnly(true);
            m_editFrameZ->setReadOnly(true);
            m_editFrameA->setReadOnly(true);
            m_editFrameB->setReadOnly(true);
            m_editFrameC->setReadOnly(true);
        }
            break;
        case 1:{
            m_radioFrameMode[0]->setText(QString(tr("工件坐标系标定")));
            m_radioFrameMode[0]->show();
            m_radioFrameMode[1]->hide();
            m_radioFrameMode[2]->hide();
            setRadioFrameModeChecked(0);

            groupJoint->hide();
            m_buttonPointZ->hide();
            m_labelPre->hide();
            m_editPre->hide();
            m_editFrameX->setReadOnly(true);
            m_editFrameY->setReadOnly(true);
            m_editFrameZ->setReadOnly(true);
            m_editFrameA->setReadOnly(true);
            m_editFrameB->setReadOnly(true);
            m_editFrameC->setReadOnly(true);
        }
            break;
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
}

void FrameEditWidget::passParameter(RobotFrame *m_frame, std::string m_name){
    m_RobotFrame=m_frame;
    m_FrameName=m_name;
    if(m_frameType==0)
    {
       m_temp=(*m_RobotFrame).getToolFrameList();
    }else
    {
       m_temp=(*m_RobotFrame).getWorkFrameList();
    }
}

void FrameEditWidget::initUI(){
    QHBoxLayout* layoutFrameMode = new QHBoxLayout();
    for(int i=0; i<3; i++){
        m_radioFrameMode[i] = new QRadioButton();
        layoutFrameMode->addWidget(m_radioFrameMode[i]);
    }

    m_buttonCalibrate = new PushButton(tr("标定"));
    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    QHBoxLayout* layoutCalibrate = new QHBoxLayout();
    layoutCalibrate->addWidget(m_buttonCalibrate);
    layoutCalibrate->addWidget(m_buttonYes);
    layoutCalibrate->addWidget(m_buttonNo);

    m_buttonCalibrate->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonYes->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_labelPre = new QLabel(tr("标定\n精度:"));
    m_editPre = new QLineEdit();
    m_editPre->setReadOnly(true);
    QHBoxLayout* layoutPre = new QHBoxLayout();
    layoutPre->addWidget(m_labelPre);
    layoutPre->addWidget(m_editPre);

    m_labelName = new QLabel(tr("坐标\n系名:"));
    m_editName = new QLineEdit();
    QHBoxLayout* layoutName = new QHBoxLayout();
    layoutName->addWidget(m_labelName);
    layoutName->addWidget(m_editName);

    m_editPre->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_editName->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);

    m_labelFrameX = new QLabel("X:");
    m_editFrameX = new QLineEdit();
    m_editFrameX->setReadOnly(true);
    m_labelFrameY = new QLabel("Y:");
    m_editFrameY = new QLineEdit();
    m_editFrameY->setReadOnly(true);
    m_labelFrameZ = new QLabel("Z:");
    m_editFrameZ = new QLineEdit();
    m_editFrameZ->setReadOnly(true);
    m_labelFrameA = new QLabel("A:");
    m_editFrameA = new QLineEdit();
    m_editFrameA->setReadOnly(true);
    m_labelFrameB = new QLabel("B:");
    m_editFrameB = new QLineEdit();
    m_editFrameB->setReadOnly(true);
    m_labelFrameC = new QLabel("C:");
    m_editFrameC = new QLineEdit();
    m_editFrameC->setReadOnly(true);
    QHBoxLayout* layoutFrameX = new QHBoxLayout();
    layoutFrameX->addWidget(m_labelFrameX);
    layoutFrameX->addWidget(m_editFrameX);
    QHBoxLayout* layoutFrameY = new QHBoxLayout();
    layoutFrameY->addWidget(m_labelFrameY);
    layoutFrameY->addWidget(m_editFrameY);
    QHBoxLayout* layoutFrameZ = new QHBoxLayout();
    layoutFrameZ->addWidget(m_labelFrameZ);
    layoutFrameZ->addWidget(m_editFrameZ);
    QHBoxLayout* layoutFrameA = new QHBoxLayout();
    layoutFrameA->addWidget(m_labelFrameA);
    layoutFrameA->addWidget(m_editFrameA);
    QHBoxLayout* layoutFrameB = new QHBoxLayout();
    layoutFrameB->addWidget(m_labelFrameB);
    layoutFrameB->addWidget(m_editFrameB);
    QHBoxLayout* layoutFrameC = new QHBoxLayout();
    layoutFrameC->addWidget(m_labelFrameC);
    layoutFrameC->addWidget(m_editFrameC);

    QHBoxLayout* layoutFrame1 = new QHBoxLayout();
    layoutFrame1->addLayout(layoutName);
    layoutFrame1->addLayout(layoutPre);

    QHBoxLayout* layoutFrame2 = new QHBoxLayout();
    layoutFrame2->addLayout(layoutFrameX);
    layoutFrame2->addLayout(layoutFrameA);

    QHBoxLayout* layoutFrame3 = new QHBoxLayout();
    layoutFrame3->addLayout(layoutFrameY);
    layoutFrame3->addLayout(layoutFrameB);

    QHBoxLayout* layoutFrame4 = new QHBoxLayout();
    layoutFrame4->addLayout(layoutFrameZ);
    layoutFrame4->addLayout(layoutFrameC);

    QVBoxLayout* layoutFrame = new QVBoxLayout();
    layoutFrame->addLayout(layoutFrame1);
    layoutFrame->addLayout(layoutFrame2);
    layoutFrame->addLayout(layoutFrame3);
    layoutFrame->addLayout(layoutFrame4);

    m_editFrameX->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_editFrameY->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_editFrameZ->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_editFrameA->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_editFrameB->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_editFrameC->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);

    groupNameFrame = new QGroupBox();
    groupNameFrame->setLayout(layoutFrame);
    groupNameFrame->setFixedWidth(0.3*WINDOWWIDTH);

    m_buttonPointOrigin = new PushButton(tr("坐标原点选取"));
    m_buttonPointX = new PushButton(tr("坐标X轴选取"));
    m_buttonPointY = new PushButton(tr("坐标Y轴选取"));
    m_buttonPointZ = new PushButton(tr("坐标Z轴选取"));
    QVBoxLayout* layoutPoint = new QVBoxLayout();
    layoutPoint->addWidget(m_buttonPointOrigin);
    layoutPoint->addWidget(m_buttonPointX);
    layoutPoint->addWidget(m_buttonPointY);
    layoutPoint->addWidget(m_buttonPointZ);
    groupPoint = new QGroupBox();
    groupPoint->setLayout(layoutPoint);
    groupPoint->setFixedWidth(0.12*WINDOWWIDTH);

    m_buttonPointOrigin->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_buttonPointX->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_buttonPointY->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    m_buttonPointZ->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);

    for(int i=0; i<10; i++){
        m_buttonJointPoint[i] = new PushButton();
        m_buttonJointPoint[i]->setIndex(i);
        m_buttonJointPoint[i]->setText(QString(tr("工具点")) + QString::number(i+1, 10) + QString(tr("选取")));
        m_buttonJointPoint[i]->setFixedSize(0.1*WINDOWWIDTH, 0.035*WINDOWWIDTH);
    }
    QVBoxLayout* layoutJointPoint1 = new QVBoxLayout();
    QVBoxLayout* layoutJointPoint2 = new QVBoxLayout();
    for(int i=0; i<5; i++){
        layoutJointPoint1->addWidget(m_buttonJointPoint[i]);
    }
    for(int i=5; i<10; i++){
        layoutJointPoint2->addWidget(m_buttonJointPoint[i]);
    }
    QHBoxLayout* layoutJoint = new QHBoxLayout();
    layoutJoint->addLayout(layoutJointPoint1);
    layoutJoint->addLayout(layoutJointPoint2);
    groupJoint = new QGroupBox();
    groupJoint->setLayout(layoutJoint);
    groupJoint->setFixedWidth(0.25*WINDOWWIDTH);

    QHBoxLayout* layoutJointPoint = new QHBoxLayout();
    layoutJointPoint->addWidget(groupJoint);
    layoutJointPoint->addWidget(groupPoint);
    layoutJointPoint->addWidget(groupNameFrame);
    //    layoutJointPoint->setStretchFactor(groupJoint, 2);
    //    layoutJointPoint->setStretchFactor(groupPoint, 1);
    //    layoutJointPoint->setStretchFactor(groupNameFrame, 1);

    QVBoxLayout* layoutFrameEdit = new QVBoxLayout(this);
    layoutFrameEdit->addLayout(layoutFrameMode);
    layoutFrameEdit->addLayout(layoutJointPoint);
    layoutFrameEdit->addLayout(layoutCalibrate);
    layoutFrameEdit->setStretchFactor(layoutFrameMode, 1);
    layoutFrameEdit->setStretchFactor(layoutJointPoint, 8);
    layoutFrameEdit->setStretchFactor(layoutCalibrate, 1);
}

void FrameEditWidget::initState(){
    m_editPre->setReadOnly(true);

    m_editFrameX->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    m_editFrameY->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    m_editFrameZ->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    m_editFrameA->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    m_editFrameB->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    m_editFrameC->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
}

void FrameEditWidget::connectSignalSlot(){
    for(int i=0; i<3; i++){
        QObject::connect(m_radioFrameMode[i], SIGNAL(toggled(bool)), this, SLOT(radioFrameModeChanged()));
    }
    for(int i=0; i<10; i++){
        QObject::connect(m_buttonJointPoint[i], SIGNAL(pushButtonClicked(int)), this, SLOT(buttonJointPointClicked(int)));
    }
    QObject::connect(m_buttonPointOrigin, SIGNAL(clicked()), this, SLOT(buttonPointOriginClicked()));
    QObject::connect(m_buttonPointX, SIGNAL(clicked()), this, SLOT(buttonPointXClicked()));
    QObject::connect(m_buttonPointY, SIGNAL(clicked()), this, SLOT(buttonPointYClicked()));
    QObject::connect(m_buttonPointZ, SIGNAL(clicked()), this, SLOT(buttonPointZClicked()));
    QObject::connect(m_buttonCalibrate, SIGNAL(clicked()), this, SLOT(buttonCalibrateClicked()));
    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));
}

void FrameEditWidget::radioFrameModeChanged(){
    if(m_radioFrameMode[0]->isChecked()){
        m_frameModeFlag = 0;
    }
    else if(m_radioFrameMode[1]->isChecked()){
        m_frameModeFlag = 1;
    }
    else if(m_radioFrameMode[2]->isChecked()){
        m_frameModeFlag = 2;
    }

    if(m_type == 2 && m_frameType == 0){    // calibration && tool frame
        switch(m_frameModeFlag){
        case 0:{
            groupPoint->hide();
            m_buttonPointX->show();
            m_buttonPointY->show();
        }
            break;
        case 1:{
            groupPoint->show();
            m_buttonPointX->hide();
            m_buttonPointY->hide();
        }
            break;
        case 2:{
            groupPoint->show();
            m_buttonPointX->show();
            m_buttonPointY->hide();
        }
            break;
        default:
            break;
        }
    }
}

void FrameEditWidget::buttonJointPointClicked(int index){
    if(m_buttonJointPointFlag[index] == 0){
        m_buttonJointPoint[index]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("free.png")));
        m_buttonJointPointFlag[index] = 1;
        m_jointPoint[index]=RobotClient::initance()->getRobotMotion().getCurrentJointPosition();
    }
    else{
        m_buttonJointPoint[index]->setIcon(QIcon());
        m_buttonJointPointFlag[index] = 0;
    }
}

void FrameEditWidget::buttonPointOriginClicked(){
    if(m_buttonPointOriginFlag == 0){
        m_buttonPointOrigin->setIcon(QIcon(QString(WINDOWICONPATH) + QString("free.png")));
        m_buttonPointOriginFlag = 1;
        if(m_frameType == 0){   // tcp
            m_jointOrigin=RobotClient::initance()->getRobotMotion().getCurrentJointPosition();
        }
        else{
            m_pointOrigin=RobotClient::initance()->getRobotMotion().getCurrentTerminal();
        }
    }
    else{
        m_buttonPointOrigin->setIcon(QIcon());
        m_buttonPointOriginFlag = 0;
    }
}

void FrameEditWidget::buttonPointXClicked(){
    if(m_buttonPointXFlag == 0){
        m_buttonPointX->setIcon(QIcon(QString(WINDOWICONPATH) + QString("free.png")));
        m_buttonPointXFlag = 1;
        if(m_frameType == 0){   // tcp
            m_jointX=RobotClient::initance()->getRobotMotion().getCurrentJointPosition();
        }
        else{
            m_pointX=RobotClient::initance()->getRobotMotion().getCurrentTerminal();
        }
    }
    else{
        m_buttonPointX->setIcon(QIcon());
        m_buttonPointXFlag= 0;
    }
}

void FrameEditWidget::buttonPointYClicked(){
    if(m_buttonPointYFlag == 0){
        m_buttonPointY->setIcon(QIcon(QString(WINDOWICONPATH) + QString("free.png")));
        m_buttonPointYFlag = 1;
        m_pointY=RobotClient::initance()->getRobotMotion().getCurrentTerminal();
    }
    else{
        m_buttonPointY->setIcon(QIcon());
        m_buttonPointYFlag = 0;
    }
}

void FrameEditWidget::buttonPointZClicked(){
    if(m_buttonPointZFlag == 0){
        m_buttonPointZ->setIcon(QIcon(QString(WINDOWICONPATH) + QString("free.png")));
        m_buttonPointZFlag = 1;
        if(m_frameType == 0){   // tcp
            m_jointZ=RobotClient::initance()->getRobotMotion().getCurrentJointPosition();
        }
        else{
            m_pointZ=RobotClient::initance()->getRobotMotion().getCurrentTerminal();
        }
    }
    else{
        m_buttonPointZ->setIcon(QIcon());
        m_buttonPointZFlag = 0;
    }
}

void FrameEditWidget::buttonCalibrateClicked(){
    Terminal frame;
    if(m_frameType == 1){   // user frame
        if(m_buttonPointOriginFlag != 1 || m_buttonPointXFlag != 1 || m_buttonPointYFlag != 1){
            QMessageBox::question(this, tr("提示"),
                                  tr("没有足够的采样点!"),
                                  QMessageBox::Yes);
            return;
        }

        if((m_pointOrigin == m_pointX) || (m_pointOrigin == m_pointY) || (m_pointX == m_pointY)){
            QMessageBox::question(this, tr("提示"),
                                  tr("采样点重复!"),
                                  QMessageBox::Yes);
            return;
        }

        RobotClient::initance()->calibrateUSRF(m_pointOrigin, m_pointX, m_pointY, frame);
    }
    else{
        int count = 0;
        vector<Joints> jointList;
        for(int i=0; i<10; i++){
            count += m_buttonJointPointFlag[i];
            if(m_buttonJointPointFlag[i]){
                jointList.push_back(m_jointPoint[i]);
            }
        }
        if(count < 3){
            QMessageBox::question(this, tr("提示"),
                                  tr("TCP标定至少需要三个末端点!"),
                                  QMessageBox::Yes);
        }

        double pre;
        switch(m_frameModeFlag){
        case 0:{

            for(int i = 0;i < jointList.size();i++){
                Joints fir = jointList[i];
                for(int j = i+1;j < jointList.size();j++){
                    if(fir == jointList[j]){
                        QMessageBox::question(this, tr("提示"),
                                              tr("采样点重复!"),
                                              QMessageBox::Yes);
                        return;
                    }
                }
            }

            pre = RobotClient::initance()->calibrateTCP(jointList, frame);
        }
            break;
        case 1:{
            if(m_buttonPointOriginFlag != 1 || m_buttonPointZFlag != 1){
                QMessageBox::question(this, tr("提示"),
                                      tr("没有足够的采样点!"),
                                      QMessageBox::Yes);
                return;
            }

            for(int i = 0;i < jointList.size();i++){
                Joints fir = jointList[i];
                for(int j = i+1;j < jointList.size();j++){
                    if(fir == jointList[j] || fir == m_jointOrigin || fir == m_jointZ){
                        QMessageBox::question(this, tr("提示"),
                                              tr("采样点重复!"),
                                              QMessageBox::Yes);
                        return;
                    }
                }
            }

            pre = RobotClient::initance()->calibrateTCFZ(jointList, m_jointOrigin, m_jointZ, frame);
        }
            break;
        case 2:{
            if(m_buttonPointOriginFlag != 1 || m_buttonPointZFlag != 1 || m_buttonPointXFlag != 1){
                QMessageBox::question(this, tr("提示"),
                                      tr("没有足够的采样点!"),
                                      QMessageBox::Yes);
                return;
            }

            for(int i = 0;i < jointList.size();i++){
                Joints fir = jointList[i];
                for(int j = i+1;j < jointList.size();j++){
                    if(fir == jointList[j] || fir == m_jointOrigin || fir == m_jointX || fir == m_jointZ){
                        QMessageBox::question(this, tr("提示"),
                                              tr("采样点重复!"),
                                              QMessageBox::Yes);
                        return;
                    }
                }
            }

            pre = RobotClient::initance()->calibrateTCFX(jointList, m_jointOrigin, m_jointX, m_jointZ, frame);
        }
            break;
        default:
            break;
        }
        m_editPre->setText(QString("%1").arg(pre));
    }
    m_editFrameX->setText(QString("%1").arg(frame[TERMINAL_X]));
    m_editFrameY->setText(QString("%1").arg(frame[TERMINAL_Y]));
    m_editFrameZ->setText(QString("%1").arg(frame[TERMINAL_Z]));
    m_editFrameA->setText(QString("%1").arg(frame[TERMINAL_A]));
    m_editFrameB->setText(QString("%1").arg(frame[TERMINAL_B]));
    m_editFrameC->setText(QString("%1").arg(frame[TERMINAL_C]));
}

void FrameEditWidget::buttonYesClicked(){
    if(m_editName->text().isEmpty()){
        QMessageBox::question(this, tr("提示"),
                              tr("坐标系名不能为空!"),
                              QMessageBox::Yes);
        return;
    }
//   ^[A-Za-z0-9]+$
    if(m_editName->text().indexOf(QRegExp("^[A-Za-z_][A-Za-z0-9_]*$")) == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("坐标系名格式错误!"),
                              QMessageBox::Yes);
        return;
    }

    bool isDigit;
    m_editName->text().toInt(&isDigit);
    if(isDigit){
        QMessageBox::question(this, tr("提示"),
                              tr("坐标系名格式错误!"),
                              QMessageBox::Yes);
        return;
    }

    if(m_editFrameX->text().isEmpty() || m_editFrameY->text().isEmpty() || m_editFrameZ->text().isEmpty() ||
            m_editFrameA->text().isEmpty() || m_editFrameB->text().isEmpty() || m_editFrameC->text().isEmpty()){
        QMessageBox::question(this, tr("提示"),
                              tr("坐标系取值不能为空!"),
                              QMessageBox::Yes);
        return;
    }

    if(m_type != 1 && (m_temp).find(m_editName->text().toStdString()) != (m_temp).end()){    // add frame with exit name
        QMessageBox::question(this, tr("提示"),
                              tr("坐标系名已经存在!"),
                              QMessageBox::Yes);
        return;
    }

    string name = m_editName->text().toStdString();

    Terminal frame;
    frame[TERMINAL_X] = m_editFrameX->text().toDouble();
    frame[TERMINAL_Y] = m_editFrameY->text().toDouble();
    frame[TERMINAL_Z] = m_editFrameZ->text().toDouble();
    frame[TERMINAL_A] = m_editFrameA->text().toDouble();
    frame[TERMINAL_B] = m_editFrameB->text().toDouble();
    frame[TERMINAL_C] = m_editFrameC->text().toDouble();

    if(m_type == 1){
        if(m_frameType==0)
        {
           (*m_RobotFrame).modifyToolFrame(name,frame);
        }
        else
        {
           (*m_RobotFrame).modifyWorkFrame(name,frame);
        }
    }
    else{
        if(m_frameType==0)
        {
            (*m_RobotFrame).addToolFrame(name,frame); 
        }
        else
        {
           (*m_RobotFrame).addWorkFrame(name,frame);
        }
    }

    emit frameEditFinishedSignal(true);
}

void FrameEditWidget::buttonNoClicked(){
    emit frameEditFinishedSignal(false);
}

void FrameEditWidget::setRadioFrameModeChecked(int index){
    for(int i=0; i<3; i++){
        m_radioFrameMode[i]->setChecked(false);
    }
    m_radioFrameMode[index]->setChecked(true);
    m_frameModeFlag = index;
}
