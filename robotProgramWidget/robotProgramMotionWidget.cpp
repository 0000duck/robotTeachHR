#include "robotProgramMotionWidget.h"

#include <QDebug>

using namespace std;
using namespace rclib;

extern bool tag_ADD;

RobotProgramMotionWidget::RobotProgramMotionWidget(QWidget *parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramMotionWidget::~RobotProgramMotionWidget(){

}

void RobotProgramMotionWidget::passParameter(RobotInterpreter* p_file){
    m_fileSrc = p_file;
}

void RobotProgramMotionWidget::setMode(OPERATIONMODE mode){
    m_mode = mode;

    if(m_mode == TEACH_MODIFY){
        vector<std::vector<ProgramSentence>> tmp_FuncTable = m_fileSrc->getFuncTable();
        map<std::string, PROGRAM_KEY_WORD> tmp_VarType = m_fileSrc->getVariableMap();

        VECTOR_STRING tmp_Sentence = Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");

        map<std::string, PROGRAM_KEY_WORD> KeyWordList=m_fileSrc->getKeyWordIndex();
        string temp = tmp_Sentence[0];
        PROGRAM_KEY_WORD tmp_KeyWord = KeyWordList[temp];

        switch(tmp_KeyWord){
        case MOVEABSJ:{
            m_radioMovABSJ->setChecked(true);
            radioMotionTypeChanged();

            m_editVel->setText(QString::fromStdString(tmp_Sentence[1]));

            m_tableWayPoint->clear();
            m_tableWayPoint->setRowCount(0);

            int row = 0;
            for(int i=2; i<tmp_Sentence.size(); i++){
                m_tableWayPoint->insertRow(row);

                string varName = tmp_Sentence[i];
                PROGRAM_KEY_WORD tmp;
                VECTOR_DOUBLE tmp_Vaule;
                m_fileSrc->getVariable(varName,tmp,tmp_Vaule);

                for(int j=0; j<1; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(varName));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableWayPoint->setItem(row, j, item);
                }

                if(!(tmp_VarType.find(varName) == (tmp_VarType.end()))){
                    for(int j=1; j<7; j++){
                        QTableWidgetItem* item = new QTableWidgetItem();
                        item->setText(QString("%1").arg(tmp_Vaule[j-1]));
                        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        m_tableWayPoint->setItem(row, j, item);
                    }
                }
                row++;
            }
        }
            break;
        case MOVEABSJR:{
            m_radioMovABSJR->setChecked(true);
            radioMotionTypeChanged();

            m_editVel->setText(QString::fromStdString(tmp_Sentence[1]));

            m_tableWayPoint->clear();
            m_tableWayPoint->setRowCount(0);

            int row = 0;
            for(int i=2; i<tmp_Sentence.size(); i++){
                m_tableWayPoint->insertRow(row);

                string varName = tmp_Sentence[i];
                PROGRAM_KEY_WORD tmp;
                VECTOR_DOUBLE tmp_Vaule;
                m_fileSrc->getVariable(varName,tmp,tmp_Vaule);

                for(int j=0; j<1; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(varName));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableWayPoint->setItem(row, j, item);
                }

                if(!(tmp_VarType.find(varName) == (tmp_VarType.end()))){
                    for(int j=1; j<7; j++){
                        QTableWidgetItem* item = new QTableWidgetItem();
                        item->setText(QString("%1").arg(tmp_Vaule[j-1]));
                        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        m_tableWayPoint->setItem(row, j, item);
                    }
                }
                row++;
            }
        }
            break;
        case MOVEJ:{
            m_radioMovJ->setChecked(true);
            radioMotionTypeChanged();

            m_editVel->setText(QString::fromStdString(tmp_Sentence[1]));

            m_comboFrame->setCurrentIndex(string_to_int(tmp_Sentence[2]));


            m_tableWayPoint->clear();
            m_tableWayPoint->setRowCount(0);

            int row = 0;
            for(int i=3; i<tmp_Sentence.size(); i++){
                m_tableWayPoint->insertRow(row);

                string varName = tmp_Sentence[i];
                PROGRAM_KEY_WORD tmp;
                VECTOR_DOUBLE tmp_Vaule;
                m_fileSrc->getVariable(varName,tmp,tmp_Vaule);

                for(int j=0; j<1; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(varName));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableWayPoint->setItem(row, j, item);
                }

                if(!(tmp_VarType.find(varName) == (tmp_VarType.end()))){
                    for(int j=1; j<7; j++){
                        QTableWidgetItem* item = new QTableWidgetItem();
                        item->setText(QString("%1").arg(tmp_Vaule[j-1]));
                        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        m_tableWayPoint->setItem(row, j, item);
                    }
                }
                row++;
            }
        }
            break;
        case MOVEJR:{
            m_radioMovJR->setChecked(true);
            radioMotionTypeChanged();

            m_editVel->setText(QString::fromStdString(tmp_Sentence[1]));

            m_comboFrame->setCurrentIndex(string_to_int(tmp_Sentence[2]));

            m_tableWayPoint->clear();
            m_tableWayPoint->setRowCount(0);

            int row = 0;
            for(int i=3; i<tmp_Sentence.size(); i++){
                m_tableWayPoint->insertRow(row);


                string varName = tmp_Sentence[i];
                PROGRAM_KEY_WORD tmp;
                VECTOR_DOUBLE tmp_Vaule;
                m_fileSrc->getVariable(varName,tmp,tmp_Vaule);

                for(int j=0; j<1; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(varName));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableWayPoint->setItem(row, j, item);
                }

                if(!(tmp_VarType.find(varName) == (tmp_VarType.end()))){
                    for(int j=1; j<7; j++){
                        QTableWidgetItem* item = new QTableWidgetItem();
                        item->setText(QString("%1").arg(tmp_Vaule[j-1]));
                        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        m_tableWayPoint->setItem(row, j, item);
                    }
                }
                row++;
            }
        }
            break;
        case MOVEL:{
            m_radioMovL->setChecked(true);
            radioMotionTypeChanged();

            m_editVel->setText(QString::fromStdString(tmp_Sentence[1]));
            m_editAcc->setText(QString::fromStdString(tmp_Sentence[2]));
            m_editJerk->setText(QString::fromStdString(tmp_Sentence[3]));
            m_editRadius->setText(QString::fromStdString(tmp_Sentence[4]));

            m_comboFrame->setCurrentIndex(string_to_int(tmp_Sentence[5]));

            m_tableWayPoint->clear();
            m_tableWayPoint->setRowCount(0);

            int row = 0;
            for(int i=6; i<tmp_Sentence.size(); i++){
                m_tableWayPoint->insertRow(row);

                string varName = tmp_Sentence[i];
                PROGRAM_KEY_WORD tmp;
                VECTOR_DOUBLE tmp_Vaule;
                m_fileSrc->getVariable(varName,tmp,tmp_Vaule);

                for(int j=0; j<1; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(varName));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableWayPoint->setItem(row, j, item);
                }

                if(!(tmp_VarType.find(varName) == (tmp_VarType.end()))){
                    for(int j=1; j<7; j++){
                        QTableWidgetItem* item = new QTableWidgetItem();
                        item->setText(QString("%1").arg(tmp_Vaule[j-1]));
                        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        m_tableWayPoint->setItem(row, j, item);
                    }
                }
                row++;
            }
        }
            break;
        case MOVELR:{
            m_radioMovLR->setChecked(true);
            radioMotionTypeChanged();

            m_editVel->setText(QString::fromStdString(tmp_Sentence[1]));
            m_editAcc->setText(QString::fromStdString(tmp_Sentence[2]));
            m_editJerk->setText(QString::fromStdString(tmp_Sentence[3]));
            m_editRadius->setText(QString::fromStdString(tmp_Sentence[4]));

            m_comboFrame->setCurrentIndex(string_to_int(tmp_Sentence[5]));

            m_tableWayPoint->clear();
            m_tableWayPoint->setRowCount(0);

            int row = 0;
            for(int i=6; i<tmp_Sentence.size(); i++){
                m_tableWayPoint->insertRow(row);

                string varName = tmp_Sentence[i];
                PROGRAM_KEY_WORD tmp;
                VECTOR_DOUBLE tmp_Vaule;
                m_fileSrc->getVariable(varName,tmp,tmp_Vaule);

                for(int j=0; j<1; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(varName));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableWayPoint->setItem(row, j, item);
                }

                if(!(tmp_VarType.find(varName) == (tmp_VarType.end()))){
                    for(int j=1; j<7; j++){
                        QTableWidgetItem* item = new QTableWidgetItem();
                        item->setText(QString("%1").arg(tmp_Vaule[j-1]));
                        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        m_tableWayPoint->setItem(row, j, item);
                    }
                }
                row++;
            }
        }
            break;
        case MOVEC:{
            m_radioMovC->setChecked(true);
            radioMotionTypeChanged();

            m_comboCircleType->setCurrentIndex(string_to_int(tmp_Sentence[1]));

            m_editVel->setText(QString::fromStdString(tmp_Sentence[2]));
            m_editAcc->setText(QString::fromStdString(tmp_Sentence[3]));

            m_editJerk->setText(QString::fromStdString(tmp_Sentence[4]));
            m_editRadius->setText(QString::fromStdString(tmp_Sentence[5]));

            m_comboFrame->setCurrentIndex(string_to_int(tmp_Sentence[6]));

            m_tableWayPoint->clear();
            m_tableWayPoint->setRowCount(0);

            int row = 0;
            for(int i=7; i<tmp_Sentence.size(); i++){
                m_tableWayPoint->insertRow(row);

                string varName = tmp_Sentence[i];
                PROGRAM_KEY_WORD tmp;
                VECTOR_DOUBLE tmp_Vaule;
                m_fileSrc->getVariable(varName,tmp,tmp_Vaule);

                for(int j=0; j<1; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(varName));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableWayPoint->setItem(row, j, item);
                }

                if(!(tmp_VarType.find(varName) == (tmp_VarType.end()))){
                    for(int j=1; j<7; j++){
                        QTableWidgetItem* item = new QTableWidgetItem();
                        item->setText(QString("%1").arg(tmp_Vaule[j-1]));
                        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        m_tableWayPoint->setItem(row, j, item);
                    }
                }
                row++;
            }
        }
            break;
        case MOVECR:{
            m_radioMovCR->setChecked(true);
            radioMotionTypeChanged();

            m_comboCircleType->setCurrentIndex(string_to_int(tmp_Sentence[1]));

            m_editVel->setText(QString::fromStdString(tmp_Sentence[2]));
            m_editAcc->setText(QString::fromStdString(tmp_Sentence[3]));

            m_editJerk->setText(QString::fromStdString(tmp_Sentence[4]));
            m_editRadius->setText(QString::fromStdString(tmp_Sentence[5]));

            m_comboFrame->setCurrentIndex(string_to_int(tmp_Sentence[6]));

            m_tableWayPoint->clear();
            m_tableWayPoint->setRowCount(0);

            int row = 0;
            for(int i=7; i<tmp_Sentence.size(); i++){
                m_tableWayPoint->insertRow(row);

                string varName = tmp_Sentence[i];
                PROGRAM_KEY_WORD tmp;
                VECTOR_DOUBLE tmp_Vaule;
                m_fileSrc->getVariable(varName,tmp,tmp_Vaule);

                for(int j=0; j<1; j++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    item->setText(QString::fromStdString(varName));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    m_tableWayPoint->setItem(row, j, item);
                }

                if(!(tmp_VarType.find(varName) == (tmp_VarType.end()))){
                    for(int j=1; j<7; j++){
                        QTableWidgetItem* item = new QTableWidgetItem();
                        item->setText(QString("%1").arg(tmp_Vaule[j-1]));
                        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        m_tableWayPoint->setItem(row, j, item);
                    }
                }
                row++;
            }
        }
            break;
        default:
            break;
        }

        groupWayPoint->hide();
        groupMOV->show();
        groupMOV->setEnabled(false);
        groupParameter->show();
        m_buttonPrevious->setEnable(false);
        m_buttonNext->setEnable(true);
        m_buttonYes->setEnable(false);
        m_buttonNo->setEnable(true);
    }
    else{
        clearState();
    }
}

