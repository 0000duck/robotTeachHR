#include "robotParameterSettingWidget.h"

#include <string>
#include <QDebug>

using namespace std;
using namespace rclib;

RobotParameterSettingWidget::RobotParameterSettingWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, "#808080");
    this->setPalette(pe);

    initUI();
    initState();
    connectSignalSlot();
}

RobotParameterSettingWidget::~RobotParameterSettingWidget(){
}

void RobotParameterSettingWidget::initUI(){
    m_labelRobotType = new QLabel(tr("机器人构型:"));
    m_comboRobotType =new ComboBox();
    m_comboRobotType->setMinimumSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_labelRobotPeriod = new QLabel(tr("指令周期/毫秒:"));
    m_editRobotPeriod = new QLineEdit();
    m_editRobotPeriod->setMinimumSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_buttonSaveRobotParameter = new PushButton(tr("保存参数"));
    m_buttonSaveRobotParameter->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutRobotType = new QHBoxLayout();
    layoutRobotType->addWidget(m_labelRobotType);
    layoutRobotType->addWidget(m_comboRobotType);

    QHBoxLayout* layoutRobotPeriod = new QHBoxLayout();
    layoutRobotPeriod->addWidget(m_labelRobotPeriod);
    layoutRobotPeriod->addWidget(m_editRobotPeriod);

    QHBoxLayout* layoutNormalParameter = new QHBoxLayout();
    layoutNormalParameter->addLayout(layoutRobotType);
    layoutNormalParameter->addLayout(layoutRobotPeriod);
    layoutNormalParameter->setStretchFactor(layoutRobotType, 1);
    layoutNormalParameter->setStretchFactor(layoutRobotPeriod, 1);

    m_groupNormalParameter = new QGroupBox();
    m_groupNormalParameter->setMaximumHeight(100);
    m_groupNormalParameter->setLayout(layoutNormalParameter);

    QHBoxLayout* layoutCommon = new QHBoxLayout();
    layoutCommon->addWidget(m_groupNormalParameter, 0, Qt::AlignLeft);
    layoutCommon->addWidget(m_buttonSaveRobotParameter, 0, Qt::AlignRight | Qt::AlignCenter);

    m_tableRatioParameter = new QTableWidget();
    m_tableMotionParameter = new QTableWidget();
    m_tableJointParameter = new QTableWidget();
    m_tableModelParameter = new QTableWidget();

    m_tabRobotParameter = new QTabWidget();
    m_tabRobotParameter->setTabPosition(QTabWidget::South);
    m_tabRobotParameter->addTab(m_tableRatioParameter, tr("比例参数"));
    m_tabRobotParameter->addTab(m_tableMotionParameter, tr("运动参数"));
    m_tabRobotParameter->addTab(m_tableJointParameter, tr("驱动参数"));
    m_tabRobotParameter->addTab(m_tableModelParameter, tr("模型参数"));

    QVBoxLayout* layoutRobotParameter = new QVBoxLayout(this);
    layoutRobotParameter->addLayout(layoutCommon);
    layoutRobotParameter->addWidget(m_tabRobotParameter);
    layoutRobotParameter->setStretchFactor(layoutCommon, 1);
    layoutRobotParameter->setStretchFactor(m_tabRobotParameter, 6);

    m_comboRobotType->insertItem(0,tr("六轴常规型"));
    m_comboRobotType->insertItem(1,tr("六轴协作型"));
    m_comboRobotType->insertItem(2,tr("四轴常规型"));
    m_comboRobotType->insertItem(3,tr("一轴SCARA型"));
    m_comboRobotType->insertItem(4,tr("四轴SCARA型"));
    m_comboRobotType->insertItem(5,tr("单姿态DELTA型"));
    m_comboRobotType->insertItem(6,tr("三姿态DELTA型"));
}

void RobotParameterSettingWidget::initState(){
    m_robot = RobotClient::initance()->getRobotParameter();

    initRatioParameterTable();
    initMotionParameterTable();
    initJointParameterTable();
    initModelParameterTable();

    fillRatioParameterTableContent();
    fillMotionParameterTableContent();
    fillJointParameterTableContent();

    m_robotType =m_robot.getRobotType();
    m_comboRobotType->setCurrentIndex(m_robotType);

    m_editRobotPeriod->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));

    m_editRobotPeriod->setText(QString("%1").arg(m_robot.getSamplePeriod() * 1000));

    fillRatioParameterTableValue();
    fillMotionParameterTableValue();
    fillJointParameterTableValue();
    fillModelParameterTableValue();

    updateSystemState();
}

