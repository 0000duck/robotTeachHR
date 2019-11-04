#include "robotTeachWidget.h"

using namespace rclib;

RobotTeachWidget::RobotTeachWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, "#808080");
    this->setPalette(pe);

    initUI();
    initState();
    connectSignalSlot();

    updateRobotMotionWidget();
}

RobotTeachWidget::~RobotTeachWidget(){

}

void RobotTeachWidget::initUI(){
    QPalette m_pe;
    m_pe.setColor(QPalette::WindowText, WINDOWTEXTCOLOR);
    m_pe.setColor(QPalette::ButtonText, BUTTONTEXTCOLOR);
//    pe.setColor(QPalette::Text, TEXTCOLOR);
    m_pe.setColor(QPalette::Button, BUTTONCOLOR);   // 按键
    m_pe.setColor(QPalette::Background, BACKGROUNDCOLOR);   // 背景
    m_pe.setColor(QPalette::Base, BASECOLOR); // 可填充文字的控件

    m_widgetMessageBox = new QWidget(this);
    m_widgetMessageBox->setWindowFlags(Qt::Dialog);
    m_widgetMessageBox->resize(300,80);
    m_widgetMessageBox->move(510,350);
    m_widgetMessageBox->setWindowTitle(tr("提示"));
    m_widgetMessageBox->setWindowModality(Qt::WindowModal);
    m_labelMessagebox = new QLabel();
    QWidget* m_widgetTmp1 = new QWidget();
    QWidget* m_widgetTmp2 = new QWidget();
    QHBoxLayout* m_layoutLabel = new QHBoxLayout();
    m_layoutLabel->addWidget(m_widgetTmp1,Qt::AlignLeft);
    m_layoutLabel->addWidget(m_labelMessagebox,Qt::AlignHCenter);
    m_layoutLabel->addWidget(m_widgetTmp2,Qt::AlignRight);
    QHBoxLayout* m_layoutMessageH = new QHBoxLayout();
    m_buttonDeafultVel = new PushButton();
    m_buttonDeafultVel->setText(tr("默认速度"));
    m_buttonCurrntVel = new PushButton();
    m_buttonCurrntVel->setText(tr("当前速度"));
    m_buttonCancel = new PushButton();
    m_buttonCancel->setText(tr("取消"));
    m_layoutMessageH->addWidget(m_buttonDeafultVel);
    m_layoutMessageH->addWidget(m_buttonCurrntVel);
    m_layoutMessageH->addWidget(m_buttonCancel);

    QVBoxLayout* m_layoutMessageV = new QVBoxLayout(m_widgetMessageBox);
    m_layoutMessageV->addLayout(m_layoutLabel);
    m_layoutMessageV->addLayout(m_layoutMessageH);

    m_widgetMessageBox->setPalette(m_pe);
    m_widgetMessageBox->hide();

    QFont ft;
    ft.setPointSize(12);

    m_widgetBackGround = new QWidget(this);
    m_widgetBackGround->move(10,10);
    m_widgetBackGround->resize(600,400);
    m_widgetBackGround->setStyleSheet("background-color: rgb(36,82,181)");

    QGroupBox* m_boxJoint=new QGroupBox();
    QGroupBox* m_boxWorld=new QGroupBox();
    QGroupBox* m_boxUser=new QGroupBox();

    QVBoxLayout* layoutCurrentJoint = new QVBoxLayout();
    m_labelCurrentJointType = new QLabel(tr("关节\n坐标系"));
    m_labelCurrentJointType->setFont(ft);
    layoutCurrentJoint->addWidget(m_labelCurrentJointType);
    for(int i=0; i<ROBOTDOF; i++){
        m_labelCurrentJoint[i] = new QLabel(":");
        layoutCurrentJoint->addWidget(m_labelCurrentJoint[i]);
        m_labelCurrentJoint[i]->setFont(ft);
    }
    m_boxJoint->setLayout(layoutCurrentJoint);

    QVBoxLayout* layoutCurrentTerminal = new QVBoxLayout();
    labelCurrentTerminal = new QLabel(tr("基\n坐标系"));
    labelCurrentTerminal->setFont(ft);
    layoutCurrentTerminal->addWidget(labelCurrentTerminal);
    for(int i=0; i<ROBOTDOF; i++){
        m_labelCurrentTerminal[i] = new QLabel(":");
        layoutCurrentTerminal->addWidget(m_labelCurrentTerminal[i]);
        m_labelCurrentTerminal[i]->setFont(ft);
    }
    m_boxWorld->setLayout(layoutCurrentTerminal);

    QVBoxLayout* layoutCurrentUserTerminal = new QVBoxLayout();
    labelCurrentUserTerminal = new QLabel(tr("工件\n坐标系"));
    labelCurrentUserTerminal->setFont(ft);
    layoutCurrentUserTerminal->addWidget(labelCurrentUserTerminal);
    for(int i=0; i<ROBOTDOF; i++){
        m_labelCurrentUserTerminal[i] = new QLabel(":");
        layoutCurrentUserTerminal->addWidget(m_labelCurrentUserTerminal[i]);
        m_labelCurrentUserTerminal[i]->setFont(ft);
    }
    m_boxUser->setLayout(layoutCurrentUserTerminal);

    QHBoxLayout* layoutCurrentState = new QHBoxLayout();
    layoutCurrentState->addWidget(m_boxJoint);
    layoutCurrentState->addWidget(m_boxWorld);
    layoutCurrentState->addWidget(m_boxUser);

    m_buttonReturnZero = new PushButton(tr("回归初始位置"));
    m_buttonReturnZero->setMinimumHeight(0.04*WINDOWWIDTH);
    m_buttonReturnZero->setParent(this);
    m_buttonCalibrateZero = new PushButton(tr("零位校准"));
    m_buttonCalibrateZero->setMinimumHeight(0.04*WINDOWWIDTH);
    m_buttonCalibrateZero->setParent(this);


    m_seriesCheck=new QWidget(this);
    m_seriesCheck->setGeometry(510,430,100,210);

    m_seriesCheck->setAutoFillBackground(true);
    QPalette palette;
    QPalette text_palette;
    text_palette.setColor(QPalette::WindowText,Qt::black);
    palette.setColor(QPalette::Background, "#808080");
    m_seriesCheck->setPalette(palette);

    palette.setColor(QPalette::Background, QColor(255,0,0));
    m_x_text=new QLabel(tr("轴1"));
    m_x_text->setFont(ft);
    m_x_text->setPalette(text_palette);
    m_x_color=new QLabel();
    m_x_color->setAutoFillBackground(true);
    m_x_color->setPalette(palette);

    palette.setColor(QPalette::Background, QColor(255,255,0));
    m_y_text=new QLabel(tr("轴2"));
    m_y_text->setFont(ft);
    m_y_text->setPalette(text_palette);
    m_y_color=new QLabel();
    m_y_color->setAutoFillBackground(true);
    m_y_color->setPalette(palette);

    palette.setColor(QPalette::Background, QColor(139,0,139));
    m_z_text=new QLabel(tr("轴3"));
    m_z_text->setFont(ft);
    m_z_text->setPalette(text_palette);
    m_z_color=new QLabel();
    m_z_color->setAutoFillBackground(true);
    m_z_color->setPalette(palette);

    palette.setColor(QPalette::Background, QColor(0,255,255));
    m_a_text=new QLabel(tr("轴4"));
    m_a_text->setFont(ft);
    m_a_text->setPalette(text_palette);
    m_a_color=new QLabel();
    m_a_color->setAutoFillBackground(true);
    m_a_color->setPalette(palette);

    palette.setColor(QPalette::Background, QColor(0,0,255));
    m_b_text=new QLabel(tr("轴5"));
    m_b_text->setFont(ft);
    m_b_text->setPalette(text_palette);
    m_b_color=new QLabel();
    m_b_color->setAutoFillBackground(true);
    m_b_color->setPalette(palette);

    palette.setColor(QPalette::Background, QColor(0,100,0));
    m_c_text=new QLabel(tr("轴6"));
    m_c_text->setFont(ft);
    m_c_text->setPalette(text_palette);
    m_c_color=new QLabel();
    m_c_color->setAutoFillBackground(true);
    m_c_color->setPalette(palette);

    m_x_check=new QCheckBox();
    m_x_check->setChecked(true);
    m_y_check=new QCheckBox();
    m_y_check->setChecked(true);
    m_z_check=new QCheckBox();
    m_z_check->setChecked(true);
    m_a_check=new QCheckBox();
    m_a_check->setChecked(true);
    m_b_check=new QCheckBox();
    m_b_check->setChecked(true);
    m_c_check=new QCheckBox();
    m_c_check->setChecked(true);

    QHBoxLayout* x_layout=new QHBoxLayout();
    x_layout->addWidget(m_x_text);
    x_layout->addWidget(m_x_color);
    x_layout->addWidget(m_x_check);

    QHBoxLayout* y_layout=new QHBoxLayout();
    y_layout->addWidget(m_y_text);
    y_layout->addWidget(m_y_color);
    y_layout->addWidget(m_y_check);

    QHBoxLayout* z_layout=new QHBoxLayout();
    z_layout->addWidget(m_z_text);
    z_layout->addWidget(m_z_color);
    z_layout->addWidget(m_z_check);

    QHBoxLayout* a_layout=new QHBoxLayout();
    a_layout->addWidget(m_a_text);
    a_layout->addWidget(m_a_color);
    a_layout->addWidget(m_a_check);

    QHBoxLayout* b_layout=new QHBoxLayout();
    b_layout->addWidget(m_b_text);
    b_layout->addWidget(m_b_color);
    b_layout->addWidget(m_b_check);

    QHBoxLayout* c_layout=new QHBoxLayout();
    c_layout->addWidget(m_c_text);
    c_layout->addWidget(m_c_color);
    c_layout->addWidget(m_c_check);

    QVBoxLayout* m_layoutCheck=new QVBoxLayout(m_seriesCheck);
    m_layoutCheck->addLayout(x_layout);
    m_layoutCheck->addLayout(y_layout);
    m_layoutCheck->addLayout(z_layout);
    m_layoutCheck->addLayout(a_layout);
    m_layoutCheck->addLayout(b_layout);
    m_layoutCheck->addLayout(c_layout);

    QGroupBox* m_boxToolFrame=new QGroupBox();
    QGroupBox* m_boxWorkFrame=new QGroupBox();

    QPalette pe;
    pe.setColor(QPalette::WindowText,"#464547");

    m_currentToolFrameName=new QLabel(tr("工具\n坐标 :"));
    m_currentToolFrameName->setPalette(pe);
    m_ToolFrameName=new QLabel("Default");

    m_currentWorkFrameName=new QLabel(tr("工件\n坐标 :"));
    m_currentWorkFrameName->setPalette(pe);
    m_WorkFrameName=new QLabel("Default");

    QHBoxLayout* m_layoutToolFrame=new QHBoxLayout();
    m_layoutToolFrame->addWidget(m_currentToolFrameName);
    m_layoutToolFrame->addWidget(m_ToolFrameName);
    m_boxToolFrame->setLayout(m_layoutToolFrame);

    QHBoxLayout* m_layoutWorkFrame=new QHBoxLayout();
    m_layoutWorkFrame->addWidget(m_currentWorkFrameName);
    m_layoutWorkFrame->addWidget(m_WorkFrameName);
    m_boxWorkFrame->setLayout(m_layoutWorkFrame);

    QHBoxLayout* m_layoutFrame=new QHBoxLayout();
    m_layoutFrame->addWidget(m_boxToolFrame);
    m_layoutFrame->addWidget(m_boxWorkFrame);

    QHBoxLayout* m_layoutButton=new QHBoxLayout();
    m_layoutButton->addWidget(m_buttonCalibrateZero);
    m_layoutButton->addWidget(m_buttonReturnZero);

    m_widgetTeach = new QWidget(this);
    m_widgetTeach->resize(350,650);
    m_widgetTeach->move(615,0);
    QVBoxLayout* layoutTeach = new QVBoxLayout(m_widgetTeach);
    layoutTeach->addLayout(layoutCurrentState,8);
    layoutTeach->addLayout(m_layoutFrame,1);
    layoutTeach->addLayout(m_layoutButton,1);

    m_chart=new QCustomPlot();
    m_chart->setBackground(QBrush("#808080"));

    m_chart->xAxis->setVisible(false);

    m_x_Graph = m_chart->addGraph(m_chart->xAxis, m_chart->yAxis);
    m_x_Graph->setPen(QPen(QColor(255,0,0)));
    m_y_Graph = m_chart->addGraph(m_chart->xAxis, m_chart->yAxis);
    m_y_Graph->setPen(QPen(QColor(255,255,0)));
    m_z_Graph = m_chart->addGraph(m_chart->xAxis, m_chart->yAxis);
    m_z_Graph->setPen(QPen(QColor(139,0,139)));
    m_a_Graph = m_chart->addGraph(m_chart->xAxis, m_chart->yAxis);
    m_a_Graph->setPen(QPen(QColor(0,255,255)));
    m_b_Graph = m_chart->addGraph(m_chart->xAxis, m_chart->yAxis);
    m_b_Graph->setPen(QPen(QColor(0,0,255)));
    m_c_Graph = m_chart->addGraph(m_chart->xAxis, m_chart->yAxis);
    m_c_Graph->setPen(QPen(QColor(0,100,0)));

    m_robotCircular=new QWidget(this);
    m_robotCircular->setGeometry(10,410,500,250);

    QHBoxLayout* m_chartLayout = new QHBoxLayout(m_robotCircular);
    m_chartLayout->addWidget(m_chart);

    m_robotModel = new RobotModel(this);
    m_robotModel->move(80,65);

    m_seriesCheck->raise();
}

