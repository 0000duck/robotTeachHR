#include "mainWindow.h"

using namespace std;
using namespace rclib;

MainWindow::MainWindow(){
    this->setWindowTitle(tr("恒锐机器人"));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);  //hide the title bar

    this->setFixedSize(1280, 800);   //set the size of window
    this->move(0, 0);                               //set the position

    initUI();

    initState();
    initParameter();
    connectSignalSlot();
}

MainWindow::~MainWindow(){

}

void MainWindow::showEvent(QShowEvent *)
{
    this->setFocus();
}

void MainWindow::initUI(){
    m_buttonServo = new PushButton(tr("伺服\n状态"));
    m_buttonServo->setFixedHeight(0.05*WINDOWWIDTH);
    m_buttonServo->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));

    m_buttonTeachPlayMode = new PushButton(tr("示教模式"));
    m_buttonTeachPlayMode->setFixedHeight(0.05*WINDOWWIDTH);

    QButtonGroup* m_buttonGroup = new QButtonGroup();
    m_buttonGroup->setExclusive(true);
    m_buttonMoveSpace = new PushButton(tr("关节\n坐标系"));
    m_buttonMoveSpace->setFixedHeight(0.05*WINDOWWIDTH);
    QHBoxLayout* layoutJoint[6];
    for(int i=0; i<6; i++){
        m_buttonJointMinus[i] = new PushButton();
        m_buttonJointMinus[i]->setIndex(i);
        m_buttonJointMinus[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("minus.png")));
        m_buttonJointPlus[i] = new PushButton();
        m_buttonJointPlus[i]->setIndex(i);
        m_buttonJointPlus[i]->setIcon(QIcon(QString(WINDOWICONPATH) + QString("plus.png")));

        m_buttonJointMinus[i]->setFixedSize(0.05*WINDOWWIDTH, 0.05*WINDOWWIDTH);
        m_buttonJointPlus[i]->setFixedSize(0.05*WINDOWWIDTH, 0.05*WINDOWWIDTH);

        m_labelJoint[i] = new QLabel();
        QFont ft;
        ft.setPointSize(12);
        m_labelJoint[i]->setFont(ft);
        m_labelJoint[i]->setText(QString("J")+ QString::number(i+1, 10));

        layoutJoint[i] = new QHBoxLayout();
        layoutJoint[i]->addWidget(m_buttonJointMinus[i]);
        layoutJoint[i]->addWidget(m_labelJoint[i]);
        layoutJoint[i]->addWidget(m_buttonJointPlus[i]);
        m_buttonGroup->addButton(m_buttonJointMinus[i]);
        m_buttonGroup->addButton(m_buttonJointPlus[i]);
    }

    QHBoxLayout* layoutFrameServo = new QHBoxLayout();
    layoutFrameServo->addWidget(m_buttonMoveSpace);
    layoutFrameServo->addWidget(m_buttonServo);

    m_widgetMainMotion = new QWidget();
    m_widgetMainMotion->setMaximumWidth(0.15*WINDOWWIDTH);
    QVBoxLayout* layoutMainMotion = new QVBoxLayout(m_widgetMainMotion);
    layoutMainMotion->addLayout(layoutFrameServo);
    for(int i=0; i<6; i++){
        layoutMainMotion->addLayout(layoutJoint[i]);
    }

    m_buttonProgramRun = new PushButton(tr("程序运行"));
    m_buttonProgramRun->setFixedHeight(0.05*WINDOWWIDTH);
    m_buttonProgramRun->setIcon(QIcon(QString(WINDOWICONPATH) + QString("right.png")));

    m_buttonPauseContinue = new PushButton(tr("程序\n暂停"));
    m_buttonPauseContinue->setFixedHeight(0.05*WINDOWWIDTH);
    m_buttonPauseContinue->setIcon(QIcon(QString(WINDOWICONPATH) + QString("pause.png")));

    m_buttonStop = new PushButton(tr("程序\n停止"));
    m_buttonStop->setFixedHeight(0.05*WINDOWWIDTH);
    m_buttonStop->setIcon(QIcon(QString(WINDOWICONPATH) + QString("stop.png")));

    m_buttonStartDragTeach = new PushButton(tr("开始\n拖动"));
    m_buttonStartDragTeach->setFixedSize(0.05*WINDOWWIDTH, 0.05*WINDOWWIDTH);
    m_buttonStartDragTeach->setIcon(QIcon(QString(WINDOWICONPATH) + QString("dragStart.png")));

    m_buttonEndDragTeach = new PushButton(tr("结束\n拖动"));
    m_buttonEndDragTeach->setFixedSize(0.05*WINDOWWIDTH, 0.05*WINDOWWIDTH);
    m_buttonEndDragTeach->setIcon(QIcon(QString(WINDOWICONPATH) + QString("dragEnd.png")));

    QHBoxLayout* layoutProgramPauseStop = new QHBoxLayout();
    layoutProgramPauseStop->addWidget(m_buttonPauseContinue);
    layoutProgramPauseStop->addWidget(m_buttonStop);

    QHBoxLayout* layoutDragTeach = new QHBoxLayout();
    layoutDragTeach->addWidget(m_buttonStartDragTeach);
    layoutDragTeach->addWidget(m_buttonEndDragTeach);

    m_widgetMainProgram = new QWidget();
    m_widgetMainProgram->setMaximumWidth(0.15*WINDOWWIDTH);
    QVBoxLayout* layoutMainProgram = new QVBoxLayout(m_widgetMainProgram);
    layoutMainProgram->addWidget(m_buttonProgramRun);
    layoutMainProgram->addLayout(layoutProgramPauseStop);

    m_pixLogo.load(QString(WINDOWICONPATH) + QString("logo.png"));

    m_buttonShutDownSystem=new PushButton();
    m_buttonShutDownSystem->setFixedSize(100,100);
    m_buttonShutDownSystem->setIcon(m_pixLogo);
    m_buttonShutDownSystem->setIconSize(QSize(100,100));
    m_buttonShutDownSystem->setFlat(true);

    QWidget* widgetTeachPlay = new QWidget;
    QHBoxLayout* layoutTeachPlay = new QHBoxLayout(widgetTeachPlay);
    layoutTeachPlay->addWidget(m_buttonTeachPlayMode);

    m_widgetMainMenu = new QWidget();
    QVBoxLayout* layoutMainMenu = new QVBoxLayout(m_widgetMainMenu);
    layoutMainMenu->addWidget(widgetTeachPlay, 1);
    layoutMainMenu->addWidget(m_widgetMainProgram, 2);
    layoutMainMenu->addWidget(m_widgetMainMotion, 7);
    layoutMainMenu->addWidget(m_buttonShutDownSystem, 2, Qt::AlignCenter);

    m_pixScreenSaverLogo.load(QString(WINDOWICONPATH) + QString("logo-full.png"));
    m_labelScreenSaverLogo = new QLabel();
    m_labelScreenSaverLogo->setFixedHeight(200);
    m_labelScreenSaverLogo->setFixedWidth(m_pixScreenSaverLogo.width()/m_pixScreenSaverLogo.height() * 200);
    m_labelScreenSaverLogo->setScaledContents(true);
    m_labelScreenSaverLogo->clear();
    m_labelScreenSaverLogo->setPixmap(m_pixScreenSaverLogo);
    m_labelScreenSaverLogo->show();
    m_widgetScreenSaver = new QWidget();
    QHBoxLayout* layoutScreenSaverLogo = new QHBoxLayout(m_widgetScreenSaver);
    layoutScreenSaverLogo->addWidget(m_labelScreenSaverLogo, 0, Qt::AlignCenter);

    m_buttonNormalDebugMode = new PushButton(tr("调试模式"));
    m_buttonNormalDebugMode->setMinimumHeight(40);
    m_buttonNormalDebugMode->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