void RobotParameterSettingWidget::connectSignalSlot(){
    QObject::connect(m_buttonSaveRobotParameter, SIGNAL(clicked()), this, SLOT(buttonSaveRobotParameterClicked()));
    QObject::connect(m_comboRobotType, SIGNAL(activated(int)), this, SLOT(comboRobotTypeChanged(int)));
}

void RobotParameterSettingWidget::initRatioParameterTable(){
    m_tableRatioParameter->setColumnCount(2);
    m_tableRatioParameter->setRowCount(6);
    m_tableRatioParameter->horizontalHeader()->setSectionsClickable(false);
    QStringList header;
    header<<tr("项目")<<tr("值");
    m_tableRatioParameter->setHorizontalHeaderLabels(header);
    QFont font = m_tableRatioParameter->horizontalHeader()->font();
    font.setBold(true);
    m_tableRatioParameter->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableRatioParameter->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableRatioParameter->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //自适应列宽
//    m_tableRatioParameter->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应行高
    m_tableRatioParameter->verticalHeader()->setStyleSheet("QHeaderView::section{background:black;}");
    m_tableRatioParameter->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableRatioParameter->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableRatioParameter->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableRatioParameter->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableRatioParameter->setSelectionMode(QAbstractItemView::SingleSelection);
//    m_tableRatioParameter->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableRatioParameter->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
    m_tableRatioParameter->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");

    m_tableRatioParameter->setStyleSheet("QTableCornerButton::section{background-color:black;}");
}

void RobotParameterSettingWidget::initMotionParameterTable(){
    m_tableMotionParameter->setColumnCount(2);
    m_tableMotionParameter->setRowCount(12 * 5);
    m_tableMotionParameter->horizontalHeader()->setSectionsClickable(false);
    QStringList header;
    header<<tr("项目")<<tr("值");
    m_tableMotionParameter->setHorizontalHeaderLabels(header);
    QFont font = m_tableMotionParameter->horizontalHeader()->font();
    font.setBold(true);
    m_tableMotionParameter->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableMotionParameter->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableMotionParameter->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    m_tableMotionParameter->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableMotionParameter->verticalHeader()->setStyleSheet("QHeaderView::section{background:black;}");
    m_tableMotionParameter->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableMotionParameter->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableMotionParameter->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableMotionParameter->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableMotionParameter->setSelectionMode(QAbstractItemView::SingleSelection);
//    m_tableMotionParameter->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableMotionParameter->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
    m_tableMotionParameter->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");

    m_tableMotionParameter->setStyleSheet("QTableCornerButton::section{background-color:black;}");
}

void RobotParameterSettingWidget::initJointParameterTable(){
    m_tableJointParameter->setColumnCount(2);
    m_tableJointParameter->setRowCount(6 * 3);
    m_tableJointParameter->horizontalHeader()->setSectionsClickable(false);
    QStringList header;
    header<<tr("项目")<<tr("值");
    m_tableJointParameter->setHorizontalHeaderLabels(header);
    QFont font = m_tableJointParameter->horizontalHeader()->font();
    font.setBold(true);
    m_tableJointParameter->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableJointParameter->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableJointParameter->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    m_tableJointParameter->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableJointParameter->verticalHeader()->setStyleSheet("QHeaderView::section{background:black;}");
    m_tableJointParameter->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableJointParameter->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableJointParameter->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableJointParameter->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableJointParameter->setSelectionMode(QAbstractItemView::SingleSelection);
//    m_tableJointParameter->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableJointParameter->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
    m_tableJointParameter->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");

    m_tableJointParameter->setStyleSheet("QTableCornerButton::section{background-color:black;}");
}

void RobotParameterSettingWidget::initModelParameterTable(){
    m_tableModelParameter->setColumnCount(4);
    m_tableModelParameter->setRowCount(6);
    m_tableModelParameter->horizontalHeader()->setSectionsClickable(false);
    QStringList header;
    header<<tr("alpha")<<tr("a")<<tr("d")<<tr("theta");
    m_tableModelParameter->setHorizontalHeaderLabels(header);
    QFont font = m_tableModelParameter->horizontalHeader()->font();
    font.setBold(true);
    m_tableModelParameter->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableModelParameter->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableModelParameter->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    m_tableModelParameter->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableModelParameter->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableModelParameter->verticalHeader()->setStyleSheet("QHeaderView::section{background:black;}");
    m_tableModelParameter->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableModelParameter->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableModelParameter->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableModelParameter->setSelectionMode(QAbstractItemView::SingleSelection);
//    m_tableModelParameter->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableModelParameter->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
    m_tableModelParameter->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");

    m_tableModelParameter->setStyleSheet("QTableCornerButton::section{background-color:black;}");
}