void RobotTeachWidget::initState(){

}

void RobotTeachWidget::connectSignalSlot(){
    QObject::connect(m_buttonReturnZero, SIGNAL(clicked()), this, SLOT(buttonReturnZeroClicked()));
    QObject::connect(m_buttonCalibrateZero, SIGNAL(clicked()), this, SLOT(buttonCalibrateZeroClicked()));

    QObject::connect(m_buttonDeafultVel, SIGNAL(clicked()), this, SLOT(buttonDeafultClicked()));
    QObject::connect(m_buttonCurrntVel, SIGNAL(clicked()), this, SLOT(buttonCurrentClicked()));
    QObject::connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(butonCancelClicked()));
}

void RobotTeachWidget::updateSystemState(){
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_TEACH){
        m_buttonReturnZero->setEnable(true);
        m_buttonCalibrateZero->setEnable(true);
    }
    else{
        m_buttonReturnZero->setEnable(false);
        m_buttonCalibrateZero->setEnable(false);
    }
}

void RobotTeachWidget::updateRobotMotionWidget(){
    updateSystemState();

    RobotMotion rm = RobotClient::initance()->getRobotMotion();
    QString temp;

    QString termianlStr[] = {"X: ", "Y: ", "Z: ", "A: ", "B: ", "C:"};
    for(int i=0; i<6; i++){
        temp = QString::number(rm.getCurrentJointPosition()[static_cast<JOINTINDEX>(i)], 10, 2);
        if(temp == QString("-0.00"))  //假如值为“-0.00”则变为“0.00”
            temp = QString("0.00");
        QString str = QString("J")+ QString::number(i+1, 10) + QString(": ") + temp;
        m_labelCurrentJoint[i]->setText(str);
    }

    for(int i=0; i<6; i++){
        temp = QString::number(rm.getCurrentWorkTerminal()[static_cast<TERMINALINDEX>(i)], 10, 2);
        if(temp == QString("-0.00"))
            temp = QString("0.00");
        QString str = termianlStr[i] + temp;
        m_labelCurrentUserTerminal[i]->setText(str);
    }

    for(int i=0; i<6; i++){
        temp = QString::number(rm.getCurrentTerminal()[static_cast<TERMINALINDEX>(i)], 10, 2);
        if(temp == QString("-0.00"))
            temp = QString("0.00");
        QString str = termianlStr[i] + temp;
        m_labelCurrentTerminal[i]->setText(str);
    }

    std::string tmp_tool,tmp_user;
    RobotFrame tmp_frame=RobotClient::initance()->getRobotFrame();
    tmp_tool=tmp_frame.getCurrentToolFrame();
    tmp_user=tmp_frame.getCurrentWorkFrame();
    m_ToolFrameName->setText(tmp_tool.c_str());
    m_WorkFrameName->setText(tmp_user.c_str());


    if(m_x_check->isChecked())
    {
        m_x_Graph->setPen(QPen(QColor(255,0,0)));
    }
    else
    {
        m_x_Graph->setPen(QPen(QColor(0,0,0,0)));
    }

    if(m_y_check->isChecked())
    {
        m_y_Graph->setPen(QPen(QColor(255,255,0)));
    }
    else
    {
        m_y_Graph->setPen(QPen(QColor(0,0,0,0)));
    }

    if(m_z_check->isChecked())
    {
        m_z_Graph->setPen(QPen(QColor(139,0,139)));
    }
    else
    {
        m_z_Graph->setPen(QPen(QColor(0,0,0,0)));
    }

    if(m_a_check->isChecked())
    {
        m_a_Graph->setPen(QPen(QColor(0,255,255)));
    }
    else
    {
        m_a_Graph->setPen(QPen(QColor(0,0,0,0)));
    }

    if(m_b_check->isChecked())
    {
        m_b_Graph->setPen(QPen(QColor(0,0,255)));
    }
    else
    {
        m_b_Graph->setPen(QPen(QColor(0,0,0,0)));
    }

    if(m_c_check->isChecked())
    {
        m_c_Graph->setPen(QPen(QColor(0,100,0)));
    }
    else
    {
        m_c_Graph->setPen(QPen(QColor(0,0,0,0)));
    }

    Joints tmp;
    tmp=rm.getCurrentJointPosition();

    m_x_Graph->addData(m_x_Graph->dataCount(), tmp[static_cast<JOINTINDEX>(0)]);
    m_y_Graph->addData(m_y_Graph->dataCount(), tmp[static_cast<JOINTINDEX>(1)]);
    m_z_Graph->addData(m_z_Graph->dataCount(), tmp[static_cast<JOINTINDEX>(2)]);
    m_a_Graph->addData(m_a_Graph->dataCount(), tmp[static_cast<JOINTINDEX>(3)]);
    m_b_Graph->addData(m_b_Graph->dataCount(), tmp[static_cast<JOINTINDEX>(4)]);
    m_c_Graph->addData(m_c_Graph->dataCount(), tmp[static_cast<JOINTINDEX>(5)]);


    m_chart->xAxis->rescale();
    m_chart->xAxis->setRange(m_chart->xAxis->range().upper, 100, Qt::AlignRight);
    m_chart->yAxis->setRange(-200,200);

    m_chart->replot();
}