//    m_buttonVirtualMahcineMode = new PushButton(tr("虚拟模式"));
//    m_buttonVirtualMahcineMode->setMinimumHeight(40);
//    m_buttonVirtualMahcineMode->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
//    m_buttonVirtualMahcineMode->hide();
    m_buttonProgramStepMode = new PushButton(tr("设置断点"));
    m_buttonProgramStepMode->setMinimumHeight(40);
    m_buttonProgramStepMode->setIcon(QIcon(QString(WINDOWICONPATH) + QString("stepin.png")));
    m_buttonProgramStep = new PushButton(tr("单步前进"));
    m_buttonProgramStep->setMinimumHeight(40);
    m_buttonProgramBack = new PushButton(tr("单步后退"));
    m_buttonProgramBack->setMinimumHeight(40);
    m_buttonProgramStepPause = new PushButton(tr("单步暂停"));
    m_buttonProgramStepPause->setMinimumHeight(40);
    m_buttonProgramStepPause->setIcon(QIcon(QString(WINDOWICONPATH) + QString("stepout.png")));
    m_buttonMoveMode = new PushButton(tr("点动模式"));
    m_buttonMoveMode->setMinimumHeight(40);

    m_labelStepDistance = new QLabel(tr("步进\n距离:"));
    m_editStepDistance = new QLineEdit();
    m_labelStepDistance->setFixedSize(40, 40);
    m_editStepDistance->setFixedSize(40, 40);
    QHBoxLayout* layoutStepDistance = new QHBoxLayout();
    layoutStepDistance->addWidget(m_labelStepDistance);
    layoutStepDistance->addWidget(m_editStepDistance);

    m_labelVel = new QLabel(tr("速度%:"));
    m_sliderVel = new QSlider(Qt::Horizontal);
    m_sliderVel->setMinimumHeight(40);
    m_sliderVel->setRange(1, 100);
    m_sliderVel->setStyleSheet(
    "QSlider::groove:horizontal {border:none; margin-top: 0px; margin-bottom: 0px; height: 5px;}"
    "QSlider::sub-page {background: lightgray;}"
    "QSlider::add-page {background: lightgray;}"
    "QSlider::handle {background: white; border: 5px solid gray; width: 25px; margin: -16px 0;}"
    );
    m_editVel = new QLineEdit();
    m_editVel->setFixedSize(40, 40);
    m_editVel->setReadOnly(true);
    m_buttonVelMinus = new PushButton();
    m_buttonVelMinus->setFixedSize(40, 40);
    m_buttonVelMinus->setIcon(QIcon(QString(WINDOWICONPATH) + QString("left.png")));
    m_buttonVelPlus = new PushButton();
    m_buttonVelPlus->setFixedSize(40, 40);
    m_buttonVelPlus->setIcon(QIcon(QString(WINDOWICONPATH) + QString("right.png")));
    QHBoxLayout* layoutVel = new QHBoxLayout();
    layoutVel->addWidget(m_labelVel);
    layoutVel->addWidget(m_buttonVelMinus);
    layoutVel->addWidget(m_sliderVel);
    layoutVel->addWidget(m_buttonVelPlus);
    layoutVel->addWidget(m_editVel);

    m_widgetMainMode = new QWidget();
    QHBoxLayout* layoutMainMode = new QHBoxLayout(m_widgetMainMode);
    layoutMainMode->addLayout(layoutVel, 4);