void RobotParameterSettingWidget::fillRatioParameterTableContent(){
    QString text[6] = {tr("关节最大速度比例"), tr("关节最大加速度比例"), tr("关节最大冲击比例"), tr("末端最大速度比例"), tr("末端最大加速度比例"), tr("末端最大冲击比例")};
    for(int i=0; i<6; i++){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(text[i]);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        m_tableRatioParameter->setItem(i, 0, item);
    }
}

void RobotParameterSettingWidget::fillMotionParameterTableContent(){
    QString text1[5] = {tr("负向限制(度)"), tr("正向限制(度)"), tr("最大速度(度/秒)"), tr("最大加速度(度/秒2)"),tr("最大冲击(度/秒3)")};
    QString text2[6] = {tr("X轴"), tr("Y轴"), tr("Z轴"),tr("角A"),tr("角B"),tr("角C")};
    QString text3[2] = {tr("负向限制(毫米)"), tr("正向限制(毫米)")};
    QString text4[6] = {tr("沿X轴"), tr("沿Y轴"), tr("沿Z轴"),tr("沿角A"),tr("沿角B"),tr("沿角C")};
    QString text5[3] = {tr("最大速度(毫米/秒)"), tr("最大加速度(毫米/秒2)"), tr("最大冲击(毫米/秒3)")};

    int count = 0;
    for(int j=0; j<5; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            QString text = tr("轴");
            text.append(QString::number(i + 1, 10));
            text.append(text1[j]);
            item->setText(text);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableMotionParameter->setItem(count+i, 0, item);
        }
        count += 6;
    }

    for(int j=0; j<2; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            QString text = text2[i];
            text.append(text3[j]);
            item->setText(text);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableMotionParameter->setItem(count+i, 0, item);
        }
        count += 6;
    }

    for(int j=0; j<3; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            QString text = text4[i];
            text.append(text5[j]);
            item->setText(text);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableMotionParameter->setItem(count+i, 0, item);
        }
        count += 6;
    }
}

void RobotParameterSettingWidget::fillJointParameterTableContent(){
    QString text1[3] = {tr("编码器分辨率"),tr("额定转矩"), tr("减速器减速比")};
    int count = 0;
    for(int j=0; j<3; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            QString text = "轴";
            text.append(QString::number(i + 1, 10));
            text.append(text1[j]);
            item->setText(text);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableJointParameter->setItem(count+i, 0, item);
        }
        count += 6;
    }
}

