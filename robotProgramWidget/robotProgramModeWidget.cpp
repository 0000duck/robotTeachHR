#include "robotProgramModeWidget.h"

#include <QDebug>

using namespace std;
using namespace rclib;

RobotProgramModeWidget::RobotProgramModeWidget(QWidget *parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramModeWidget::~RobotProgramModeWidget(){

}

void RobotProgramModeWidget::passParameter(RobotInterpreter* p_file){
    m_fileSrc = p_file;
}

void RobotProgramModeWidget::setMode(OPERATIONMODE mode){
    m_mode = mode;

    vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();

    if(m_mode == TEACH_MODIFY){
        VECTOR_STRING tmp_Strentence=Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");
        string name = tmp_Strentence[0];
        if(name == string("MOVB")){
            m_radioBCurve->setChecked(true);
            radioModeTypeChanged();

            m_editBCurveAcc->setText(QString::fromStdString(tmp_Strentence[1]));
            m_editBCurveJerk->setText(QString::fromStdString(tmp_Strentence[2]));
            m_editBCurveAngleVel->setText(QString::fromStdString(tmp_Strentence[3]));
            m_editBCurvePre->setText(QString::fromStdString(tmp_Strentence[4]));

            m_buttonBCurveFrame->setState(string_to_int(tmp_Strentence[5]));
            m_buttonBCurveFrame->decState();
            buttonBCurveFrameClicked();

            m_tableBCurve->clear();
            m_tableBCurve->setRowCount(0);

            int row = 0;
            for(int i=7; i<tmp_Strentence.size(); ){
                m_tableBCurve->insertRow(row);
                for(int j=0; j<7; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(tmp_Strentence[i]));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableBCurve->setItem(row, j, item);

                    i++;
                }
                row++;
            }
        }
        else if(name == string("MOVJT")){
            m_radioDragTeach->setChecked(true);
            radioModeTypeChanged();

            m_editDragTeachPlayPeriod->setText(QString::fromStdString(tmp_Strentence[1]));

            m_tableDragTeach->clear();
            m_tableDragTeach->setRowCount(0);

            int row = 0;
            for(int i=3; i<tmp_Strentence.size(); ){
                m_tableDragTeach->insertRow(row);
                for(int j=0; j<6; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(tmp_Strentence[i]));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableDragTeach->setItem(row, j, item);

                    i++;
                }
                row++;
            }
        }

        groupModeType->setEnabled(false);
    }
    else{
        clearState();
    }
}

void RobotProgramModeWidget::passSentenceIndex(int func, int line){
    m_func = func;
    m_line = line;
}

