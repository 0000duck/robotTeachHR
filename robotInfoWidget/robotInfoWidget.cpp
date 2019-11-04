#include "robotInfoWidget.h"

using namespace std;
using namespace rclib;

RobotInfoWidget::RobotInfoWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, "#808080");
    this->setPalette(pe);

    initUI();
    initState();
}

RobotInfoWidget::~RobotInfoWidget(){

}

void RobotInfoWidget::initUI(){
    m_tableInfomation = new QTableWidget();

    QHBoxLayout* layoutInfo = new QHBoxLayout(this);
    layoutInfo->addWidget(m_tableInfomation);
}

void RobotInfoWidget::initState(){
//    m_tableInfomation->setFixedSize(0.825 * WINDOWWIDTH, 0.72 * WINDOWHIGHT);
    m_tableInfomation->setColumnCount(2);
    m_tableInfomation->horizontalHeader()->setDefaultSectionSize(200);
    m_tableInfomation->horizontalHeader()->setSectionsClickable(false);

    QStringList header;
    header<<tr("时间")<<tr("消息内容");
    m_tableInfomation->setHorizontalHeaderLabels(header);

    QFont font = m_tableInfomation->horizontalHeader()->font();
    font.setBold(true);
    m_tableInfomation->horizontalHeader()->setFont(font);
    m_tableInfomation->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableInfomation->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度

    m_tableInfomation->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //m_tableInfomation->verticalHeader()->setDefaultSectionSize(10); //设置行高
    //m_tableInfomation->verticalHeader()->setVisible(false); //设置垂直头不可见
    m_tableInfomation->verticalHeader()->setStyleSheet("QHeaderView::section{background:black;}");
    m_tableInfomation->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    m_tableInfomation->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableInfomation->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableInfomation->setSelectionMode(QAbstractItemView::SingleSelection);

    //设置水平、垂直滚动条样式
    m_tableInfomation->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
    m_tableInfomation->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
}

void RobotInfoWidget::updateNewInfo(INFOSTATE state, QString content){
    if(state != INFO_ERROR && state != INFO_WARNING && state != INFO_RECOM){
        return;
    }

    int rowcount = m_tableInfomation->rowCount();
    if(rowcount > 100){
        m_tableInfomation->removeRow(0);
        rowcount--;
    }

    m_tableInfomation->insertRow(rowcount);

    for(int i=0; i<1; i++){
        QDateTime timeCurrent = QDateTime::currentDateTime();
        QString time = timeCurrent.toString("yyyy-MM-dd hh:mm:ss");

        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(time);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        m_tableInfomation->setItem(rowcount, 0, item);
    }

    for(int i=0; i<1; i++){
        QTableWidgetItem* item = new QTableWidgetItem();
        switch(state){
        case INFO_ERROR:{
            item->setIcon(QIcon(QString(WINDOWICONPATH) + QString("error.png")));
            item->setText(content);
        }
            break;
        case INFO_WARNING:{
            item->setIcon(QIcon(QString(WINDOWICONPATH) + QString("warn.png")));
            item->setText(content);
        }
            break;
        case INFO_RECOM:{
            item->setIcon(QIcon(QString(WINDOWICONPATH) + QString("info.png")));
            item->setText(content);
        }
            break;
        default:
            break;
        }

        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        m_tableInfomation->setItem(rowcount, 1, item);
    }

    m_tableInfomation->verticalScrollBar()->setSliderPosition(rowcount);
}
