#include "paintWidget.h"

using namespace std;
using namespace rclib;

static std::vector<std::vector<QPoint>> m_pointList;

PaintWidget::PaintWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, WINDOWTEXTCOLOR);
    pe.setColor(QPalette::Button, BACKGROUNDCOLOR);
    this->setPalette(pe);

    initUI();
    initState();
    connectSignalSlot();
}

PaintWidget::~PaintWidget(){

}

void PaintWidget::initUI(){
    m_buttonClearPaint = new PushButton(tr("清空画布"));
    m_buttonMovePaint = new PushButton(tr("开始绘制"));
    m_editRatio = new QLineEdit();
    m_buttonClearPaint->setFixedSize(100, 50);
    m_buttonMovePaint->setFixedSize(100, 50);
    m_editRatio ->setFixedSize(50, 50);
    QLabel* labelRatio = new QLabel();
    QString str = tr("当前窗口尺寸为");
//    str.append(QString::number(this->height(), 10));
    str.append("600 X 1000");
//    str.append(QString::number(this->width(), 10));
    str.append(tr("，缩放比例为："));
    labelRatio->setText(str);

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    labelRatio->setPalette(pe);

    QString tmp;
    read_cache_ratio(tmp);
    m_editRatio->setText(tmp);

    QHBoxLayout* layoutPaint = new QHBoxLayout(this);
    layoutPaint->addWidget(m_buttonClearPaint, 0, Qt::AlignBottom | Qt::AlignLeft);
    layoutPaint->addWidget(labelRatio, 0, Qt::AlignBottom);
    layoutPaint->addWidget(m_editRatio, 0, Qt::AlignBottom);
    layoutPaint->addWidget(m_buttonMovePaint, 0, Qt::AlignBottom | Qt::AlignRight);
}

void PaintWidget::initState(){

}

void PaintWidget::connectSignalSlot(){
    QObject::connect(m_buttonClearPaint, SIGNAL(clicked()), this, SLOT(buttonClearPaint()));
    QObject::connect(m_buttonMovePaint, SIGNAL(clicked()), this, SLOT(buttonMovePaint()));

    QObject::connect(m_editRatio, SIGNAL(editingFinished()), this, SLOT(ratioConfigChanged()));
}

void PaintWidget::updatePaintWidget()
{
    updateSystemState();
}

void PaintWidget::updateSystemState()
{
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_TEACH) {
        this->setEnabled(true);
    }
    else{
        this->setEnabled(false);
    }
}

void PaintWidget::passState(rclib::SYSPLAYSTATE playStateFlag,SWITCHSTATE controlStateFlag)
{
    local_playStateFlag = playStateFlag;
    local_controlStateFlag = controlStateFlag;
}

static int s_count = 0;
static int s_period = 1;
double s_ratio = 0.5;
vector<QPoint> s_currentTerminalList;
void PaintWidget::mouseMoveEvent(QMouseEvent *event){
    s_count++;
    s_count = s_count % s_period;
    if(s_count != 0){
        return;
    }

    s_currentTerminalList.push_back(QPoint(event->x(), event->y()));
    cout << event->x() << ", " << event->y() << endl;

    repaint();
}

void PaintWidget::mousePressEvent(QMouseEvent *event){
    s_currentTerminalList.clear();
    s_count = -1;
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event){
    if(!s_currentTerminalList.empty()){
        m_pointList.push_back(s_currentTerminalList);
        s_currentTerminalList.clear();
    }

    repaint();
}

void PaintWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);

    for(int i=0; i<m_pointList.size(); i++){
        QPoint point1 = m_pointList[i][0];
        for(int j=0; j<m_pointList[i].size(); j++){
            QPoint point2 = m_pointList[i][j];
            painter.drawLine(point1, point2);
            point1 = point2;
        }
    }

    if(!s_currentTerminalList.empty()){
        QPoint point1 = s_currentTerminalList[0];
        for(int j=0; j<s_currentTerminalList.size(); j++){
            QPoint point2 = s_currentTerminalList[j];
            painter.drawLine(point1, point2);
            point1 = point2;
        }
    }
}

void PaintWidget::buttonClearPaint(){
    m_pointList.clear();
    s_currentTerminalList.clear();
    repaint();
}

void PaintWidget::ratioConfigChanged()
{
    QString ratio = m_editRatio->text();
    write_cache_ratio(ratio);
}

double pointDis(const QPoint& p1, const QPoint& p2){
    double dis;
    dis = sqrt(pow(p1.x()-p2.x(), 2) + pow(p1.y()-p2.y(), 2));
    return dis;
}