void RobotParameterSettingWidget::updateSystemState(){
    m_tableRatioParameter->setEditTriggers(QAbstractItemView::DoubleClicked);
    m_tableMotionParameter->setEditTriggers(QAbstractItemView::DoubleClicked);
    m_tableJointParameter->setEditTriggers(QAbstractItemView::DoubleClicked);
    m_tableModelParameter->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void RobotParameterSettingWidget::updateRobotParameterWidget(){
    m_robot = RobotClient::initance()->getRobotParameter();

    m_robotType = m_robot.getRobotType();
    m_comboRobotType->setCurrentIndex(m_robotType);
    m_editRobotPeriod->setText(QString("%1").arg(m_robot.getSamplePeriod() * 1000));

    fillRatioParameterTableValue();
    fillMotionParameterTableValue();
    fillJointParameterTableValue();
    fillModelParameterTableValue();
}

void RobotParameterSettingWidget::fillRatioParameterTableValue(){
    int count = 0;
    double temp= 0;
    for(int i=0; i<1; i++){
        m_robot.getJointMaxVelRatio(temp);
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(QString("%1").arg(temp));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        m_tableRatioParameter->setItem(count, 1, item);
        count++;
    }

    for(int i=0; i<1; i++){
        m_robot.getJointMaxAccRatio(temp);
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(QString("%1").arg(temp));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        m_tableRatioParameter->setItem(count, 1, item);
        count++;
    }

    for(int i=0; i<1; i++){
        m_robot.getJointMaxJerkRatio(temp);
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(QString("%1").arg(temp));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        m_tableRatioParameter->setItem(count, 1, item);
        count++;
    }

    for(int i=0; i<1; i++){
        m_robot.getTerminalMaxVelRatio(temp);
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(QString("%1").arg(temp));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        m_tableRatioParameter->setItem(count, 1, item);
        count++;
    }

    for(int i=0; i<1; i++){
        m_robot.getTerminalMaxAccRatio(temp);
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(QString("%1").arg(temp));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        m_tableRatioParameter->setItem(count, 1, item);
        count++;
    }

    for(int i=0; i<1; i++){
        m_robot.getTerminalMaxJerkRatio(temp);
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(QString("%1").arg(temp));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        m_tableRatioParameter->setItem(count, 1, item);
        count++;
    }
}

void RobotParameterSettingWidget::fillMotionParameterTableValue(){
    int count = 0;
    Joints tempJoint;
    Terminal tempTerminal;
    for(int j=0; j<1; j++){
        tempJoint=m_robot.getJointRangeMinus(
                    );
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempJoint[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        tempJoint=m_robot.getJointRangePlus();
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempJoint[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        m_robot.getJointMaxVelRange(tempJoint);
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempJoint[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        m_robot.getJointMaxAccRange(tempJoint);
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempJoint[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        m_robot.getJointMaxJerkRange(tempJoint);
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempJoint[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        tempTerminal=m_robot.getTerminalRangeMinus();
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempTerminal[static_cast<TERMINALINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        tempTerminal=m_robot.getTerminalRangePlus();
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempTerminal[static_cast<TERMINALINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        tempTerminal=m_robot.getTerminalMaxVelRange();
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempTerminal[static_cast<TERMINALINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        tempTerminal=m_robot.getTerminalMaxAccRange();
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempTerminal[static_cast<TERMINALINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        tempTerminal=m_robot.getTerminalMaxJerkRange();
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempTerminal[static_cast<TERMINALINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableMotionParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }
}

void RobotParameterSettingWidget::fillJointParameterTableValue(){
    int count = 0;
    Joints tempJoint;
    for(int j=0; j<1; j++){
        m_robot.getEncoderResolution(tempJoint);
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempJoint[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableJointParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        m_robot.getRateTorque(tempJoint);
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempJoint[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableJointParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        m_robot.getReduceRatio(tempJoint);
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tempJoint[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableJointParameter->setItem(count+i, 1, item);
        }
        count += 6;
    }
}

void RobotParameterSettingWidget::fillModelParameterTableValue(){
    int count=0;
    Joints DHParameterAlpha, DHParameterA, DHParameterD, DHParameterTheta;
    m_robot.getDHParameter(DHParameterAlpha, DHParameterA, DHParameterD, DHParameterTheta);
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(DHParameterAlpha[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableModelParameter->setItem(i, count, item);
        }
        count+=1;

        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(DHParameterA[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableModelParameter->setItem(i, count, item);
        }
        count+=1;

        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(DHParameterD[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableModelParameter->setItem(i, count, item);
        }
        count+=1;

        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(DHParameterTheta[static_cast<JOINTINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            m_tableModelParameter->setItem(i, count, item);
        }

}

void RobotParameterSettingWidget::getRatioParameterTableValue(){
    int count = 0;
    for(int i=0; i<1; i++){
        QTableWidgetItem* item = m_tableRatioParameter->item(count, 1);
        m_robot.setJointMaxVelRatio(item->text().toDouble());
        count++;
    }

    for(int i=0; i<1; i++){
        QTableWidgetItem* item = m_tableRatioParameter->item(count, 1);
        m_robot.setJointMaxAccRatio(item->text().toDouble());
        count++;
    }

    for(int i=0; i<1; i++){
        QTableWidgetItem* item = m_tableRatioParameter->item(count, 1);
        m_robot.setJointMaxJerkRatio(item->text().toDouble());
        count++;
    }

    for(int i=0; i<1; i++){
        QTableWidgetItem* item = m_tableRatioParameter->item(count, 1);
        m_robot.setTerminalMaxVelRatio(item->text().toDouble());
        count++;
    }

    for(int i=0; i<1; i++){
        QTableWidgetItem* item = m_tableRatioParameter->item(count, 1);
        m_robot.setTerminalMaxAccRatio(item->text().toDouble());
        count++;
    }

    for(int i=0; i<1; i++){
        QTableWidgetItem* item = m_tableRatioParameter->item(count, 1);
        m_robot.setTerminalMaxJerkRatio(item->text().toDouble());
        count++;
    }
}

void RobotParameterSettingWidget::getMotionParameterTableValue(){
    int count = 0;   
    Joints jointRangeMinus,jointRangePlus;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            jointRangeMinus[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            jointRangePlus[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setJointRange(jointRangeMinus,jointRangePlus);

    Joints jointMaxVel;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            jointMaxVel[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setJointMaxVelRange(jointMaxVel);

    Joints jointMaxAcc;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            jointMaxAcc[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setJointMaxAccRange(jointMaxAcc);

    Joints jointMaxJerk;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            jointMaxJerk[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setJointMaxJerkRange(jointMaxJerk);

    Terminal terminalRangeMinus, terminalRangePlus;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            terminalRangeMinus[static_cast<TERMINALINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }

    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            terminalRangePlus[static_cast<TERMINALINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setTerminalRange(terminalRangeMinus, terminalRangePlus);

    Terminal terminalMaxVel;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            terminalMaxVel[static_cast<TERMINALINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setTerminalMaxVelRange(terminalMaxVel);

    Terminal terminalMaxAcc;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            terminalMaxAcc[static_cast<TERMINALINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setTerminalMaxAccRange(terminalMaxAcc);

    Terminal terminalMaxJerk;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableMotionParameter->item(count+i, 1);
            terminalMaxJerk[static_cast<TERMINALINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setTerminalMaxJerkRange(terminalMaxJerk);
}

void RobotParameterSettingWidget::getJointParameterTableValue(){
    int count = 0;
    Joints encoderResolution;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableJointParameter->item(count+i, 1);
            encoderResolution[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setEncoderResolution(encoderResolution);

    Joints rateTorque;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableJointParameter->item(count+i, 1);
            rateTorque[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setRateTorque(rateTorque);

    Joints reduceRatio;
    for(int j=0; j<1; j++){
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = m_tableJointParameter->item(count+i, 1);
            reduceRatio[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count += 6;
    }
    m_robot.setReduceRatio(reduceRatio);
}

void RobotParameterSettingWidget::getModelParameterTableValue(){ 
    int count=0;
    QTableWidgetItem* item;
    Joints DHParameterAlpha, DHParameterA, DHParameterD, DHParameterTheta;
        for(int i=0; i<6; i++){
            item = m_tableModelParameter->item(i, count);
            DHParameterAlpha[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count+=1;

        for(int i=0; i<6; i++){
            item = m_tableModelParameter->item(i, count);
            DHParameterA[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count+=1;

        for(int i=0; i<6; i++){
            item = m_tableModelParameter->item(i, count);
            DHParameterD[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        count+=1;

        for(int i=0; i<6; i++){
            item = m_tableModelParameter->item(i, count);
            DHParameterTheta[static_cast<JOINTINDEX>(i)] = item->text().toDouble();
        }
        m_robot.setDHParameter(DHParameterAlpha, DHParameterA, DHParameterD, DHParameterTheta);
}

void RobotParameterSettingWidget::buttonSaveRobotParameterClicked(){
    m_robot.setRobotType(static_cast<ROBOTTYPE>(m_comboRobotType->currentIndex()));
    m_robot.setSamplePeriod(m_editRobotPeriod->text().toDouble() / 1000);
    getRatioParameterTableValue();
    getMotionParameterTableValue();
    getJointParameterTableValue();
    getModelParameterTableValue();

    RobotClient::initance()->setRobotParameter(m_robot);

    QMessageBox::question(this, tr("提示"),
                          tr("保存机器人参数成功!"),
                          QMessageBox::Yes);
}

void RobotParameterSettingWidget::comboRobotTypeChanged(int)
{
    switch(m_comboRobotType->currentIndex())
    {
    case 0:
        m_robot.setRobotType(ROBSOFT_SERIAL_SIX_CONVENTION);
        break;
    case 1:
        m_robot.setRobotType(ROBSOFT_SERIAL_SIX_COOPERATION);
        break;
    case 2:
        m_robot.setRobotType(ROBSOFT_SERIAL_FOUR_CONVENTION);
        break;
    case 3:
        m_robot.setRobotType(ROBOTSOFT_SCARA_FOUR_ONERF);
        break;
    case 4:
        m_robot.setRobotType(ROBOTSOFT_SCARA_FOUR_FOURRF);
        break;
    case 5:
        m_robot.setRobotType(ROBOTSOFT_DELTA_FOUR);
        break;
    case 6:
        m_robot.setRobotType(ROBOTSOFT_DELTA_SIX);
        break;
    default:
        break;
    }
}

void RobotParameterSettingWidget::updateOperationEnabled(bool state){
    m_buttonSaveRobotParameter->setEnable(state);
}