//    layoutMainMode->addWidget(m_buttonVirtualMahcineMode, 1);
    layoutMainMode->addWidget(m_buttonNormalDebugMode, 1);
    layoutMainMode->addWidget(m_buttonProgramStepMode, 1);
    layoutMainMode->addWidget(m_buttonProgramStep, 1);
    layoutMainMode->addWidget(m_buttonProgramBack, 1);
    layoutMainMode->addWidget(m_buttonProgramStepPause, 1);
    layoutMainMode->addWidget(m_buttonMoveMode, 1);
    layoutMainMode->addLayout(layoutStepDistance, 1);

    m_mainFunc = new QTabWidget();
    m_mainFunc->setTabPosition(QTabWidget::West);
    m_mainFunc->tabBar()->setStyle(new CustomTabStyle);

    m_buttonClearError = new PushButton(tr("清除错误"));
    m_buttonClearError->setFixedSize(62, 30);

    m_labelTime = new QLabel("0000-00-00 00:00:00");

    m_version = new QLabel();
    m_version->setMaximumWidth(0.15*WINDOWWIDTH);
    m_version->setText(tr("版本:")+"1.1.0");

    m_tableNewInformation = new QTableWidget();
    QHBoxLayout* layoutInfomation = new QHBoxLayout();
    layoutInfomation->addWidget(m_buttonClearError);
    layoutInfomation->addWidget(m_tableNewInformation);
    layoutInfomation->addWidget(m_labelTime);
    layoutInfomation->addWidget(m_version);

    m_widgetMainFunc = new QWidget();
    QVBoxLayout* layoutMainFunc = new QVBoxLayout(m_widgetMainFunc);
    layoutMainFunc->addWidget(m_mainFunc);
    layoutMainFunc->addWidget(m_widgetMainMode);
    layoutMainFunc->addLayout(layoutInfomation);

    QHBoxLayout* layoutMainWindow = new QHBoxLayout(this);
    layoutMainWindow->addWidget(m_widgetMainFunc);
    layoutMainWindow->addWidget(m_widgetMainMenu);
    layoutMainWindow->addWidget(m_widgetScreenSaver);

    m_widgetMainMenu->show();
    m_widgetMainFunc->show();
    m_widgetScreenSaver->hide();

    QString error ;
    read_cache_Error(error);

    if(error == "false"){
        QString name("");
        write_cache_program(name);
    }else if(error == "true"){
        QString tmp("false");
        write_cache_Error(tmp);
    }

    m_widgetRobotTeach = new RobotTeachWidget();
    m_widgetRobotTeach->setFocusPolicy(Qt::ClickFocus);
    m_mainFunc->addTab(m_widgetRobotTeach, tr("模型"));
    m_mainFunc->setFocusPolicy(Qt::ClickFocus);
    m_widgetRobotPorgram = new RobotProgramWidget();
    m_mainFunc->addTab(m_widgetRobotPorgram, tr("程序"));
    m_widgetRobotIO = new RobotIOWidget();
    m_widgetRobotIO->setFocusPolicy(Qt::ClickFocus);
    m_mainFunc->addTab(m_widgetRobotIO, "I/O");
    m_widgetRobotSetting = new RobotSettingWidget();
    m_widgetRobotSetting->setFocusPolicy(Qt::ClickFocus);
    m_mainFunc->addTab(m_widgetRobotSetting, tr("设置"));
    m_widgetRobotInfo = new RobotInfoWidget();
    m_widgetRobotInfo->setFocusPolicy(Qt::ClickFocus);
    m_mainFunc->addTab(m_widgetRobotInfo, tr("消息"));
    m_widgetPaint = new PaintWidget();
    m_widgetPaint->setFocusPolicy(Qt::ClickFocus);
    m_mainFunc->addTab(m_widgetPaint, tr("绘制"));

    m_mainFunc->setCurrentIndex(0);

    m_stepMode = false;
}

void MainWindow::initState(){
    m_editStepDistance->setText("30");
    m_editStepDistance->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));

    initNewInfoTable();

    m_buttonMoveSpace->setStateSum(4);
    m_buttonMoveMode->setStateSum(2);

    QString language;
    read_cache_language(language);
    if(language == QString("English")){
        RobotClient::initance()->sendSZYY(ENGLISH);
    }
    else{
        RobotClient::initance()->sendSZYY(CHINESE);
    }
}

void MainWindow::initNewInfoTable(){
    m_tableNewInformation->setFixedHeight(30);
    m_tableNewInformation->setColumnCount(1);
    //    m_tableNewInformation->setRowCount(1);
//    m_tableNewInformation->horizontalHeader()->setDefaultSectionSize(150);
    m_tableNewInformation->horizontalHeader()->setSectionsClickable(false);

    QStringList header;
    header<<tr("时间")<<tr("消息类型")<<tr("消息内容");
    m_tableNewInformation->setHorizontalHeaderLabels(header);

    QFont font = m_tableNewInformation->horizontalHeader()->font();
    font.setBold(true);
    m_tableNewInformation->horizontalHeader()->setFont(font);
    m_tableNewInformation->horizontalHeader()->setStyleSheet("QHeaderView::section{background:white;}"); //设置表头背景色
    m_tableNewInformation->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableNewInformation->horizontalHeader()->setVisible(false);

//    m_tableNewInformation->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableNewInformation->verticalHeader()->setDefaultSectionSize(30); //设置行高
    m_tableNewInformation->verticalHeader()->setVisible(false); //设置垂直头不可见
    m_tableNewInformation->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    m_tableNewInformation->setStyleSheet("selection-background-color:white;"); //设置选中背景色
    m_tableNewInformation->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableNewInformation->setSelectionMode(QAbstractItemView::SingleSelection);

    //设置水平、垂直滚动条样式
    m_tableNewInformation->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                                "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                                "QScrollBar::handle:hover{background:gray;}"
                                                                "QScrollBar::sub-line{background:transparent;}"
                                                                "QScrollBar::add-line{background:transparent;}");
    m_tableNewInformation->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                              "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                              "QScrollBar::handle:hover{background:gray;}"
                                                              "QScrollBar::sub-line{background:transparent;}"
                                                              "QScrollBar::add-line{background:transparent;}");
}