bool terminalEqual(const robsoft::Terminal& t1, const robsoft::Terminal& t2){
    double sum = 0;
    for(int i=0; i<3; i++){
        sum += pow(t1[TERMINALINDEX(i)]-t2[TERMINALINDEX(i)], 2);
    }
    sum = sqrt(sum);
    if(sum < 5){
        return true;
    }
    else{
        return false;
    }
}

static double s_high = 10;
static double s_vel = 100;
static double s_vel1 = 300;
static double s_acc = 1;
static double s_jerk = 1;
static double s_turn = -1;
void PaintWidget::buttonMovePaint(){
    s_ratio = m_editRatio->text().toDouble();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("提示"),
                                    tr("请再次确认坐标系选择是否正确！\n坐标系设置错误易造成设备损坏！\n工具坐标系请在列表中选择\"Bi\"，\n工件坐标系请在列表中选择\"Ban\"。\n确定开始绘制？"),
                                    QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
        return;

    if(!RobotClient::initance()->getRobotState().getServoState()){
        return;
    }

    pthread_t id;
    int ret;

    if ((ret = pthread_create(&id, NULL, moveThread, NULL)) != 0){
        printf("create tcp recv thread failed\n");
        return;
    }
}

void* PaintWidget::moveThread(void *){
    robsoft::Terminal lastter=RobotClient::initance()->getRobotMotion().getCurrentWorkTerminal();
    robsoft::Terminal ter=lastter;

    // 开始绘制
    int ret = RobotClient::initance()->moveStartCon(5);
    if(ret){
        return NULL;
    }

    double vel;
    for(int i=0; i<m_pointList.size(); i++){      
        vel = s_vel1;
        // 先提笔，运动到当前位置指定高度
        ter[TERMINAL_Z]=s_high;
        ret = RobotClient::initance()->moveLineCon(ter, vel*getGlobalVel(), s_acc, s_jerk, s_turn, COORDINATE_WORK);
        if(ret){
            return NULL;
        }
        lastter = ter;
        // 运动到当前轨迹段第一个点的位置
        ter[TERMINAL_X]= m_pointList[i][0].y()*1.0*s_ratio;
        ter[TERMINAL_Y]= m_pointList[i][0].x()*1.0*s_ratio;
        ter[TERMINAL_Z]= s_high;
        ret = RobotClient::initance()->moveLineCon(ter, vel*getGlobalVel(), s_acc, s_jerk, s_turn, COORDINATE_WORK);
        if(ret){
            return NULL;
        }
        lastter = ter;
        // 落笔
        ter[TERMINAL_Z]= 0;
        ret = RobotClient::initance()->moveLineCon(ter, vel*getGlobalVel(), s_acc, s_jerk, s_turn, COORDINATE_WORK);
        if(ret){
            return NULL;
        }
        lastter = ter;

        vel = s_vel;
        // 开始走轨迹
        for(int j=1; j<m_pointList[i].size(); j++){
            ter[TERMINAL_X]= m_pointList[i][j].y()*1.0*s_ratio;
            ter[TERMINAL_Y]= m_pointList[i][j].x()*1.0*s_ratio;
            ter[TERMINAL_Z]= 0;
            if(!terminalEqual(ter, lastter) || j==m_pointList[i].size()-1){  // 排除短距离的点，最后一个点必须走
                ret = RobotClient::initance()->moveLineCon(ter, vel*getGlobalVel(), s_acc, s_jerk, s_turn, COORDINATE_WORK);
                if(ret){
                    return NULL;
                }
                lastter=ter;
            }
        }
    }

    vel = s_vel1;
    // 轨迹运行完成，提笔
    ter[TERMINAL_Z]= s_high;
    ret = RobotClient::initance()->moveLineCon(ter, vel*getGlobalVel(), s_acc, s_jerk, s_turn, COORDINATE_WORK);
    if(ret){
        return NULL;
    }
    // 走到准备位置
    ter[TERMINAL_X]= 0;
    ter[TERMINAL_Y]= 0;
    ter[TERMINAL_Z]= 50;
    vel = s_vel1;
    ret = RobotClient::initance()->moveLineCon(ter, vel*getGlobalVel(), s_acc, s_jerk, s_turn, COORDINATE_WORK);
    if(ret){
        return NULL;
    }

    // 结束绘制
    RobotClient::initance()->moveEndCon();
    ret = RobotClient::initance()->waitCommandEnd();
    if(ret){
        return NULL;
    }
}