void RobotProgramModeWidget::initUI(){
    m_radioBCurve = new QRadioButton(tr("样条曲线"));
    m_radioDragTeach = new QRadioButton(tr("拖动示教"));

    QHBoxLayout* layoutModeType = new QHBoxLayout();
    layoutModeType->addWidget(m_radioBCurve);
    layoutModeType->addWidget(m_radioDragTeach);
    groupModeType = new QGroupBox(tr("模式"));
    groupModeType->setLayout(layoutModeType);

    m_buttonAddPoint = new PushButton(tr("添加点"));
    m_buttonInsertPoint = new PushButton(tr("插入点"));
    m_buttonDeletePoint = new PushButton(tr("删除点"));
    m_buttonClearPoint = new PushButton(tr("清除点"));
    m_buttonMovePoint = new PushButton(tr("移动到"));
    m_buttonAddPoint->setFixedSize(0.08*WINDOWWIDTH, 0.03*WINDOWWIDTH);
    m_buttonInsertPoint->setFixedSize(0.08*WINDOWWIDTH, 0.03*WINDOWWIDTH);
    m_buttonDeletePoint->setFixedSize(0.08*WINDOWWIDTH, 0.03*WINDOWWIDTH);
    m_buttonClearPoint->setFixedSize(0.08*WINDOWWIDTH, 0.03*WINDOWWIDTH);
    m_buttonMovePoint->setFixedSize(0.08*WINDOWWIDTH, 0.03*WINDOWWIDTH);

    QHBoxLayout* layoutBCurvePoint = new QHBoxLayout();
    layoutBCurvePoint->addWidget(m_buttonAddPoint);
    layoutBCurvePoint->addWidget(m_buttonInsertPoint);
    layoutBCurvePoint->addWidget(m_buttonDeletePoint);
    layoutBCurvePoint->addWidget(m_buttonClearPoint);
    layoutBCurvePoint->addWidget(m_buttonMovePoint);
    m_groupPointOperation = new QGroupBox();
    m_groupPointOperation->setLayout(layoutBCurvePoint);

    QLabel* labelBCurveVel = new QLabel(tr("默认\n速度"));
    QLabel* labelBCurveAcc = new QLabel(tr("加\n速度"));
    QLabel* labelBCurveJerk = new QLabel(tr("冲击"));
    QLabel* labelBCurveAngleVel = new QLabel(tr("角\n速度"));
    QLabel* labelBCurvePre = new QLabel(tr("精度"));
    QLabel* labelBCurveFrame = new QLabel(tr("坐标\n系"));
    m_editBCurveVel = new QLineEdit();
    m_editBCurveAcc = new QLineEdit();
    m_editBCurveJerk = new QLineEdit();
    m_editBCurveAngleVel = new QLineEdit();
    m_editBCurvePre = new QLineEdit();
    m_buttonBCurveFrame = new PushButton(tr("基坐标"));
    m_editBCurveVel->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editBCurveAcc->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editBCurveJerk->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editBCurveAngleVel->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editBCurvePre->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonBCurveFrame->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutBCurveVel = new QHBoxLayout();
    layoutBCurveVel->addWidget(labelBCurveVel);
    layoutBCurveVel->addWidget(m_editBCurveVel);
    QHBoxLayout* layoutBCurveAcc = new QHBoxLayout();
    layoutBCurveAcc->addWidget(labelBCurveAcc);
    layoutBCurveAcc->addWidget(m_editBCurveAcc);
    QHBoxLayout* layoutBCurveJerk = new QHBoxLayout();
    layoutBCurveJerk->addWidget(labelBCurveJerk);
    layoutBCurveJerk->addWidget(m_editBCurveJerk);
    QHBoxLayout* layoutBCurveAngleVel = new QHBoxLayout();
    layoutBCurveAngleVel->addWidget(labelBCurveAngleVel);
    layoutBCurveAngleVel->addWidget(m_editBCurveAngleVel);
    QHBoxLayout* layoutBCurvePre = new QHBoxLayout();
    layoutBCurvePre->addWidget(labelBCurvePre);
    layoutBCurvePre->addWidget(m_editBCurvePre);
    QHBoxLayout* layoutBCurveFrame = new QHBoxLayout();
    layoutBCurveFrame->addWidget(labelBCurveFrame);
    layoutBCurveFrame->addWidget(m_buttonBCurveFrame);

    QVBoxLayout* layoutBCurveParameter1 = new QVBoxLayout();
    layoutBCurveParameter1->addLayout(layoutBCurveVel);
    layoutBCurveParameter1->addLayout(layoutBCurveAngleVel);
    QVBoxLayout* layoutBCurveParameter2 = new QVBoxLayout();
    layoutBCurveParameter2->addLayout(layoutBCurveAcc);
    layoutBCurveParameter2->addLayout(layoutBCurvePre);
    QVBoxLayout* layoutBCurveParameter3 = new QVBoxLayout();
    layoutBCurveParameter3->addLayout(layoutBCurveJerk);
    layoutBCurveParameter3->addLayout(layoutBCurveFrame);
    QHBoxLayout* layoutBCurveParameter = new QHBoxLayout();
    layoutBCurveParameter->addLayout(layoutBCurveParameter1);
    layoutBCurveParameter->addLayout(layoutBCurveParameter2);
    layoutBCurveParameter->addLayout(layoutBCurveParameter3);
    m_groupBCurveParameter = new QGroupBox();
    m_groupBCurveParameter->setLayout(layoutBCurveParameter);

    QLabel* labelDragTeachSamplePeriod = new QLabel(tr("采样\n周期/ms"));
    QLabel* labelDragTeachTime = new QLabel(tr("采样\n时长/s"));
    QLabel* labelDragTeachPlayPeriod = new QLabel(tr("再现\n周期/ms"));
    m_editDragTeachSamplePeriod = new QLineEdit();
    m_editDragTeachTime = new QLineEdit();
    m_editDragTeachPlayPeriod = new QLineEdit();
    m_buttonStartDragRecord = new PushButton(tr("开始拖动记录"));
    m_buttonDragPlayPrepare = new PushButton(tr("拖动再现准备"));
    m_buttonDragPlay = new PushButton(tr("开始拖动再现"));
    m_editDragTeachSamplePeriod->setMinimumSize(0.05*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editDragTeachTime->setMinimumSize(0.05*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editDragTeachPlayPeriod->setMinimumSize(0.05*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonStartDragRecord->setMinimumSize(0.06*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonDragPlayPrepare->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonDragPlay->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutDragTeachSamplePeriod = new QHBoxLayout();
    layoutDragTeachSamplePeriod->addWidget(labelDragTeachSamplePeriod);
    layoutDragTeachSamplePeriod->addWidget(m_editDragTeachSamplePeriod);
    QHBoxLayout* layoutDragTeachTime = new QHBoxLayout();
    layoutDragTeachTime->addWidget(labelDragTeachTime);
    layoutDragTeachTime->addWidget(m_editDragTeachTime);
    QHBoxLayout* layoutDragTeachPlayPeriod = new QHBoxLayout();
    layoutDragTeachPlayPeriod->addWidget(labelDragTeachPlayPeriod);
    layoutDragTeachPlayPeriod->addWidget(m_editDragTeachPlayPeriod);

    QVBoxLayout* layoutDragTeachParameter1 = new QVBoxLayout();
    layoutDragTeachParameter1->addLayout(layoutDragTeachSamplePeriod);
    layoutDragTeachParameter1->addLayout(layoutDragTeachPlayPeriod);
    QVBoxLayout* layoutDragTeachParameter2 = new QVBoxLayout();
    layoutDragTeachParameter2->addLayout(layoutDragTeachTime);
    layoutDragTeachParameter2->addWidget(m_buttonDragPlayPrepare);
    QVBoxLayout* layoutDragTeachParameter3 = new QVBoxLayout();
    layoutDragTeachParameter3->addWidget(m_buttonStartDragRecord);
    layoutDragTeachParameter3->addWidget(m_buttonDragPlay);
    QHBoxLayout* layoutDragTeachParameter = new QHBoxLayout();
    layoutDragTeachParameter->addLayout(layoutDragTeachParameter1);
    layoutDragTeachParameter->addLayout(layoutDragTeachParameter2);
    layoutDragTeachParameter->addLayout(layoutDragTeachParameter3);
    m_groupDragTeachParameter = new QGroupBox();
    m_groupDragTeachParameter->setLayout(layoutDragTeachParameter);

    QVBoxLayout* layoutParameter = new QVBoxLayout();
    layoutParameter->addWidget(m_groupBCurveParameter);
    layoutParameter->addWidget(m_groupDragTeachParameter);

    m_tableBCurve = new QTableWidget();
    m_tableDragTeach = new QTableWidget();

    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    m_buttonYes->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutSure = new QVBoxLayout();
    layoutSure->addWidget(m_buttonYes);
    layoutSure->addWidget(m_buttonNo);
    QGroupBox* groupSure = new QGroupBox();
    groupSure->setLayout(layoutSure);

    QHBoxLayout* layoutParameterSure = new QHBoxLayout();
    layoutParameterSure->addLayout(layoutParameter);
    layoutParameterSure->addWidget(groupSure);

    QVBoxLayout* layoutMode = new QVBoxLayout(this);
    layoutMode->addWidget(groupModeType);
    layoutMode->addWidget(m_groupPointOperation);
    layoutMode->addWidget(m_tableBCurve);
    layoutMode->addWidget(m_tableDragTeach);
    layoutMode->addLayout(layoutParameterSure);
    layoutMode->setStretchFactor(groupModeType, 1);
    layoutMode->setStretchFactor(m_groupPointOperation, 1);
    layoutMode->setStretchFactor(m_tableBCurve, 5);
    layoutMode->setStretchFactor(m_tableDragTeach, 5);
    layoutMode->setStretchFactor(layoutParameterSure, 2);
}

void RobotProgramModeWidget::initState(){
    m_buttonBCurveFrame->setStateSum(2);

    initBCurveTable();
    initDragTeachTable();

    m_editBCurveVel->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editBCurveAcc->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editBCurveJerk->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editBCurveAngleVel->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editBCurvePre->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));

    m_editDragTeachSamplePeriod->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editDragTeachTime->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editDragTeachPlayPeriod->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));

    clearState();

    m_editDragTeachSamplePeriod->setText("200");
    m_editDragTeachTime->setText("10");
    m_editDragTeachPlayPeriod->setText("200");

    m_editBCurveVel->setText("100");
    m_editBCurveAcc->setText("0.8");
    m_editBCurveJerk->setText("0.8");
    m_editBCurveAngleVel->setText("0.8");
    m_editBCurvePre->setText("0.01");
}