void MainWindow::initParameter(){  
    m_controlStateFlag = RobotClient::initance()->getControlState();
    m_RobotState = RobotClient::initance()->getRobotState();

    local_controlStateFlag = m_controlStateFlag;
    local_State = m_RobotState;

    local_runStateFlag = local_State.getRunState();          //运行状态
    local_playStateFlag = local_State.getPlayState();         //运行模式
    local_servoStateFlag = local_State.getServoState();       //伺服状态
    local_virtualStateFlag = local_State.getVirtualState();     //虚拟模式
    local_debugStateFlag = local_State.getDebugState();       //调试模式

    switch(local_playStateFlag){
    case SYSPLAY_TEACH:{
        m_buttonTeachPlayMode->setText(tr("示教模式"));
    }
        break;
    case SYSPLAY_PLAY:{
        m_buttonTeachPlayMode->setText(tr("再现模式"));
    }
        break;
    case SYSPLAY_REMOTE:{
        m_buttonTeachPlayMode->setText(tr("远程模式"));
    }
        break;
    default:
        break;
    }

    updateDebugState(local_debugStateFlag);
    updateSendState(local_runStateFlag);
    updateServoState(local_servoStateFlag);

    double vel = RobotClient::initance()->getRobotState().getVel();
    m_sliderVel->setValue(vel*100);
    int value = m_sliderVel->value();
    m_editVel->setText(QString::number(value, 10));
    setGlobalVel(vel);

    RobotStateChange();

    m_timer = new QTimer(this);
    m_timer->start(100);
}

void MainWindow::connectSignalSlot(){
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(updateAllState()));

    QObject::connect(this, SIGNAL(signalRobotStateChange()), this, SLOT(RobotStateChange()));

    QObject::connect(m_widgetRobotPorgram, SIGNAL(showRecomInfoSignal(QString)), this, SLOT(showRecomInfoSlot(QString)));

    QObject::connect(m_buttonMoveSpace, SIGNAL(clicked()), this, SLOT(buttonMoveSpaceClicked()));

    QObject::connect(m_buttonProgramStepMode, SIGNAL(clicked()), this, SLOT(buttonProgramStepModeClicked()));
    QObject::connect(m_buttonProgramRun, SIGNAL(clicked()), this, SLOT(buttonProgramRunClicked()));
    QObject::connect(m_buttonProgramStep, SIGNAL(clicked()), this, SLOT(buttonProgramStepClicked()));
    QObject::connect(m_buttonProgramBack, SIGNAL(clicked()), this, SLOT(buttonProgramBackClicked()));
    QObject::connect(m_buttonProgramStepPause, SIGNAL(clicked()), this, SLOT(buttonProgramStepPauseClicked()));
    QObject::connect(m_buttonPauseContinue, SIGNAL(clicked()), this, SLOT(buttonPauseContinueClicked()));
    QObject::connect(m_buttonStop, SIGNAL(clicked()), this, SLOT(buttonStopClicked()));
    QObject::connect(m_buttonServo, SIGNAL(clicked()), this, SLOT(buttonServoClicked()));
    QObject::connect(m_buttonClearError, SIGNAL(clicked()), this, SLOT(buttonClearErrorClicked()));
    QObject::connect(m_buttonStartDragTeach, SIGNAL(clicked()), this, SLOT(buttonStartDragTeachClicked()));
    QObject::connect(m_buttonEndDragTeach, SIGNAL(clicked()), this, SLOT(buttonEndDragTeachClicked()));

    QObject::connect(m_buttonTeachPlayMode, SIGNAL(clicked()), this, SLOT(buttonTeachPlayModeClicked()));
    QObject::connect(m_buttonNormalDebugMode, SIGNAL(clicked()), this, SLOT(buttonNormalDebugModeClicked()));
//    QObject::connect(m_buttonVirtualMahcineMode, SIGNAL(clicked()), this, SLOT(buttonVirtualMahcineModeClicked()));
    QObject::connect(m_buttonMoveMode, SIGNAL(clicked()), this, SLOT(buttonMoveModeClicked()));

    for(int i=0; i<6; i++){
        QObject::connect(m_buttonJointMinus[i], SIGNAL(pushButtonPressed(int)), this, SLOT(buttonJointMinusPressed(int)));
        QObject::connect(m_buttonJointPlus[i], SIGNAL(pushButtonPressed(int)), this, SLOT(buttonJointPlusPressed(int)));
        QObject::connect(m_buttonJointMinus[i], SIGNAL(pushButtonReleased(int)), this, SLOT(buttonJointReleased(int)));
        QObject::connect(m_buttonJointPlus[i], SIGNAL(pushButtonReleased(int)), this, SLOT(buttonJointReleased(int)));
    }

    QObject::connect(m_sliderVel, SIGNAL(valueChanged(int)), this, SLOT(sliderVelChanged()));
    QObject::connect(m_buttonVelMinus, SIGNAL(clicked()), this, SLOT(buttonVelMinusClicked()));
    QObject::connect(m_buttonVelPlus, SIGNAL(clicked()), this, SLOT(buttonVelPlusClicked()));

    QObject::connect(m_buttonShutDownSystem, SIGNAL(clicked()), this, SLOT(buttonshutdownSystemClicked()));
}