void RobotProgramMotionWidget::passSentenceIndex(int func, int line){
    m_func = func;
    m_line = line;
}

void RobotProgramMotionWidget::initUI(){
    m_radioMovABSJ = new QRadioButton("MOVABSJ\nABS关节");
    m_radioMovABSJR = new QRadioButton("MOVABSJR\n相对ABS关节");
    m_radioMovJ = new QRadioButton("MOVJ\n关节");
    m_radioMovJR = new QRadioButton("MOVJR\n相对关节");
    m_radioMovL = new QRadioButton("MOVL\n直线");
    m_radioMovLR = new QRadioButton("MOVLR\n相对直线");
    m_radioMovC = new QRadioButton("MOVC\n圆弧");
    m_radioMovCR = new QRadioButton("MOVCR\n相对圆弧");

    QVBoxLayout* layoutMOVABSJ = new QVBoxLayout();
    layoutMOVABSJ->addWidget(m_radioMovABSJ);
    layoutMOVABSJ->addWidget(m_radioMovABSJR);
    QVBoxLayout* layoutMOVJ = new QVBoxLayout();
    layoutMOVJ->addWidget(m_radioMovJ);
    layoutMOVJ->addWidget(m_radioMovJR);
    QVBoxLayout* layoutMOVL = new QVBoxLayout();
    layoutMOVL->addWidget(m_radioMovL);
    layoutMOVL->addWidget(m_radioMovLR);
    QVBoxLayout* layoutMOVC = new QVBoxLayout();
    layoutMOVC->addWidget(m_radioMovC);
    layoutMOVC->addWidget(m_radioMovCR);
    QHBoxLayout* layoutMOV = new QHBoxLayout();
    layoutMOV->addLayout(layoutMOVABSJ);
    layoutMOV->addLayout(layoutMOVJ);
    layoutMOV->addLayout(layoutMOVL);
    layoutMOV->addLayout(layoutMOVC);
    groupMOV = new QGroupBox();
    groupMOV->setLayout(layoutMOV);

    m_labelCircleType = new QLabel(tr("圆类型"));
    m_labelVel = new QLabel(tr("速度\n0 -- 1"));
    m_labelAcc = new QLabel(tr("加速度\n0 -- 1"));
    m_labelJerk = new QLabel(tr("冲击\n0 -- 1"));
    m_labelRadius = new QLabel(tr("转弯区\n0 -- 1"));
    m_labelFrame = new QLabel(tr("坐标系"));

    m_comboCircleType = new ComboBox();
    m_comboFrame = new ComboBox();
    m_comboCircleType->setFixedSize(0.075*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboFrame->setFixedSize(0.075*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboCircleType->insertItem(0,"部分圆");
    m_comboCircleType->insertItem(1,"整圆");
    m_comboFrame->insertItem(0,"基坐标");
    m_comboFrame->insertItem(1,"工件坐标");
    m_comboFrame->insertItem(2,"工件坐标");

    m_editVel = new QLineEdit();
    m_editAcc = new QLineEdit();
    m_editJerk = new QLineEdit();
    m_editRadius = new QLineEdit();

    m_editVel->setFixedSize(0.075*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editAcc->setFixedSize(0.075*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editJerk->setFixedSize(0.075*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_editRadius->setFixedSize(0.075*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutCircleType = new QHBoxLayout();
    layoutCircleType->addWidget(m_labelCircleType);
    layoutCircleType->addWidget(m_comboCircleType);
    QHBoxLayout* layoutVel = new QHBoxLayout();
    layoutVel->addWidget(m_labelVel);
    layoutVel->addWidget(m_editVel);
    QHBoxLayout* layoutAcc = new QHBoxLayout();
    layoutAcc->addWidget(m_labelAcc);
    layoutAcc->addWidget(m_editAcc);
    QHBoxLayout* layoutJerk = new QHBoxLayout();
    layoutJerk->addWidget(m_labelJerk);
    layoutJerk->addWidget(m_editJerk);
    QHBoxLayout* layoutRadius = new QHBoxLayout();
    layoutRadius->addWidget(m_labelRadius);
    layoutRadius->addWidget(m_editRadius);
    QHBoxLayout* layoutFrame = new QHBoxLayout();
    layoutFrame->addWidget(m_labelFrame);
    layoutFrame->addWidget(m_comboFrame);

    QVBoxLayout* layoutParameter1 = new QVBoxLayout();
    layoutParameter1->addLayout(layoutVel);
    layoutParameter1->addLayout(layoutJerk);
    QVBoxLayout* layoutParameter2 = new QVBoxLayout();
    layoutParameter2->addLayout(layoutAcc);
    layoutParameter2->addLayout(layoutRadius);
    QVBoxLayout* layoutParameter3 = new QVBoxLayout();
    layoutParameter3->addLayout(layoutCircleType);
    layoutParameter3->addLayout(layoutFrame);

    QHBoxLayout* layoutParameter = new QHBoxLayout();
    layoutParameter->addLayout(layoutParameter1);
    layoutParameter->addLayout(layoutParameter2);
    layoutParameter->addLayout(layoutParameter3);
    layoutParameter->setStretchFactor(layoutParameter1, 1);
    layoutParameter->setStretchFactor(layoutParameter2, 1);
    layoutParameter->setStretchFactor(layoutParameter3, 1);
    groupParameter = new QGroupBox();
    groupParameter->setLayout(layoutParameter);

    m_buttonPrevious = new PushButton(tr("上一步"));
    m_buttonNext = new PushButton(tr("下一步"));
    m_buttonPrevious->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNext->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutStep = new QVBoxLayout();
    layoutStep->addWidget(m_buttonPrevious);
    layoutStep->addWidget(m_buttonNext);
    QGroupBox* groupStep = new QGroupBox();
    groupStep->setLayout(layoutStep);

    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    m_buttonYes->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutSure = new QVBoxLayout();
    layoutSure->addWidget(m_buttonYes);
    layoutSure->addWidget(m_buttonNo);
    QGroupBox* groupSure = new QGroupBox();
    groupSure->setLayout(layoutSure);

    QHBoxLayout* layoutStepSure = new QHBoxLayout();
    layoutStepSure->addWidget(groupStep, 0, Qt::AlignLeft);
    layoutStepSure->addWidget(groupSure, 0, Qt::AlignRight);
    layoutStepSure->setStretchFactor(groupStep, 1);
    layoutStepSure->setStretchFactor(groupSure, 1);

    m_tableWayPoint = new QTableWidget();

    m_buttonPointSource = new PushButton(tr("新的路点"));
    m_buttonAddPoint = new PushButton(tr("添加\n路点"));
    m_buttonInsertPoint = new PushButton(tr("插入\n路点"));
    m_buttonDeletePoint = new PushButton(tr("删除\n路点"));
    m_buttonInsertCurrentPoint = new PushButton(tr("插入当前点"));
    m_buttonPointSourceFrame = new PushButton(tr("基坐标"));

    m_buttonPointSource->setMinimumSize(0.07*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonAddPoint->setMinimumSize(0.04*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonInsertPoint->setMinimumSize(0.04*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonDeletePoint->setMinimumSize(0.04*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonInsertCurrentPoint->setMinimumSize(0.07*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonPointSourceFrame->setMinimumSize(0.07*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutWayPointOperation1 = new QHBoxLayout();
    layoutWayPointOperation1->addWidget(m_buttonPointSource);
    QGroupBox* groupWayPointOperation1 = new QGroupBox();
    groupWayPointOperation1->setLayout(layoutWayPointOperation1);

    QHBoxLayout* layoutWayPointOperation2 = new QHBoxLayout();
    layoutWayPointOperation2->addWidget(m_buttonAddPoint);
    layoutWayPointOperation2->addWidget(m_buttonInsertPoint);
    layoutWayPointOperation2->addWidget(m_buttonDeletePoint);
    QGroupBox* groupWayPointOperation2 = new QGroupBox();
    groupWayPointOperation2->setLayout(layoutWayPointOperation2);

    QHBoxLayout* layoutWayPointOperation3 = new QHBoxLayout();
    layoutWayPointOperation3->addWidget(m_buttonInsertCurrentPoint);
    layoutWayPointOperation3->addWidget(m_buttonPointSourceFrame);
    QGroupBox* groupWayPointOperation3 = new QGroupBox();
    groupWayPointOperation3->setLayout(layoutWayPointOperation3);

    QHBoxLayout* layoutWayPointOperation = new QHBoxLayout();
    layoutWayPointOperation->addWidget(groupWayPointOperation1);
    layoutWayPointOperation->addWidget(groupWayPointOperation3);
    layoutWayPointOperation->addWidget(groupWayPointOperation2);

    m_editPointName = new QLineEdit();
    m_editPointName->setMinimumSize(0.06*WINDOWWIDTH, 0.03*WINDOWWIDTH);
    m_comboPointName = new QComboBox();
    m_comboPointName->setStyleSheet("QComboBox QAbstractItemView{"
                                           "font-size: 18px;"
                                       "}");
    m_comboPointName->setView(new  QListView());
    m_comboPointName->setMinimumSize(0.06*WINDOWWIDTH, 0.03*WINDOWWIDTH);
    m_comboPointName->setMaxVisibleItems(7);
    QHBoxLayout* layoutWayPointEditName = new QHBoxLayout();
    layoutWayPointEditName->addWidget(m_editPointName);
    layoutWayPointEditName->addWidget(m_comboPointName);
    QGroupBox* groupWayPointEditName = new QGroupBox(tr("路点名称"));
    groupWayPointEditName->setLayout(layoutWayPointEditName);

    QHBoxLayout* layoutWayPointEditValue = new QHBoxLayout();
    for(int i=0; i<6; i++){
        m_editPointValue[i] = new QLineEdit();
        m_editPointValue[i]->setMinimumSize(0.05*WINDOWWIDTH, 0.03*WINDOWWIDTH);
        layoutWayPointEditValue->addWidget(m_editPointValue[i]);
    }
    QGroupBox* groupWayPointEditValue = new QGroupBox(tr("路点取值"));
    groupWayPointEditValue->setLayout(layoutWayPointEditValue);

    QHBoxLayout* layoutWayPointEdit = new QHBoxLayout();
    layoutWayPointEdit->addWidget(groupWayPointEditName);
    layoutWayPointEdit->addWidget(groupWayPointEditValue);

    QVBoxLayout* layoutWayPoint = new QVBoxLayout();
    layoutWayPoint->addLayout(layoutWayPointOperation);
    layoutWayPoint->addLayout(layoutWayPointEdit);
    layoutWayPoint->addWidget(m_tableWayPoint);
    layoutWayPoint->setStretchFactor(layoutWayPointOperation, 1);
    layoutWayPoint->setStretchFactor(layoutWayPointEdit, 1);
    layoutWayPoint->setStretchFactor(m_tableWayPoint, 3);
    groupWayPoint = new QGroupBox();
    groupWayPoint->setLayout(layoutWayPoint);

    QVBoxLayout* layoutMotion = new QVBoxLayout(this);
    layoutMotion->addWidget(groupMOV);
    layoutMotion->addWidget(groupParameter);
    layoutMotion->addWidget(groupWayPoint);
    layoutMotion->addLayout(layoutStepSure);
    layoutMotion->setStretchFactor(groupMOV, 2);
    layoutMotion->setStretchFactor(groupParameter, 4);
    layoutMotion->setStretchFactor(groupWayPoint, 6);
    layoutMotion->setStretchFactor(layoutStepSure, 2);

    m_comboCircleType->setCurrentIndex(0);
    m_comboFrame->setCurrentIndex(0);

    m_comboPointName->hide();
}

void RobotProgramMotionWidget::initState(){
    initWayPointTable();

    m_editVel->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editAcc->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editJerk->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));
    m_editRadius->setValidator(new QDoubleValidator(0, 1000, NUMPRECISION, this));

    for(int i=0; i<6; i++){
        m_editPointValue[i]->setValidator(new QDoubleValidator(-1000, 1000, NUMPRECISION, this));
    }

    m_buttonPointSource->setStateSum(2);
    m_buttonPointSourceFrame->setStateSum(2);

    m_editVel->setText("100");
    m_editAcc->setText("0.8");
    m_editJerk->setText("0.8");
    m_editRadius->setText("0.2");

    clearState();
}

void RobotProgramMotionWidget::initWayPointTable(){
    m_tableWayPoint->setColumnCount(7);
    m_tableWayPoint->horizontalHeader()->setSectionsClickable(false);
    QFont font = m_tableWayPoint->horizontalHeader()->font();
    font.setBold(true);
    m_tableWayPoint->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableWayPoint->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableWayPoint->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    m_tableWayPoint->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableWayPoint->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableWayPoint->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableWayPoint->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableWayPoint->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableWayPoint->setSelectionMode(QAbstractItemView::SingleSelection);
    //    m_tableWayPoint->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableWayPoint->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                          "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                          "QScrollBar::handle:hover{background:gray;}"
                                                          "QScrollBar::sub-line{background:transparent;}"
                                                          "QScrollBar::add-line{background:transparent;}");
    m_tableWayPoint->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                        "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                        "QScrollBar::handle:hover{background:gray;}"
                                                        "QScrollBar::sub-line{background:transparent;}"
                                                        "QScrollBar::add-line{background:transparent;}");
}

void RobotProgramMotionWidget::connectSignalSlot(){
    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));

    QObject::connect(m_buttonPrevious, SIGNAL(clicked()), this, SLOT(buttonPreviousClicked()));
    QObject::connect(m_buttonNext, SIGNAL(clicked()), this, SLOT(buttonNextClicked()));

    QObject::connect(m_buttonAddPoint, SIGNAL(clicked()), this, SLOT(buttonAddPointClicked()));
    QObject::connect(m_buttonInsertPoint, SIGNAL(clicked()), this, SLOT(buttonInsertPointClicked()));
    QObject::connect(m_buttonInsertCurrentPoint, SIGNAL(clicked()), this, SLOT(buttonInsertCurrentPointClicked()));
    QObject::connect(m_buttonDeletePoint, SIGNAL(clicked()), this, SLOT(buttonDeletePointClicked()));

    QObject::connect(m_buttonPointSourceFrame, SIGNAL(clicked()), this, SLOT(buttonPointSourceFrameClicked()));
    QObject::connect(m_buttonPointSource, SIGNAL(clicked()), this, SLOT(buttonPointSourceClicked()));

    QObject::connect(m_radioMovABSJ, SIGNAL(toggled(bool)), this, SLOT(radioMotionTypeChanged()));
    QObject::connect(m_radioMovABSJR, SIGNAL(toggled(bool)), this, SLOT(radioMotionTypeChanged()));
    QObject::connect(m_radioMovJ, SIGNAL(toggled(bool)), this, SLOT(radioMotionTypeChanged()));
    QObject::connect(m_radioMovJR, SIGNAL(toggled(bool)), this, SLOT(radioMotionTypeChanged()));
    QObject::connect(m_radioMovL, SIGNAL(toggled(bool)), this, SLOT(radioMotionTypeChanged()));
    QObject::connect(m_radioMovLR, SIGNAL(toggled(bool)), this, SLOT(radioMotionTypeChanged()));
    QObject::connect(m_radioMovC, SIGNAL(toggled(bool)), this, SLOT(radioMotionTypeChanged()));
    QObject::connect(m_radioMovCR, SIGNAL(toggled(bool)), this, SLOT(radioMotionTypeChanged()));
}

void RobotProgramMotionWidget::clearState(){
    groupWayPoint->hide();
    groupMOV->show();
    groupMOV->setEnabled(true);
    groupParameter->show();
    m_buttonPrevious->setEnable(false);
    m_buttonNext->setEnable(true);
    m_buttonYes->setEnable(false);
    m_buttonNo->setEnable(true);

    m_radioMovABSJ->setChecked(true);
    radioMotionTypeChanged();

    m_buttonPointSource->setState(0);
    m_buttonPointSource->decState();
    buttonPointSourceClicked();
}

bool RobotProgramMotionWidget::grammarCheck(){
    if(m_radioMovABSJ->isChecked() || m_radioMovABSJR->isChecked() || m_radioMovJ->isChecked() || m_radioMovJR->isChecked()){
        if(m_editVel->text().isEmpty()){
            QMessageBox::question(this, tr("提示"),
                                  tr("参数不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }

        if(m_tableWayPoint->rowCount() < 1){
            QMessageBox::question(this, tr("提示"),
                                  tr("最少需要一组路点!"),
                                  QMessageBox::Yes);
            return false;
        }
    }
    else if(m_radioMovL->isChecked() || m_radioMovLR->isChecked()){
        if(m_editVel->text().isEmpty() || m_editAcc->text().isEmpty() || m_editJerk->text().isEmpty() || m_editRadius->text().isEmpty()){
            QMessageBox::question(this, tr("提示"),
                                  tr("参数不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }

        if(m_tableWayPoint->rowCount() < 1){
            QMessageBox::question(this, tr("提示"),
                                  tr("直线需要一组路点!"),
                                  QMessageBox::Yes);
            return false;
        }
    }
    else if(m_radioMovC->isChecked() || m_radioMovCR->isChecked()){
        if(m_editVel->text().isEmpty() || m_editAcc->text().isEmpty() || m_editJerk->text().isEmpty() || m_editRadius->text().isEmpty()){
            QMessageBox::question(this, tr("提示"),
                                  tr("参数不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }

        if(m_tableWayPoint->rowCount() < 2){
            QMessageBox::question(this, tr("提示"),
                                  tr("圆弧需要两组路点!"),
                                  QMessageBox::Yes);
            return false;
        }
    }

    return true;
}

void RobotProgramMotionWidget::buttonYesClicked(){
    if(!grammarCheck()){
        return;
    }

    string tempSentence;
    \
    ProgramSentence m_Sentence;

    ProgramPointer tmp_Pointer;
    tmp_Pointer.nFunction=m_func;
    tmp_Pointer.nSentence=m_line;

    if(m_radioMovABSJ->isChecked()){
        tempSentence=tempSentence+string("MOVABSJ")+" "+m_editVel->text().toStdString();
    }
    else if(m_radioMovABSJR->isChecked()){
        tempSentence=tempSentence+string("MOVABSJR")+" "+m_editVel->text().toStdString();
    }
    else if(m_radioMovJ->isChecked()){
        tempSentence=tempSentence+string("MOVJ")+" "+m_editVel->text().toStdString()+" "+num_to_string(m_comboFrame->currentIndex());
    }
    else if(m_radioMovJR->isChecked()){
        tempSentence=tempSentence+string("MOVJR")+" "+m_editVel->text().toStdString()+" "+num_to_string(m_comboFrame->currentIndex());
    }
    else if(m_radioMovL->isChecked()){
        tempSentence=tempSentence+string("MOVL")+" "+m_editVel->text().toStdString()+
                " "+m_editAcc->text().toStdString()+" "+m_editJerk->text().toStdString()+
                " "+m_editRadius->text().toStdString()+" "+num_to_string(m_comboFrame->currentIndex());
    }
    else if(m_radioMovLR->isChecked()){
        tempSentence=tempSentence+string("MOVLR")+" "+m_editVel->text().toStdString()+
                " "+m_editAcc->text().toStdString()+" "+m_editJerk->text().toStdString()+
                " "+m_editRadius->text().toStdString()+" "+num_to_string(m_comboFrame->currentIndex());
    }
    else if(m_radioMovC->isChecked()){
        tempSentence=tempSentence+string("MOVC")+" "+num_to_string(m_comboCircleType->currentIndex())+" "+m_editVel->text().toStdString()
                +" "+m_editAcc->text().toStdString()+" "+m_editJerk->text().toStdString()+
                " "+m_editRadius->text().toStdString()+" "+num_to_string(m_comboFrame->currentIndex());
    }
    else if(m_radioMovCR->isChecked()){
        tempSentence=tempSentence+string("MOVCR")+" "+num_to_string(m_comboCircleType->currentIndex())+" "+m_editVel->text().toStdString()
                +" "+m_editAcc->text().toStdString()+" "+m_editJerk->text().toStdString()+
                " "+m_editRadius->text().toStdString()+" "+num_to_string(m_comboFrame->currentIndex());
    }

    for(int i=0; i<m_tableWayPoint->rowCount(); i++){
        string name=m_tableWayPoint->item(i, 0)->text().toStdString();
        tempSentence=tempSentence+" "+name;

        string value;
        for(int j=1; j<7; j++){
            QTableWidgetItem* item = m_tableWayPoint->item(i, j);
            value=value+" "+item->text().toStdString();
        }

        string tmp_Sentence;
        if(m_radioMovABSJ->isChecked() || m_radioMovABSJR->isChecked()){
            tmp_Sentence =string("JOINT");
        }
        else{
            tmp_Sentence =string("TERMINAL");
        }
        tmp_Sentence=tmp_Sentence+" "+name+" "+"="+value;

        m_fileSrc->addVariable(tmp_Sentence);
    }

    m_Sentence.strSentence=tempSentence;

    if(m_mode == TEACH_MODIFY){
        m_fileSrc->modifyProgramSentence(tmp_Pointer,m_Sentence);
        tag_ADD = false;
    }
    else{
        tmp_Pointer.nSentence=m_line+1;
        m_fileSrc->addProgramSentence(tmp_Pointer,m_Sentence);
        tag_ADD = true;
    }

    groupMOV->setEnabled(false);
    emit programOperationFinishedSignal(true);
}

void RobotProgramMotionWidget::buttonNoClicked(){
    groupMOV->setEnabled(false);
    emit programOperationFinishedSignal(false);
}

void RobotProgramMotionWidget::buttonPreviousClicked(){
    groupWayPoint->hide();
    groupMOV->show();
    groupParameter->show();
    m_buttonPrevious->setEnable(false);
    m_buttonNext->setEnable(true);
    m_buttonYes->setEnable(false);
    m_buttonNo->setEnable(true);
}

void RobotProgramMotionWidget::buttonNextClicked(){
    groupMOV->hide();
    groupParameter->hide();
    groupWayPoint->show();
    m_buttonPrevious->setEnable(true);
    m_buttonNext->setEnable(false);
    m_buttonYes->setEnable(true);
    m_buttonNo->setEnable(true);
}

void RobotProgramMotionWidget::buttonAddPointClicked(){
    int row = m_tableWayPoint->rowCount();
    if(m_radioMovL->isChecked() || m_radioMovLR->isChecked()){
        if(row >= 1){
            QMessageBox::question(this, tr("提示"),
                                  tr("直线仅需一个目标点!"),
                                  QMessageBox::Yes);
            return;
        }
    }
    if(m_radioMovC->isChecked() || m_radioMovCR->isChecked()){
        if(row >= 2){
            QMessageBox::question(this, tr("提示"),
                                  tr("圆弧仅需两个目标点!"),
                                  QMessageBox::Yes);
            return;
        }
    }

    insertWayPoint(row);
}

void RobotProgramMotionWidget::buttonInsertPointClicked(){
    int row = m_tableWayPoint->rowCount();
    if(m_radioMovL->isChecked() || m_radioMovLR->isChecked()){
        if(row >= 1){
            QMessageBox::question(this, tr("提示"),
                                  tr("直线仅需一个目标点!"),
                                  QMessageBox::Yes);
            return;
        }
    }
    if(m_radioMovC->isChecked() || m_radioMovCR->isChecked()){
        if(row >= 2){
            QMessageBox::question(this, tr("提示"),
                                  tr("圆弧仅需两个目标点!"),
                                  QMessageBox::Yes);
            return;
        }
    }

    row = m_tableWayPoint->currentRow();
    if(row == -1){
        row = m_tableWayPoint->rowCount();
    }

    insertWayPoint(row);
}

void RobotProgramMotionWidget::insertWayPoint(int row){
    map<std::string, PROGRAM_KEY_WORD> tmp_VarType=m_fileSrc->getVariableMap();
    string tmp = getCurrentText();
    if(m_buttonPointSource->returnState() == 0){
//        if(!(tmp_VarType.find(m_editPointName->text().toStdString()) == tmp_VarType.end())){
        if(!(tmp_VarType.find(tmp) == tmp_VarType.end())){
            QMessageBox::question(this, tr("提示"),
                                  tr("该变量名已存在!"),
                                  QMessageBox::Yes);
            return;
        }

        for(int i=0; i<6; i++){
            if(m_editPointValue[i]->text().isEmpty()){
                QMessageBox::question(this, tr("提示"),
                                      tr("路点取值不能为空!"),
                                      QMessageBox::Yes);
                return;
            }
        }

        QString name;
//        if(m_editPointName->text().isEmpty()){
         if(tmp == ""){
            if(m_radioMovABSJ->isChecked() || m_radioMovABSJR->isChecked()){
                name = randomWayPointName(0);
            }
            else{
                name = randomWayPointName(1);
            }
        }
        else{
//            name = m_editPointName->text();
             name = QString::fromStdString(tmp);
        }

        m_tableWayPoint->insertRow(row);

        for(int j=0; j<1; j++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(name);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableWayPoint->setItem(row, j, item);
        }
        for(int j=1; j<7; j++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(m_editPointValue[j-1]->text());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableWayPoint->setItem(row, j, item);
        }
    }
    else{
        if(tmp_VarType.find(/*m_editPointName->text().toStdString()*/tmp) == tmp_VarType.end()){
            QMessageBox::question(this, tr("提示"),
                                  tr("该变量名不存在!"),
                                  QMessageBox::Yes);
            return;
        }

        if(m_radioMovABSJ->isChecked() || m_radioMovABSJR->isChecked()){
            if(tmp_VarType[/*m_editPointName->text().toStdString()*/tmp] != JOINT){
                QMessageBox::question(this, tr("提示"),
                                      tr("该变量类型与待添加的类型不一致!"),
                                      QMessageBox::Yes);
                return;
            }
        }
        else{
            if(tmp_VarType[/*m_editPointName->text().toStdString()*/tmp] != TERMINAL){
                QMessageBox::question(this, tr("提示"),
                                      tr("该变量类型与待添加的类型不一致!"),
                                      QMessageBox::Yes);
                return;
            }
        }

        m_tableWayPoint->insertRow(row);

        for(int j=0; j<1; j++){
            QTableWidgetItem* item = new QTableWidgetItem();
//            item->setText(m_editPointName->text());
            item->setText(QString::fromStdString(tmp));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableWayPoint->setItem(row, j, item);
        }

//        string name = m_editPointName->text().toStdString();
        string name = tmp;
        PROGRAM_KEY_WORD tmp_KeyWord;
        VECTOR_DOUBLE tmp_VarValue;

        m_fileSrc->getVariable(name,tmp_KeyWord,tmp_VarValue);

        for(int j=1; j<7; j++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString("%1").arg(tmp_VarValue[j-1]));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            m_tableWayPoint->setItem(row, j, item);
        }
    }

    m_tableWayPoint->verticalScrollBar()->setSliderPosition(row);
}

void RobotProgramMotionWidget::buttonInsertCurrentPointClicked(){
    RobotMotion rm = RobotClient::initance()->getRobotMotion();
    if(m_radioMovABSJ->isChecked() || m_radioMovABSJR->isChecked()){
        for(int i=0; i<6; i++){
            m_editPointValue[i]->setText(QString("%1").arg((rm.getCurrentJointPosition())[static_cast<JOINTINDEX>(i)]));
        }
    }
    else{
        if(m_buttonPointSource->returnState() == 0){
            for(int i=0; i<6; i++){
                m_editPointValue[i]->setText(QString("%1").arg((rm.getCurrentTerminal())[static_cast<TERMINALINDEX>(i)]));
            }
        }
        else{
            for(int i=0; i<6; i++){
                m_editPointValue[i]->setText(QString("%1").arg((rm.getCurrentWorkTerminal())[static_cast<TERMINALINDEX>(i)]));
            }
        }
    }
}

void RobotProgramMotionWidget::buttonDeletePointClicked(){
    int row = m_tableWayPoint->currentRow();
    if(row == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("请先选中一组点!"),
                              QMessageBox::Yes);
        return;
    }

    m_tableWayPoint->removeRow(row);
}

void RobotProgramMotionWidget::buttonPointSourceFrameClicked(){
    m_buttonPointSourceFrame->incState();
    switch (m_buttonPointSourceFrame->returnState()) {
    case 0:
        m_buttonPointSourceFrame->setText(tr("基坐标"));
        break;
    case 1:
        m_buttonPointSourceFrame->setText(tr("工件坐标"));
        break;
    default:
        break;
    }
}

void RobotProgramMotionWidget::buttonPointSourceClicked(){
    m_buttonPointSource->incState();
    switch (m_buttonPointSource->returnState()) {
    case 0:
        m_buttonPointSource->setText(tr("新的路点"));
        m_editPointName->show();
        m_comboPointName->hide();
        break;
    case 1:
        m_buttonPointSource->setText(tr("已有路点"));
        m_editPointName->hide();
        updateComboList();
        m_comboPointName->show();
        break;
    default:
        break;
    }

    switch(m_buttonPointSource->returnState()){
    case 0:{
        m_buttonInsertCurrentPoint->setEnable(true);
        if(m_radioMovABSJ->isChecked() || m_radioMovABSJR->isChecked()){
            m_buttonPointSourceFrame->setEnable(false);
        }
        else{
            m_buttonPointSourceFrame->setEnable(true);
        }
        for(int i=0; i<6; i++){
            m_editPointValue[i]->setEnabled(true);
        }
    }
        break;
    case 1:{
        m_buttonInsertCurrentPoint->setEnable(false);
        m_buttonPointSourceFrame->setEnable(false);
        for(int i=0; i<6; i++){
            m_editPointValue[i]->setEnabled(false);
        }
    }
        break;
    default:
        break;
    }
}

void RobotProgramMotionWidget::radioMotionTypeChanged(){
    m_buttonPointSource->setState(0);
    m_buttonPointSource->decState();
    buttonPointSourceClicked();

    if(m_radioMovABSJ->isChecked()){
        m_labelVel->setText(tr("速度\n0 -- 1"));
        QStringList header;
        header<<tr("名称")<<tr("J1")<<tr("J2")<<tr("J3")<<tr("J4")<<tr("J5")<<tr("J6");
        m_tableWayPoint->setHorizontalHeaderLabels(header);

        m_labelCircleType->hide();
        m_labelVel->show();
        m_labelAcc->hide();
        m_labelJerk->hide();
        m_labelRadius->hide();
        m_labelFrame->hide();
        m_comboCircleType->hide();
        m_editVel->show();
        m_editAcc->hide();
        m_editJerk->hide();
        m_editRadius->hide();
        m_comboFrame->hide();

        m_editVel->setText("0.8");
    }
    if(m_radioMovABSJR->isChecked()){
        m_labelVel->setText(tr("速度\n0 -- 1"));
        QStringList header;
        header<<tr("名称")<<tr("J1")<<tr("J2")<<tr("J3")<<tr("J4")<<tr("J5")<<tr("J6");
        m_tableWayPoint->setHorizontalHeaderLabels(header);

        m_labelCircleType->hide();
        m_labelVel->show();
        m_labelAcc->hide();
        m_labelJerk->hide();
        m_labelRadius->hide();
        m_labelFrame->hide();
        m_comboCircleType->hide();
        m_editVel->show();
        m_editAcc->hide();
        m_editJerk->hide();
        m_editRadius->hide();
        m_comboFrame->hide();

        m_editVel->setText("0.8");
    }
    if(m_radioMovJ->isChecked()){
        m_labelVel->setText(tr("速度\n0 -- 1"));
        QStringList header;
        header<<tr("名称")<<tr("X")<<tr("Y")<<tr("Z")<<tr("A")<<tr("B")<<tr("C");
        m_tableWayPoint->setHorizontalHeaderLabels(header);

        m_labelCircleType->hide();
        m_labelVel->show();
        m_labelAcc->hide();
        m_labelJerk->hide();
        m_labelRadius->hide();
        m_labelFrame->show();
        m_comboCircleType->hide();
        m_editVel->show();
        m_editAcc->hide();
        m_editJerk->hide();
        m_editRadius->hide();
        m_comboFrame->show();

        m_editVel->setText("0.8");
    }
    if(m_radioMovJR->isChecked()){
        m_labelVel->setText(tr("速度\n0 -- 1"));
        QStringList header;
        header<<tr("名称")<<tr("X")<<tr("Y")<<tr("Z")<<tr("A")<<tr("B")<<tr("C");
        m_tableWayPoint->setHorizontalHeaderLabels(header);

        m_labelCircleType->hide();
        m_labelVel->show();
        m_labelAcc->hide();
        m_labelJerk->hide();
        m_labelRadius->hide();
        m_labelFrame->show();
        m_comboCircleType->hide();
        m_editVel->show();
        m_editAcc->hide();
        m_editJerk->hide();
        m_editRadius->hide();
        m_comboFrame->show();

        m_editVel->setText("0.8");
    }
    if(m_radioMovL->isChecked()){
        m_labelVel->setText(tr("速度\nmm/s"));
        QStringList header;
        header<<tr("名称")<<tr("X")<<tr("Y")<<tr("Z")<<tr("A")<<tr("B")<<tr("C");
        m_tableWayPoint->setHorizontalHeaderLabels(header);

        m_labelCircleType->hide();
        m_labelVel->show();
        m_labelAcc->show();
        m_labelJerk->show();
        m_labelRadius->show();
        m_labelFrame->show();
        m_comboCircleType->hide();
        m_editVel->show();
        m_editAcc->show();
        m_editJerk->show();
        m_editRadius->show();
        m_comboFrame->show();

        m_editVel->setText("100");
    }
    if(m_radioMovLR->isChecked()){
        m_labelVel->setText(tr("速度\nmm/s"));
        QStringList header;
        header<<tr("名称")<<tr("X")<<tr("Y")<<tr("Z")<<tr("A")<<tr("B")<<tr("C");
        m_tableWayPoint->setHorizontalHeaderLabels(header);

        m_labelCircleType->hide();
        m_labelVel->show();
        m_labelAcc->show();
        m_labelJerk->show();
        m_labelRadius->show();
        m_labelFrame->show();
        m_comboCircleType->hide();
        m_editVel->show();
        m_editAcc->show();
        m_editJerk->show();
        m_editRadius->show();
        m_comboFrame->show();

        m_editVel->setText("100");
    }
    if(m_radioMovC->isChecked()){
        m_labelVel->setText(tr("速度\nmm/s"));
        QStringList header;
        header<<tr("名称")<<tr("X")<<tr("Y")<<tr("Z")<<tr("A")<<tr("B")<<tr("C");
        m_tableWayPoint->setHorizontalHeaderLabels(header);

        m_labelCircleType->show();
        m_labelVel->show();
        m_labelAcc->show();
        m_labelJerk->show();
        m_labelRadius->show();
        m_labelFrame->show();
        m_comboCircleType->show();
        m_editVel->show();
        m_editAcc->show();
        m_editJerk->show();
        m_editRadius->show();
        m_comboFrame->show();

        m_editVel->setText("100");
    }
    if(m_radioMovCR->isChecked()){
        m_labelVel->setText(tr("速度\nmm/s"));
        QStringList header;
        header<<tr("名称")<<tr("X")<<tr("Y")<<tr("Z")<<tr("A")<<tr("B")<<tr("C");
        m_tableWayPoint->setHorizontalHeaderLabels(header);

        m_labelCircleType->show();
        m_labelVel->show();
        m_labelAcc->show();
        m_labelJerk->show();
        m_labelRadius->show();
        m_labelFrame->show();
        m_comboCircleType->show();
        m_editVel->show();
        m_editAcc->show();
        m_editJerk->show();
        m_editRadius->show();
        m_comboFrame->show();

        m_editVel->setText("100");
    }

    m_editPointName->clear();
    m_comboPointName->clear();
    for(int i=0; i<6; i++){
        m_editPointValue[i]->clear();
    }
    m_tableWayPoint->clear();
    m_tableWayPoint->setRowCount(0);
}

QString RobotProgramMotionWidget::randomWayPointName(int type){
    QString name;

    map<std::string, PROGRAM_KEY_WORD> tmp_VarType=m_fileSrc->getVariableMap();
    if(type == 0){
        name = "JPos";
        QString tempName;
        do{
            tempName = name + QString::number(m_jointRandomIndex++, 10);
        }while(tmp_VarType.find(tempName.toStdString()) != tmp_VarType.end());
        name = tempName;
    }
    else{
        name = "TPos";
        QString tempName;
        do{
            tempName = name + QString::number(m_terminalRandomIndex++, 10);
        }while(tmp_VarType.find(tempName.toStdString()) != tmp_VarType.end());
        name = tempName;
    }
    return name;
}

void RobotProgramMotionWidget::updateComboList()
{
    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType= m_fileSrc->getVariableMap();
    map<string, PROGRAM_KEY_WORD>::iterator it_varType = tmp_VarType.begin();

    m_comboPointName->clear();

    if(m_radioMovABSJ->isChecked() || m_radioMovABSJR->isChecked() ){
        int i = 0;
        while(it_varType != tmp_VarType.end()){
            string variableName = it_varType->first;
            int KeyWord=it_varType->second;
            if(KeyWord == 4){
                m_comboPointName->insertItem(i,variableName.c_str());
                i++;
            }
            it_varType++;
        }

        if(i == 0)
            m_comboPointName->insertItem(0,"");

        m_comboPointName->setCurrentIndex(0);
        return;
    }

    int i = 0;
    while(it_varType != tmp_VarType.end()){
        string variableName = it_varType->first;
        int KeyWord=it_varType->second;
        if(KeyWord == 5){
            m_comboPointName->insertItem(i,variableName.c_str());
            i++;
        }
        it_varType++;
    }

    if(i == 0)
        m_comboPointName->insertItem(0,"");

    m_comboPointName->setCurrentIndex(0);
    return;
}

string RobotProgramMotionWidget::getCurrentText()
{
    string tmp = "";

    if(m_editPointName->isVisible())
        tmp = m_editPointName->text().toStdString();
    else if(m_comboPointName->isVisible())
        tmp = m_comboPointName->currentText().toStdString();
    return tmp;
}

void RobotProgramMotionWidget::resetWayPointRandomIndex(){
    m_jointRandomIndex = 1;
    m_terminalRandomIndex = 1;
}