void RobotProgramModeWidget::connectSignalSlot(){
    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));

    QObject::connect(m_buttonAddPoint, SIGNAL(clicked()), this, SLOT(buttonAddPointClicked()));
    QObject::connect(m_buttonInsertPoint, SIGNAL(clicked()), this, SLOT(buttonInsertPointClicked()));
    QObject::connect(m_buttonDeletePoint, SIGNAL(clicked()), this, SLOT(buttonDeletePointClicked()));
    QObject::connect(m_buttonClearPoint, SIGNAL(clicked()), this, SLOT(buttonClearPointClicked()));
    QObject::connect(m_buttonMovePoint, SIGNAL(clicked()), this, SLOT(buttonMovePointClicked()));

    QObject::connect(m_buttonBCurveFrame, SIGNAL(clicked()), this, SLOT(buttonBCurveFrameClicked()));

    QObject::connect(m_buttonStartDragRecord, SIGNAL(clicked()), this, SLOT(buttonStartDragRecordClicked()));
    QObject::connect(m_buttonDragPlayPrepare, SIGNAL(clicked()), this, SLOT(buttonDragPlayPrepareClicked()));
    QObject::connect(m_buttonDragPlay, SIGNAL(clicked()), this, SLOT(buttonDragPlayClicked()));

    QObject::connect(m_radioBCurve, SIGNAL(toggled(bool)), this, SLOT(radioModeTypeChanged()));
    QObject::connect(m_radioDragTeach, SIGNAL(toggled(bool)), this, SLOT(radioModeTypeChanged()));
}

