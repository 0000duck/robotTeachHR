#include "robotFrameSettingWidget.h"

#include <QDebug>

using namespace std;
using namespace rclib;

RobotFrameSettingWidget::RobotFrameSettingWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, "#808080");
    this->setPalette(pe);

    initUI();
    initState();
    connectSignalSlot();
}

RobotFrameSettingWidget::~RobotFrameSettingWidget(){

}

void RobotFrameSettingWidget::initUI(){
    m_tableFrame = new QTableWidget();

    m_radioChooseToolFrame = new QRadioButton(tr("工具坐标系"));
    m_radioChooseUserFrame = new QRadioButton(tr("工件坐标系"));

    QHBoxLayout* layoutRadioChooseFrame = new QHBoxLayout();
    layoutRadioChooseFrame->addWidget(m_radioChooseToolFrame);
    layoutRadioChooseFrame->addWidget(m_radioChooseUserFrame);

    QGroupBox* groupRadioChooseFrame = new QGroupBox();
    groupRadioChooseFrame->setLayout(layoutRadioChooseFrame);

    m_buttonAddFrame = new PushButton(tr("添加坐标系"));
    m_buttonAddFrame->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonModifyFrame = new PushButton(tr("修改坐标系"));
    m_buttonModifyFrame->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonDeleteFrame = new PushButton(tr("删除坐标系"));
    m_buttonDeleteFrame->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonCalibrateFrame = new PushButton(tr("标定坐标系"));
    m_buttonCalibrateFrame->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonChooseFrame = new PushButton(tr("设定坐标系"));
    m_buttonChooseFrame->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutFrameOperation = new QHBoxLayout();
    layoutFrameOperation->addWidget(m_buttonAddFrame);
    layoutFrameOperation->addWidget(m_buttonModifyFrame);
    layoutFrameOperation->addWidget(m_buttonDeleteFrame);
    layoutFrameOperation->addWidget(m_buttonCalibrateFrame);
    layoutFrameOperation->addWidget(m_buttonChooseFrame);

    QGroupBox* groupFrameOperation = new QGroupBox();
    groupFrameOperation->setLayout(layoutFrameOperation);

    QHBoxLayout* layoutFrameChoose = new QHBoxLayout();
    layoutFrameChoose->addWidget(groupFrameOperation, 0, Qt::AlignLeft);
    layoutFrameChoose->addWidget(groupRadioChooseFrame, 0, Qt::AlignRight);

    m_widgetFrame = new QWidget();

    QVBoxLayout* layoutFrame = new QVBoxLayout(m_widgetFrame);
    layoutFrame->addLayout(layoutFrameChoose);
    layoutFrame->addWidget(m_tableFrame);

    m_widgetFrameEdit = new FrameEditWidget(this);

    QVBoxLayout* layoutFrameEdit = new QVBoxLayout(this);
    layoutFrameEdit->addWidget(m_widgetFrame);
    layoutFrameEdit->addWidget(m_widgetFrameEdit);
}

void RobotFrameSettingWidget::initState(){
    initFrameTable();

    m_radioChooseToolFrame->setChecked(true);

    updateFrame();

    m_widgetFrameEdit->hide();
}

void RobotFrameSettingWidget::connectSignalSlot(){
    QObject::connect(m_radioChooseToolFrame, SIGNAL(toggled(bool)), this, SLOT(updateFrame()));
    QObject::connect(m_radioChooseUserFrame, SIGNAL(toggled(bool)), this, SLOT(updateFrame()));
    QObject::connect(m_buttonAddFrame, SIGNAL(clicked()), this, SLOT(buttonAddFrameClicked()));
    QObject::connect(m_buttonModifyFrame, SIGNAL(clicked()), this, SLOT(buttonModifyFrameClicked()));
    QObject::connect(m_buttonDeleteFrame, SIGNAL(clicked()), this, SLOT(buttonDeleteFrameClicked()));
    QObject::connect(m_buttonCalibrateFrame, SIGNAL(clicked()), this, SLOT(buttonCalibrateFrameClicked()));
    QObject::connect(m_buttonChooseFrame, SIGNAL(clicked()), this, SLOT(buttonChooseFrameClicked()));

    QObject::connect(m_widgetFrameEdit, SIGNAL(frameEditFinishedSignal(bool)), this, SLOT(frameEditFinished(bool)));
}