void MainWindow::updateAllState(){
    updateRobotState();

    if(local_runStateFlag == SYSRUN_RUN){
        m_widgetRobotPorgram->updateRobotProgramWidget();
    }

    m_widgetRobotTeach->passState(local_playStateFlag,local_controlStateFlag);
    m_widgetRobotTeach->updateRobotMotionWidget();

    switch(m_mainFunc->currentIndex()){
    case 1:{
        m_widgetRobotPorgram->passState(local_playStateFlag,local_controlStateFlag);
        m_widgetRobotPorgram->updateSystemState();
    }
        break;
    case 2:{
        m_widgetRobotIO->passState(local_playStateFlag,local_controlStateFlag);
        m_widgetRobotIO->updateRobotIOWidget();
    }
        break;
    case 3:{
        m_widgetRobotSetting->passState(local_playStateFlag,local_controlStateFlag);
        m_widgetRobotSetting->updateRobotSetting();
        m_widgetRobotSetting->updateAdvanceCurrentJoints();
        m_widgetRobotSetting->updateRobotAdvanceSettingLight();
    }
        break;
    case 5:{
        m_widgetPaint->passState(local_playStateFlag,local_controlStateFlag);
        m_widgetPaint->updatePaintWidget();
    }
        break;
    default:
        break;
    }

    updateCurrentTimeShow();
}

void MainWindow::updateRobotState(){
    INFOSTATE rsInfoType=INFO_NONE;
    string rsInfoString;

    RobotClient::initance()->getInfo(rsInfoType,rsInfoString);
    updateNewInfo(rsInfoType, rsInfoString);

    bool JUD = 0;

    m_controlStateFlag = RobotClient::initance()->getControlState();
    m_RobotState = RobotClient::initance()->getRobotState();
    m_runStateFlag = m_RobotState.getRunState();        //运行状态
    m_playStateFlag = m_RobotState.getPlayState();         //运行模式
    m_servoStateFlag = m_RobotState.getServoState();       //伺服状态
    m_virtualStateFlag = m_RobotState.getVirtualState();     //虚拟模式
    m_debugStateFlag = m_RobotState.getDebugState();       //调试模式

    if(local_controlStateFlag != m_controlStateFlag){
        local_controlStateFlag = m_controlStateFlag;
        JUD = 1;
    }

    if(local_playStateFlag != m_playStateFlag){
        local_playStateFlag = m_playStateFlag;
        updateTeachPlayMode(local_playStateFlag);
        JUD = 1;
    }

    if(local_debugStateFlag != m_debugStateFlag){
        local_debugStateFlag = m_debugStateFlag;
        updateDebugState(local_debugStateFlag);
        JUD = 1;
    }

    if(local_runStateFlag != m_runStateFlag){
        local_runStateFlag = m_runStateFlag;
        updateSendState(local_runStateFlag);
        JUD = 1;
    }

    if(local_servoStateFlag != m_servoStateFlag){
        local_servoStateFlag = m_servoStateFlag;
        updateServoState(local_servoStateFlag);
        JUD = 1;
    }

    if(local_virtualStateFlag != m_virtualStateFlag){
        local_virtualStateFlag = m_virtualStateFlag;
        JUD = 1;
    }

    if(JUD == 1){
        emit signalRobotStateChange();
    }

    local_State = m_RobotState;
}