void RobotProgramModeWidget::initBCurveTable(){
    m_tableBCurve->setColumnCount(7);
    m_tableBCurve->horizontalHeader()->setSectionsClickable(false);
    QStringList header;
    header<<tr("速度")<<tr("X")<<tr("Y")<<tr("Z")<<tr("A")<<tr("B")<<tr("C");
    m_tableBCurve->setHorizontalHeaderLabels(header);
    QFont font = m_tableBCurve->horizontalHeader()->font();
    font.setBold(true);
    m_tableBCurve->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableBCurve->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableBCurve->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    m_tableBCurve->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableBCurve->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableBCurve->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableBCurve->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableBCurve->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableBCurve->setSelectionMode(QAbstractItemView::SingleSelection);
    //    m_tableBCurve->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableBCurve->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                        "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                        "QScrollBar::handle:hover{background:gray;}"
                                                        "QScrollBar::sub-line{background:transparent;}"
                                                        "QScrollBar::add-line{background:transparent;}");
    m_tableBCurve->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                      "QScrollBar::handle:hover{background:gray;}"
                                                      "QScrollBar::sub-line{background:transparent;}"
                                                      "QScrollBar::add-line{background:transparent;}");
}

void RobotProgramModeWidget::initDragTeachTable(){
    m_tableDragTeach->setColumnCount(6);
    m_tableDragTeach->horizontalHeader()->setSectionsClickable(false);
    QStringList header;
    header<<tr("J1")<<tr("J2")<<tr("J3")<<tr("J4")<<tr("J5")<<tr("J6");
    m_tableDragTeach->setHorizontalHeaderLabels(header);
    QFont font = m_tableDragTeach->horizontalHeader()->font();
    font.setBold(true);
    m_tableDragTeach->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableDragTeach->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableDragTeach->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    m_tableDragTeach->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableDragTeach->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableDragTeach->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableDragTeach->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableDragTeach->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableDragTeach->setSelectionMode(QAbstractItemView::SingleSelection);
    //    m_tableDragTeach->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableDragTeach->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                           "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                           "QScrollBar::handle:hover{background:gray;}"
                                                           "QScrollBar::sub-line{background:transparent;}"
                                                           "QScrollBar::add-line{background:transparent;}");
    m_tableDragTeach->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                         "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background:gray;}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");
}

void RobotProgramModeWidget::clearState(){
    m_radioBCurve->setChecked(true);
    radioModeTypeChanged();

    groupModeType->setEnabled(true);

    m_tableBCurve->clearContents();
    m_tableBCurve->setRowCount(0);

    m_tableDragTeach->clearContents();
    m_tableDragTeach->setRowCount(0);
}