void RobotTeachWidget::passState(rclib::SYSPLAYSTATE playStateFlag,SWITCHSTATE controlStateFlag)
{
    local_playStateFlag = playStateFlag;
    local_controlStateFlag = controlStateFlag;
}

void RobotTeachWidget::buttonReturnZeroClicked(){
    JUD = true;
    m_labelMessagebox->setText("");
    m_labelMessagebox->setText(tr("确认回归初始位置？"));
    m_widgetMessageBox->show();
}

void RobotTeachWidget::buttonCalibrateZeroClicked(){
    JUD = false;
    m_labelMessagebox->setText("");
    m_labelMessagebox->setText(tr("确认开始回零?"));
    m_widgetMessageBox->show();
}

void RobotTeachWidget::buttonDeafultClicked()
{
    if(JUD){
        RobotClient::initance()->returnZero(0.05);
    }
    else{
        RobotClient::initance()->moveToZero(0.05);
    }

    m_widgetMessageBox->hide();
}

void RobotTeachWidget::buttonCurrentClicked()
{
    if(JUD){
        RobotClient::initance()->returnZero(getGlobalVel());
    }
    else{
        float tmp = getGlobalVel();
        if(tmp > 0.3)
            tmp = 0.3;
        RobotClient::initance()->moveToZero(tmp);
    }

    m_widgetMessageBox->hide();
}

void RobotTeachWidget::butonCancelClicked()
{
    m_widgetMessageBox->hide();
}

void RobotTeachWidget::setMoveSpace(int state){
    m_moveSpace = state;
}