void RobotFrameSettingWidget::initFrameTable(){
    m_tableFrame->setColumnCount(8);
    m_tableFrame->horizontalHeader()->setSectionsClickable(false);
    QStringList header;
    header<<tr("坐标系名")<<tr("X")<<tr("Y")<<tr("Z")<<tr("A")<<tr("B")<<tr("C")<<tr("当前坐标系");
    m_tableFrame->setHorizontalHeaderLabels(header);
    QFont font = m_tableFrame->horizontalHeader()->font();
    font.setBold(true);
    m_tableFrame->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableFrame->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableFrame->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    m_tableFrame->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableFrame->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableFrame->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableFrame->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableFrame->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableFrame->setSelectionMode(QAbstractItemView::SingleSelection);
    //    m_tableFrame->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableFrame->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                       "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                       "QScrollBar::handle:hover{background:gray;}"
                                                       "QScrollBar::sub-line{background:transparent;}"
                                                       "QScrollBar::add-line{background:transparent;}");
    m_tableFrame->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
                                                     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                     "QScrollBar::handle:hover{background:gray;}"
                                                     "QScrollBar::sub-line{background:transparent;}"
                                                     "QScrollBar::add-line{background:transparent;}");
}

void RobotFrameSettingWidget::updateToolFrameTable(){
    m_tableFrame->clearContents();
    m_tableFrame->setRowCount(0);

    int row = 0;
    for(std::map<std::string, Terminal>::iterator it = m_toolFrame.begin(); it != m_toolFrame.end(); it++){
        m_tableFrame->insertRow(row);
        for(int i=0; i<1; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString::fromStdString(it->first));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableFrame->setItem(row, i, item);
        }

        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(it->second[static_cast<TERMINALINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableFrame->setItem(row, i+1, item);
        }

        for(int i=0; i<1; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            if(it->first == m_toolFrameName){
                item->setIcon(QIcon(QString(WINDOWICONPATH) + QString("check.png")));
            }
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableFrame->setItem(row, 7, item);
        }

        row++;
    }
}

void RobotFrameSettingWidget::updateUserFrameTable(){
    m_tableFrame->clearContents();
    m_tableFrame->setRowCount(0);

    int row = 0;
    for(std::map<std::string, Terminal>::iterator it = m_userFrame.begin(); it != m_userFrame.end(); it++){
        m_tableFrame->insertRow(row);
        for(int i=0; i<1; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString::fromStdString(it->first));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableFrame->setItem(row, i, item);
        }

        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(it->second[static_cast<TERMINALINDEX>(i)]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableFrame->setItem(row, i+1, item);
        }

        for(int i=0; i<1; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            if(it->first == m_userFrameName){
                item->setIcon(QIcon(QString(WINDOWICONPATH) + QString("check.png")));
            }
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableFrame->setItem(row, 7, item);
        }

        row++;
    }
}

void RobotFrameSettingWidget::updateFrame(){
    if(m_radioChooseToolFrame->isChecked()){
        m_chooseFrameFlag = 0;
    }
    else if(m_radioChooseUserFrame->isChecked()){
        m_chooseFrameFlag = 1;
    }

    m_RobotFrame=RobotClient::initance()->getRobotFrame();

    m_toolFrame = m_RobotFrame.getToolFrameList();
    m_userFrame = m_RobotFrame.getWorkFrameList();

    m_toolFrameName = m_RobotFrame.getCurrentToolFrame();
    m_userFrameName = m_RobotFrame.getCurrentWorkFrame();

    switch(m_chooseFrameFlag){
    case 0:updateToolFrameTable();
        break;
    case 1:updateUserFrameTable();
        break;
    default:
        break;
    }
}

void RobotFrameSettingWidget::buttonAddFrameClicked(){
    switch(m_chooseFrameFlag){
    case 0:
        m_widgetFrame->hide();
        m_widgetFrameEdit->setMode(0, m_chooseFrameFlag);
        m_widgetFrameEdit->passParameter(&m_RobotFrame);
        m_widgetFrameEdit->show();
        break;
    case 1:
        m_widgetFrame->hide();
        m_widgetFrameEdit->setMode(0, m_chooseFrameFlag);
        m_widgetFrameEdit->passParameter(&m_RobotFrame);
        m_widgetFrameEdit->show();
        break;
    default:
        break;
    }
}