bool RobotProgramModeWidget::grammarCheck(){
    if(m_radioBCurve->isChecked()){
        if(m_tableBCurve->rowCount() < 1){
            QMessageBox::question(this, tr("提示"),
                                  tr("没有足够的采样点!"),
                                  QMessageBox::Yes);
            return false;
        }

        if(m_editBCurveAcc->text().isEmpty() || m_editBCurveJerk->text().isEmpty() || m_editBCurveAngleVel->text().isEmpty() || m_editBCurvePre->text().isEmpty()){
            QMessageBox::question(this, tr("提示"),
                                  tr("样条曲线参数不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }
    }
    else{
        if(m_tableDragTeach->rowCount() < 1){
            QMessageBox::question(this, tr("提示"),
                                  tr("没有足够的采样点!"),
                                  QMessageBox::Yes);
            return false;
        }

        if(m_editDragTeachPlayPeriod->text().isEmpty()){
            QMessageBox::question(this, tr("提示"),
                                  tr("拖动示教再现周期不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }
    }

    return true;
}

void RobotProgramModeWidget::buttonYesClicked(){
    if(!grammarCheck()){
        return;
    }

    string tempSentence;

    ProgramSentence tmp_Sentence;

    ProgramPointer tmp_Pointer;
    tmp_Pointer.nFunction=m_func;
    tmp_Pointer.nSentence=m_line;


    if(m_radioBCurve->isChecked()){
        tempSentence=tempSentence+string("MOVB")+" "+m_editBCurveAcc->text().toStdString()+" "+
                m_editBCurveJerk->text().toStdString()+" "+m_editBCurveAngleVel->text().toStdString()+
                " "+m_editBCurvePre->text().toStdString()+" "+num_to_string(m_buttonBCurveFrame->returnState()*2)+
                " "+num_to_string(m_tableBCurve->rowCount());

        for(int i=0; i<m_tableBCurve->rowCount(); i++){
            for(int j=0; j<7; j++){
                QTableWidgetItem* item = m_tableBCurve->item(i, j);
                tempSentence=tempSentence+" "+item->text().toStdString();
//                tempSentence.push_back(item->text().toStdString());
            }
        }
    }
    else if(m_radioDragTeach->isChecked()){
        tempSentence=tempSentence+string("MOVJT")+" "+m_editDragTeachPlayPeriod->text().toStdString()+
                " "+num_to_string(m_tableDragTeach->rowCount());

        for(int i=0; i<m_tableDragTeach->rowCount(); i++){
            for(int j=0; j<6; j++){
                QTableWidgetItem* item = m_tableDragTeach->item(i, j);
                tempSentence=tempSentence+" "+item->text().toStdString();
            }
        }
    }

    tmp_Sentence.strSentence=tempSentence;

    if(m_mode == TEACH_MODIFY){
        m_fileSrc->modifyProgramSentence(tmp_Pointer,tmp_Sentence);
    }
    else{
        tmp_Pointer.nSentence=m_line+1;
        m_fileSrc->addProgramSentence(tmp_Pointer,tmp_Sentence);
    }

    emit programOperationFinishedSignal(true);
}

void RobotProgramModeWidget::buttonNoClicked(){
    emit programOperationFinishedSignal(false);
}

void RobotProgramModeWidget::radioModeTypeChanged(){
    if(m_radioBCurve->isChecked()){
        m_tableDragTeach->hide();
        m_groupDragTeachParameter->hide();

        m_tableBCurve->show();
        m_groupBCurveParameter->show();
        m_groupPointOperation->setEnabled(true);
    }
    if(m_radioDragTeach->isChecked()){
        m_tableBCurve->hide();
        m_groupBCurveParameter->hide();
        m_groupPointOperation->setEnabled(false);

        m_tableDragTeach->show();
        m_groupDragTeachParameter->show();
    }
}

void RobotProgramModeWidget::buttonAddPointClicked(){
    if(m_editBCurveVel->text().isEmpty()){
        QMessageBox::question(this, tr("提示"),
                              tr("默认速度值为空!"),
                              QMessageBox::Yes);
        return;
    }
    int row = m_tableBCurve->rowCount();

    m_tableBCurve->insertRow(row);
    for(int j=0; j<1; j++){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(m_editBCurveVel->text());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        m_tableBCurve->setItem(row, j, item);
    }

    RobotMotion rm = RobotClient::initance()->getRobotMotion();
    for(int j=1; j<7; j++){
        QTableWidgetItem* item = new QTableWidgetItem();
        QString text;
        if(m_buttonBCurveFrame->returnState() == 0){
            text = QString("%1").arg((rm.getCurrentTerminal())[static_cast<TERMINALINDEX>(j-1)]);
        }
        else{
            text = QString("%1").arg((rm.getCurrentWorkTerminal())[static_cast<TERMINALINDEX>(j-1)]);
        }
        item->setText(text);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        m_tableBCurve->setItem(row, j, item);
    }

    m_tableBCurve->verticalScrollBar()->setSliderPosition(row);
}


void RobotProgramModeWidget::buttonInsertPointClicked(){
    if(m_editBCurveVel->text().isEmpty()){
        QMessageBox::question(this, tr("提示"),
                              tr("默认速度值为空!"),
                              QMessageBox::Yes);
        return;
    }

    int row = m_tableBCurve->currentRow();
    if(row == -1){
        row = m_tableBCurve->rowCount();
    }

    m_tableBCurve->insertRow(row);
    for(int j=0; j<1; j++){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(m_editBCurveVel->text());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        m_tableBCurve->setItem(row, j, item);

    }

    RobotMotion rm = RobotClient::initance()->getRobotMotion();
    for(int j=0; j<6; j++){
        QTableWidgetItem* item = new QTableWidgetItem();
        QString text;
        if(m_buttonBCurveFrame->returnState() == 0){
            text = QString("%1").arg((rm.getCurrentTerminal())[static_cast<TERMINALINDEX>(j)]);
        }
        else{
            text = QString("%1").arg((rm.getCurrentWorkTerminal())[static_cast<TERMINALINDEX>(j)]);
        }
        item->setText(text);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        m_tableBCurve->setItem(row, j+1, item);
    }

    m_tableBCurve->verticalScrollBar()->setSliderPosition(row);
}

void RobotProgramModeWidget::buttonDeletePointClicked(){
    int row = m_tableBCurve->currentRow();
    if(row == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("请先选中一组点!"),
                              QMessageBox::Yes);
        return;
    }

    m_tableBCurve->removeRow(row);
}

void RobotProgramModeWidget::buttonClearPointClicked(){
    m_tableBCurve->clearContents();
    m_tableBCurve->setRowCount(0);
}

void RobotProgramModeWidget::buttonMovePointClicked(){
    int row = m_tableBCurve->currentRow();
    if(row == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("请先选中一组点!"),
                              QMessageBox::Yes);
        return;
    }

    Terminal terminal;
    for(int i=0; i<6; i++){
        QTableWidgetItem* item = m_tableBCurve->item(row, i+1);
        terminal[static_cast<TERMINALINDEX>(i)] = item->text().toDouble();
    }
    double vel = getGlobalVel();
    if(m_buttonBCurveFrame->returnState() == 0){
        RobotClient::initance()->moveJoint(terminal, vel);
    }
    else{
        RobotClient::initance()->moveJoint(terminal, vel, COORDINATE_WORK);
    }
}

void RobotProgramModeWidget::buttonStartDragRecordClicked(){
    RobotClient::initance()->dragModeTimeStart(m_editDragTeachSamplePeriod->text().toInt(), m_editDragTeachTime->text().toInt());
}

void RobotProgramModeWidget::buttonDragPlayPrepareClicked(){
    RobotClient::initance()->dragModePlayPrepare(getGlobalVel());
}

void RobotProgramModeWidget::buttonDragPlayClicked(){
    RobotClient::initance()->dragModePlay(string_to_int(m_editDragTeachPlayPeriod->text().toStdString()));
}

void RobotProgramModeWidget::updateDragPointTable(){
    list<Joints> jlist = RobotClient::initance()->getDragPoint();

    m_tableDragTeach->clear();
    m_tableDragTeach->setRowCount(0);

    int row = 0;
    for(list<Joints>::iterator it=jlist.begin(); it!=jlist.end(); it++){
        m_tableDragTeach->insertRow(row);
        for(int i=0; i<6; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            QString text = QString("%1").arg((*it)[static_cast<JOINTINDEX>(i)]);
            item->setText(text);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableDragTeach->setItem(row, i, item);
        }
        row++;
    }
}

void RobotProgramModeWidget::buttonBCurveFrameClicked(){
    m_buttonBCurveFrame->incState();

    if(m_buttonBCurveFrame->returnState() == 0){
        m_buttonBCurveFrame->setText(tr("基坐标"));
    }
    else{
        m_buttonBCurveFrame->setText(tr("工件坐标"));
    }
}