void MainWindow::updateSendState(SYSRUNSTATE state){
    switch(state){
    case SYSRUN_STOP:{
        if(m_playStateFlag == SYSPLAY_PLAY){
            m_buttonPauseContinue->setText(tr("程序\n暂停"));
        }
    }
        break;
    case SYSRUN_RUN:{
        if(m_playStateFlag == SYSPLAY_PLAY){
            m_buttonPauseContinue->setText(tr("程序\n暂停"));
        }
    }
        break;
    case SYSRUN_PAUSE:{
        if(m_playStateFlag == SYSPLAY_PLAY){
            m_buttonPauseContinue->setText(tr("程序\n继续"));
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::updateTeachPlayMode(SYSPLAYSTATE state){
    switch(state){
    case SYSPLAY_TEACH:{
        m_buttonTeachPlayMode->setText(tr("示教模式"));
        m_sliderVel->setValue(5);
        sliderVelChanged();
    }
        break;
    case SYSPLAY_PLAY:{
        m_buttonTeachPlayMode->setText(tr("再现模式"));
        m_sliderVel->setValue(5);
        sliderVelChanged();
    }
        break;
    case SYSPLAY_REMOTE:{
        m_buttonTeachPlayMode->setText(tr("远程模式"));
        m_sliderVel->setValue(100);
        sliderVelChanged();
    }
        break;
    default:
        break;
    }
}

void MainWindow::updateNewInfo(INFOSTATE state, string scontent){
    if(state != INFO_ERROR && state != INFO_WARNING && state != INFO_RECOM){
        return;
    }

    QString content;
    if(is_str_utf8(scontent.c_str())){
        content = scontent.c_str();
    }else{
        auto* codec = QTextCodec::codecForName("GB18030");
        content  = codec->toUnicode(scontent.c_str());
    }

    m_widgetRobotInfo->updateNewInfo(state, content);

    m_tableNewInformation->clearContents();
    m_tableNewInformation->setRowCount(0);

    m_tableNewInformation->insertRow(0);

    for(int i=0; i<1; i++){
        QTableWidgetItem* item = new QTableWidgetItem();
        switch(state){
        case INFO_ERROR:{
            item->setIcon(QIcon(QString(WINDOWICONPATH) + QString("error.png")));
        }
            break;
        case INFO_WARNING:{
            item->setIcon(QIcon(QString(WINDOWICONPATH) + QString("warn.png")));
        }
            break;
        case INFO_RECOM:{
            item->setIcon(QIcon(QString(WINDOWICONPATH) + QString("info.png")));
        }
            break;
        default:
            break;
        }

        item->setText(content);

        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        m_tableNewInformation->setItem(0, 0, item);
    }

    if(content == QString(tr("限位回零过程成功")) || content == QString(tr("限位回零过程失败，请重新回零"))){
        QMessageBox::question(this, tr("提示"),
                              content,
                              QMessageBox::Yes);
    }
}

void MainWindow::buttonProgramRunClicked(){
    m_widgetRobotPorgram->programLoad();
    RobotClient::initance()->programRun();
}

void MainWindow::buttonProgramStepModeClicked()
{
    m_widgetRobotPorgram->setBreakPoint(true);
    RobotClient::initance()->programStepRun();
    if(m_stepMode){
        m_stepMode = false;
    }else{
        m_stepMode = true;
    }
    emit signalRobotStateChange();
}

void MainWindow::buttonProgramBackClicked()
{
    m_widgetRobotPorgram->setBreakPoint(false);
    RobotClient::initance()->programStepRun();
}

void MainWindow::buttonProgramStepClicked(){
    RobotClient::initance()->programStepRun();
}

void MainWindow::buttonProgramStepPauseClicked(){
    RobotClient::initance()->programStepPause();
}

void MainWindow::buttonPauseContinueClicked(){
    if(m_runStateFlag == SYSRUN_RUN){
        RobotClient::initance()->programPause(true);
    }
    else if(m_runStateFlag == SYSRUN_PAUSE){
        RobotClient::initance()->programPause(false);
    }
}

void MainWindow::buttonStopClicked(){
    RobotClient::initance()->programStop();
}

void MainWindow::buttonServoClicked(){
    if(local_servoStateFlag == SWITCHON){
        RobotClient::initance()->setServoState(SWITCHOFF);
    }
    else{
        RobotClient::initance()->setServoState(SWITCHON);
    }
}

void MainWindow::buttonClearErrorClicked(){
    RobotClient::initance()->setErrorClear();

    m_tableNewInformation->clearContents();
    m_tableNewInformation->setRowCount(0);
}

void MainWindow::buttonStartDragTeachClicked(){
    RobotClient::initance()->dragModeStart();
}

void MainWindow::buttonEndDragTeachClicked(){
    RobotClient::initance()->dragModeEnd();
    m_widgetRobotPorgram->updateDragPointList();
}

void MainWindow::buttonTeachPlayModeClicked(){
    if(local_playStateFlag == SYSPLAY_TEACH)
    {
        RobotClient::initance()->setPlayState(SYSPLAY_PLAY);
        RobotClient::initance()->setServoState(SWITCHON);
    }else if(local_playStateFlag == SYSPLAY_PLAY)
    {
        RobotClient::initance()->setPlayState(SYSPLAY_REMOTE);
        RobotClient::initance()->setServoState(SWITCHON);
    }else if(local_playStateFlag == SYSPLAY_REMOTE)
    {
        RobotClient::initance()->setPlayState(SYSPLAY_TEACH);
        RobotClient::initance()->setServoState(SWITCHOFF);
    }
    m_widgetRobotPorgram->updateWidget();
    m_stepMode = false;
}

void MainWindow::buttonNormalDebugModeClicked(){
    switch (local_debugStateFlag) {
    case SWITCHON:
        RobotClient::initance()->setDebugState(SWITCHOFF);
        break;
    case SWITCHOFF:
        RobotClient::initance()->setDebugState(SWITCHON);
        break;
    default:
        break;
    }
}

//void MainWindow::buttonVirtualMahcineModeClicked(){
//    switch (local_virtualStateFlag) {
//    case SWITCHON:
//        RobotClient::initance()->setTryRunState(SWITCHOFF);
//        break;
//    case SWITCHOFF:
//        RobotClient::initance()->setTryRunState(SWITCHON);
//        break;
//    default:
//        break;
//    }
//}

void MainWindow::showRecomInfoSlot(QString info){
    updateNewInfo(INFO_RECOM, info.toStdString());
}

void MainWindow::showWarnInfoSlot(QString info){
    updateNewInfo(INFO_WARNING, info.toStdString());
}

void MainWindow::showErrorInfoSlot(QString info){
    updateNewInfo(INFO_ERROR, info.toStdString());
}

void MainWindow::buttonMoveSpaceClicked(){
    m_buttonMoveSpace->incState();

    switch (m_buttonMoveSpace->returnState()) {
    case 0:
        m_buttonMoveSpace->setText(tr("关节\n坐标系"));
        break;
    case 1:
        m_buttonMoveSpace->setText(tr("基\n坐标系"));
        break;
    case 2:
        m_buttonMoveSpace->setText(tr("末端\n坐标系"));
        break;
    case 3:
        m_buttonMoveSpace->setText(tr("工件\n坐标系"));
        break;
    default:
        break;
    }

    m_widgetRobotTeach->setMoveSpace(m_buttonMoveSpace->returnState());
    if(m_buttonMoveSpace->returnState()){   // cartisian coordinate
        QString termianlStr[] = {"X ", "Y ", "Z ", "A ", "B ", "C "};
        for(int i=0; i<6; i++){
            m_labelJoint[i]->setText(termianlStr[i]);
        }

        if(RobotClient::initance()->getRobotParameter().getRobotType() == 2){
            m_buttonJointMinus[4]->setEnable(true);
            m_buttonJointMinus[5]->setEnable(true);
            m_buttonJointPlus[4]->setEnable(true);
            m_buttonJointPlus[5]->setEnable(true);
        }
    }
    else{
        for(int i=0; i<6; i++){
            m_labelJoint[i]->setText(QString("J")+ QString::number(i+1, 10));
        }
        if(RobotClient::initance()->getRobotParameter().getRobotType()  == 2){
            m_buttonJointMinus[4]->setEnable(false);
            m_buttonJointMinus[5]->setEnable(false);
            m_buttonJointPlus[4]->setEnable(false);
            m_buttonJointPlus[5]->setEnable(false);
        }
    }
}

void MainWindow::buttonMoveModeClicked(){
    m_buttonMoveMode->incState();

    if(m_buttonMoveMode->returnState() == 0){
        m_buttonMoveMode->setText(tr("点动模式"));
    }
    else{
        m_buttonMoveMode->setText(tr("步进模式"));
    }
}

void MainWindow::jointTerminalJogStep(int index, MOVEDIRECTION direction){
    double vel = m_sliderVel->value()/100.0;

    switch(m_buttonMoveSpace->returnState()){
    case 0:{
        if(m_buttonMoveMode->returnState() == 0){
            RobotClient::initance()->jointJOG(static_cast<JOINTINDEX>(index), direction, vel);
        }
        else{
            RobotClient::initance()->jointStep(static_cast<JOINTINDEX>(index), direction, m_editStepDistance->text().toDouble(), vel);
        }
    }
        break;
    case 1:{
        if(m_buttonMoveMode->returnState() == 0){
            RobotClient::initance()->terminalJOG(static_cast<TERMINALINDEX>(index), direction, vel, COORDINATE_BASE);
        }
        else{
            RobotClient::initance()->terminalStep(static_cast<TERMINALINDEX>(index), (direction), m_editStepDistance->text().toDouble(), vel, COORDINATE_BASE);
        }
    }
        break;
    case 2:{
        if(m_buttonMoveMode->returnState() == 0){
            RobotClient::initance()->terminalJOG(static_cast<TERMINALINDEX>(index), direction, vel, COORDINATE_TOOL);
        }
        else{
            RobotClient::initance()->terminalStep(static_cast<TERMINALINDEX>(index), direction, m_editStepDistance->text().toDouble(), vel, COORDINATE_TOOL);
        }
    }
        break;
    case 3:{
        if(m_buttonMoveMode->returnState() == 0){
            RobotClient::initance()->terminalJOG(TERMINALINDEX(index), direction, vel, COORDINATE_WORK);
        }
        else{
            RobotClient::initance()->terminalStep(TERMINALINDEX(index), direction, m_editStepDistance->text().toDouble(), vel, COORDINATE_WORK);
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::buttonJointMinusPressed(int index){
    jointTerminalJogStep(index, MOVE_NEGATIVE);
}

void MainWindow::buttonJointPlusPressed(int index){
    jointTerminalJogStep(index, MOVE_POSITIVE);
}

void MainWindow::buttonJointReleased(int){
    if(m_buttonMoveMode->returnState() == 0){ // jog
        RobotClient::initance()->stopJOG();
    }
}

void MainWindow::updateCurrentTimeShow(){
    QDateTime timeCurrent = QDateTime::currentDateTime();
    QString time = timeCurrent.toString("yyyy-MM-dd hh:mm:ss");
    m_labelTime->setText(time);
}

void MainWindow::updateDebugState(SWITCHSTATE state)
{
    switch (state) {
    case SWITCHOFF:
        m_buttonNormalDebugMode->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
        break;
    case SWITCHON:
        m_buttonNormalDebugMode->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
        break;
    default:
        break;
    }
}

void MainWindow::updateServoState(SWITCHSTATE state)
{
    switch (state) {
    case SWITCHON:
        m_buttonServo->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servoon.png")));
        break;
    case SWITCHOFF:
        m_buttonServo->setIcon(QIcon(QString(WINDOWICONPATH) + QString("servooff.png")));
        break;
    default:
        break;
    }
}

void MainWindow::updateTeachPlayButton()
{
    if(local_controlStateFlag == SWITCHON && local_runStateFlag != SYSRUN_RUN)
    {
        m_buttonTeachPlayMode->setEnable(true);
    }
    else{
        m_buttonTeachPlayMode->setEnable(false);
    }
}

void MainWindow::updateDebugButton()
{
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_PLAY
            && local_runStateFlag == SYSRUN_STOP)
    {
        m_buttonNormalDebugMode->setEnable(true);
    }
    else{
        m_buttonNormalDebugMode->setEnable(false);
    }
}

void MainWindow::updateStepButton()
{
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_PLAY && local_debugStateFlag == SWITCHON)
    {
        if(local_runStateFlag == SYSRUN_RUN){
            m_buttonProgramStepPause->setEnable(true);
        }
        if(local_runStateFlag == SYSRUN_STOP){
            m_buttonProgramStepMode->setEnable(true);

            if(m_stepMode){
                m_buttonProgramStep->setEnable(true);
                m_buttonProgramBack->setEnable(true);
            }else{
                m_buttonProgramStep->setEnable(false);
                m_buttonProgramBack->setEnable(false);
                m_buttonProgramStepPause->setEnable(false);
            }
        }
    }
    else{
        m_buttonProgramStepMode->setEnable(false);
        m_buttonProgramStep->setEnable(false);
        m_buttonProgramBack->setEnable(false);
        m_buttonProgramStepPause->setEnable(false);
    }
}

void MainWindow::updateMoveModeButton()
{
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_TEACH
            && local_runStateFlag == SYSRUN_STOP )
    {
        m_buttonMoveMode->setEnable(true);
        m_editStepDistance->setEnabled(true);
    }
    else{
        m_buttonMoveMode->setEnable(false);
        m_editStepDistance->setEnabled(false);
    }
}

void MainWindow::updateClearErrorButton()
{
    if(local_controlStateFlag == SWITCHON && local_runStateFlag == SYSRUN_STOP )
    {
        m_buttonClearError->setEnable(true);
    }
    else{
        m_buttonClearError->setEnable(false);
    }
}

void MainWindow::updateMainProgram()
{
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_PLAY)
    {
        if(local_runStateFlag == SYSRUN_STOP){
            m_buttonProgramRun->setEnable(true);
            m_buttonPauseContinue->setEnable(false);
            m_buttonStop->setEnable(false);
        }
        if(local_runStateFlag != SYSRUN_STOP){
            m_buttonProgramRun->setEnable(false);
            m_buttonPauseContinue->setEnable(true);
            m_buttonStop->setEnable(true);
        }  
    }
    else{
        m_buttonProgramRun->setEnable(false);
        m_buttonPauseContinue->setEnable(false);
        m_buttonStop->setEnable(false);
    }
}

void MainWindow::updateMainMotion()
{
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_TEACH)
    {
        m_buttonMoveSpace->setEnable(true);
        m_buttonServo->setEnable(true);
        for(int i=0; i<6; i++){
            m_buttonJointMinus[i]->setEnable(true);
            m_buttonJointPlus[i]->setEnable(true);
        }
    }
    else{
        m_buttonMoveSpace->setEnable(false);
        m_buttonServo->setEnable(false);
        for(int i=0; i<6; i++){
            m_buttonJointMinus[i]->setEnable(false);
            m_buttonJointPlus[i]->setEnable(false);
        }
    }
}

void MainWindow::updateVelSlider()
{
    if(local_controlStateFlag == SWITCHON && local_playStateFlag !=SYSPLAY_REMOTE
            && local_runStateFlag == SYSRUN_STOP)
    {
        m_buttonVelMinus->setEnable(true);
        m_sliderVel->setEnabled(true);
        m_buttonVelPlus->setEnable(true);
        m_editVel->setEnabled(true);
    }
    else{
        m_buttonVelMinus->setEnable(false);
        m_sliderVel->setEnabled(false);
        m_buttonVelPlus->setEnable(false);
        m_editVel->setEnabled(false);
    }
}

void MainWindow::sliderVelChanged(){
    int value = m_sliderVel->value();
    m_editVel->setText(QString::number(value, 10));
    RobotClient::initance()->programVelocity(value / 100.0);
    setGlobalVel(value/100.0);
}

void MainWindow::buttonVelMinusClicked(){
    int value = m_sliderVel->value();
    value--;
    m_sliderVel->setValue(value);
    sliderVelChanged();
}

void MainWindow::buttonVelPlusClicked(){
    int value = m_sliderVel->value();
    value++;
    m_sliderVel->setValue(value);
    sliderVelChanged();
}

void MainWindow::buttonshutdownSystemClicked(){
    qApp->quit();
}

void MainWindow::RobotStateChange()
{
    updateTeachPlayButton();
    updateMainProgram();
    updateMainMotion();

    updateVelSlider();
    updateDebugButton();
    updateStepButton();
    updateMoveModeButton();

    updateClearErrorButton();
}

bool MainWindow::is_str_utf8(const char* str)
{
    unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr = *str;
    bool bAllAscii = true;
    for (unsigned int i = 0; str[i] != '\0'; ++i){
        chr = *(str + i);
        //判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
        if (nBytes == 0 && (chr & 0x80) != 0){
            bAllAscii = false;
        }
        if (nBytes == 0) {
            //如果不是ASCII码,应该是多字节符,计算字节数
            if (chr >= 0x80) {
                if (chr >= 0xFC && chr <= 0xFD){
                    nBytes = 6;
                }
                else if (chr >= 0xF8){
                    nBytes = 5;
                }
                else if (chr >= 0xF0){
                    nBytes = 4;
                }
                else if (chr >= 0xE0){
                    nBytes = 3;
                }
                else if (chr >= 0xC0){
                    nBytes = 2;
                }
                else{
                    return false;
                }
                nBytes--;
            }
        }
        else{
            //多字节符的非首字节,应为 10xxxxxx
            if ((chr & 0xC0) != 0x80){
                return false;
            }
            //减到为零为止
            nBytes--;
        }
    }
    //违返UTF8编码规则
    if (nBytes != 0) {
        return false;
    }
    if (bAllAscii){ //如果全部都是ASCII, 也是UTF8
        return true;
    }
    return true;
}