void RobotFrameSettingWidget::buttonDeleteFrameClicked(){
    int currentRowIndex = m_tableFrame->currentRow();
    if(currentRowIndex == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("请先选中需要删除的坐标系!"),
                              QMessageBox::Yes);
        return;
    }

    QTableWidgetItem* item = m_tableFrame->item(currentRowIndex, 0);
    string text = item->text().toStdString();

    if(text == "Default"){
        QMessageBox::question(this, tr("提示"),
                              tr("默认坐标系不可删除!"),
                              QMessageBox::Yes);
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("提示"),
                                  tr("确定要删除该坐标系吗？"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
        return;

    switch(m_chooseFrameFlag){
    case 0:{
        m_RobotFrame.deleteToolFrame(text);
    }
        break;
    case 1:{
        m_RobotFrame.deleteWorkFrame(text);
    }
        break;
    default:
        break;
    }

    frameEditFinished(true);
}

void RobotFrameSettingWidget::buttonModifyFrameClicked(){
    int currentRowIndex = m_tableFrame->currentRow();
    if(currentRowIndex == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("请先选中需要修改的坐标系!"),
                              QMessageBox::Yes);
        return;
    }

    QTableWidgetItem* item = m_tableFrame->item(currentRowIndex, 0);
    string text = item->text().toStdString();

    if(text == "Default"){
        QMessageBox::question(this, tr("提示"),
                              tr("默认坐标系不可修改!"),
                              QMessageBox::Yes);
        return;
    }

    switch(m_chooseFrameFlag){
    case 0:
        m_widgetFrame->hide();
        m_widgetFrameEdit->setMode(1, m_chooseFrameFlag);  //更新坐标系类型，0：tool 1:work
        m_widgetFrameEdit->passParameter(&m_RobotFrame, text);
        m_widgetFrameEdit->setMode(1, m_chooseFrameFlag);  //根据上一行的text初始化
        m_widgetFrameEdit->show();
        break;
    case 1:
        m_widgetFrame->hide();
        m_widgetFrameEdit->setMode(1, m_chooseFrameFlag);
        m_widgetFrameEdit->passParameter(&m_RobotFrame, text);
        m_widgetFrameEdit->setMode(1, m_chooseFrameFlag);
        m_widgetFrameEdit->show();
        break;
    default:
        break;
    }
}

void RobotFrameSettingWidget::buttonCalibrateFrameClicked(){
    switch(m_chooseFrameFlag){
    case 0:
        m_widgetFrame->hide();
        m_widgetFrameEdit->show();
        m_widgetFrameEdit->setMode(2, m_chooseFrameFlag);
        m_widgetFrameEdit->passParameter(&m_RobotFrame);
        break;
    case 1:
        m_widgetFrame->hide();
        m_widgetFrameEdit->show();
        m_widgetFrameEdit->setMode(2, m_chooseFrameFlag);
        m_widgetFrameEdit->passParameter(&m_RobotFrame);
        break;
    default:
        break;
    }
}

void RobotFrameSettingWidget::buttonChooseFrameClicked(){
    int currentRowIndex = m_tableFrame->currentRow();
    if(currentRowIndex == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("请先选中需要设定的坐标系!"),
                              QMessageBox::Yes);
        return;
    }

    string name;
    switch(m_chooseFrameFlag){
    case 0:{
        name = m_toolFrameName;
    }
        break;
    case 1:{
        name = m_userFrameName;
    }
        break;
    default:
        break;
    }

    for(int i=0; i<m_tableFrame->rowCount(); i++){
        QTableWidgetItem* item = m_tableFrame->item(i, 0);
        if(item->text().toStdString() == name){
            QTableWidgetItem* item1 = m_tableFrame->item(i, 7);
            item1->setIcon(QIcon());
        }
    }

    QTableWidgetItem* item = m_tableFrame->item(currentRowIndex, 0);
    string text = item->text().toStdString();
    QTableWidgetItem* item1 = m_tableFrame->item(currentRowIndex, 7);
    item1->setIcon(QIcon(QString(WINDOWICONPATH) + QString("check.png")));

    switch(m_chooseFrameFlag){
    case 0:{
        m_toolFrameName = text;
        RobotClient::initance()->modifyToolFrame(m_toolFrameName);
    }
        break;
    case 1:{
        m_userFrameName = text;  
        RobotClient::initance()->modifyWorkFrame(m_userFrameName);
    }
        break;
    default:
        break;
    }

    updateAllFrame();
}

void RobotFrameSettingWidget::frameEditFinished(bool state){
    m_widgetFrame->show();
    m_widgetFrameEdit->hide();

    if(state){
        RobotClient::initance()->setRobotFrame(m_RobotFrame);
    }
}

void RobotFrameSettingWidget::updateOperationEnabled(bool state){
    m_buttonAddFrame->setEnable(state);
    m_buttonModifyFrame->setEnable(state);
    m_buttonDeleteFrame->setEnable(state);
    m_buttonCalibrateFrame->setEnable(state);
    m_buttonChooseFrame->setEnable(state);
}

void RobotFrameSettingWidget::updateAllFrame()
{
    m_RobotFrame = RobotClient::initance()->getRobotFrame();

    m_toolFrame = m_RobotFrame.getToolFrameList();
    m_userFrame = m_RobotFrame.getWorkFrameList();

    m_toolFrameName = m_RobotFrame.getCurrentToolFrame();
    m_userFrameName = m_RobotFrame.getCurrentWorkFrame();

    switch(m_chooseFrameFlag){
    case 0:updateToolFrameTable();
        break;
    case 1:updateUserFrameTable();
        break;
    default:
        break;
    }
}
