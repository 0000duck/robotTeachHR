#include "robotProgramProcessWidget.h"

#include <QDebug>

using namespace std;
using namespace rclib;

extern bool tag_ADD;

RobotProgramProcessWidget::RobotProgramProcessWidget(QWidget *parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramProcessWidget::~RobotProgramProcessWidget(){

}

void RobotProgramProcessWidget::passParameter(RobotInterpreter* p_file){
    m_fileSrc = p_file;
}

void RobotProgramProcessWidget::setMode(OPERATIONMODE mode){
    m_mode = mode;

    if(m_mode == TEACH_MODIFY){
        vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();
        VECTOR_STRING tmp_FuncStrentence=Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");
        string name=tmp_FuncStrentence[0];

        PROGRAM_KEY_WORD tmp_keyWord=m_fileSrc->getKeyWordIndex()[name];

        switch(tmp_keyWord){
        case FOR:{
            m_radioFor->setChecked(true);
            radioProcessTypeChanged();
            m_comboSubMenuFor->setCurrentIndex(0);
            comboSubMenuForChanged(0);

            QString tmp_Qstring;
            if(find(m_comboParameterLeft,tmp_FuncStrentence[1]) == -1){
                int tmp_count = m_comboParameterLeft->count();
                tmp_Qstring = QString::fromStdString(tmp_FuncStrentence[1]);

                m_comboParameterLeft->insertItem(tmp_count,tmp_Qstring);
                m_comboParameterLeft->setCurrentIndex(tmp_count);
            }else{
                m_comboParameterLeft->setCurrentIndex(find(m_comboParameterRight,tmp_FuncStrentence[1]));
            }
        }
            break;
        case WHILE:{
            m_radioWhile->setChecked(true);
            string tempOperator = tmp_FuncStrentence[2];
            if(tempOperator == string("==")){
                m_comboOperator->setCurrentIndex(0);
            }
            else if(tempOperator == string("!=")){
                m_comboOperator->setCurrentIndex(1);
            }
            else if(tempOperator == string(">=")){
                m_comboOperator->setCurrentIndex(2);
            }
            else if(tempOperator == string("<=")){
                m_comboOperator->setCurrentIndex(3);
            }
            else if(tempOperator == string(">")){
                m_comboOperator->setCurrentIndex(4);
            }
            else if(tempOperator == string("<")){
                m_comboOperator->setCurrentIndex(5);
            }
            radioProcessTypeChanged();
            m_comboSubMenuWhile->setCurrentIndex(0);
            comboSubMenuWhileChanged(0);

            QString tmp_Qstring;
            if(find(m_comboParameterLeft,tmp_FuncStrentence[1]) == -1){
                int tmp_count = m_comboParameterLeft->count();
                tmp_Qstring = QString::fromStdString(tmp_FuncStrentence[1]);

                m_comboParameterLeft->insertItem(tmp_count,tmp_Qstring);
                m_comboParameterLeft->setCurrentIndex(tmp_count);
            }else{
                m_comboParameterLeft->setCurrentIndex(find(m_comboParameterRight,tmp_FuncStrentence[1]));
            }

            if(find(m_comboParameterRight,tmp_FuncStrentence[3]) == -1){
                int tmp_count = m_comboParameterLeft->count();
                tmp_Qstring = QString::fromStdString(tmp_FuncStrentence[3]);

                m_comboParameterRight->insertItem(tmp_count,tmp_Qstring);
                m_comboParameterRight->setCurrentIndex(tmp_count);
            }else{
                m_comboParameterRight->setCurrentIndex(find(m_comboParameterRight,tmp_FuncStrentence[3]));
            }
        }
            break;
        case IF:{
            m_radioIf->setChecked(true);
            string tempOperator = tmp_FuncStrentence[2];
            if(tempOperator == string("==")){
                m_comboOperator->setCurrentIndex(0);
            }
            else if(tempOperator == string("!=")){
                m_comboOperator->setCurrentIndex(1);
            }
            else if(tempOperator == string(">=")){
                m_comboOperator->setCurrentIndex(2);
            }
            else if(tempOperator == string("<=")){
                m_comboOperator->setCurrentIndex(3);
            }
            else if(tempOperator == string(">")){
                m_comboOperator->setCurrentIndex(4);
            }
            else if(tempOperator == string("<")){
                m_comboOperator->setCurrentIndex(5);
            }
            radioProcessTypeChanged();
            m_comboSubMenuIf->setCurrentIndex(0);
            comboSubMenuIfChanged(0);

            QString tmp_Qstring;
            if(find(m_comboParameterLeft,tmp_FuncStrentence[1]) == -1){
                int tmp_count = m_comboParameterLeft->count();
                tmp_Qstring = QString::fromStdString(tmp_FuncStrentence[1]);

                m_comboParameterLeft->insertItem(tmp_count,tmp_Qstring);
                m_comboParameterLeft->setCurrentIndex(tmp_count);
            }else{
                m_comboParameterLeft->setCurrentIndex(find(m_comboParameterRight,tmp_FuncStrentence[1]));
            }

            if(find(m_comboParameterRight,tmp_FuncStrentence[3]) == -1){
                int tmp_count = m_comboParameterLeft->count();
                tmp_Qstring = QString::fromStdString(tmp_FuncStrentence[3]);

                m_comboParameterRight->insertItem(tmp_count,tmp_Qstring);
                m_comboParameterRight->setCurrentIndex(tmp_count);
            }else{
                m_comboParameterRight->setCurrentIndex(find(m_comboParameterRight,tmp_FuncStrentence[3]));
            }
        }
            break;
        case WAITDIN:{
            m_radioIO->setChecked(true);
            radioProcessTypeChanged();
            m_comboSubMenuIO->setCurrentIndex(0);
            comboSubMenuIOChanged(0);
            m_comboParameterRight->setCurrentIndex(find(m_comboParameterLeft,tmp_FuncStrentence[1]));
            m_comboParameterRight->setCurrentIndex(find(m_comboParameterRight,tmp_FuncStrentence[2]));
        }
            break;
        case DOUT:{
            m_radioIO->setChecked(true);
            radioProcessTypeChanged();
            m_comboSubMenuIO->setCurrentIndex(1);
            comboSubMenuIOChanged(1);
            m_comboParameterRight->setCurrentIndex(find(m_comboParameterLeft,tmp_FuncStrentence[1]));
            m_comboParameterRight->setCurrentIndex(find(m_comboParameterRight,tmp_FuncStrentence[2]));
        }
            break;
        case AOUT:{
            m_radioIO->setChecked(true);
            radioProcessTypeChanged();
            m_comboSubMenuIO->setCurrentIndex(2);
            comboSubMenuIOChanged(2);

            m_comboParameterLeft->setCurrentIndex(find(m_comboParameterLeft,tmp_FuncStrentence[1]));

            QString tmp_Qstring;
            if(find(m_comboParameterRight,tmp_FuncStrentence[2]) == -1){
                tmp_Qstring = QString::fromStdString(tmp_FuncStrentence[2]);

                m_comboParameterRight->clear();
                m_comboParameterRight->insertItem(0,tmp_Qstring);
                m_comboParameterRight->setCurrentIndex(0);
            }else{
                m_comboParameterRight->setCurrentIndex(find(m_comboParameterRight,tmp_FuncStrentence[2]));
            }
        }
            break;
        case DELAY:{
            m_radioTime->setChecked(true);
            radioProcessTypeChanged();
            m_comboSubMenuTime->setCurrentIndex(0);
            comboSubMenuTimeChanged(0);

            QString tmp_Qstring;
            if(find(m_comboParameterLeft,tmp_FuncStrentence[1]) == -1){
                int tmp_count = m_comboParameterLeft->count();
                tmp_Qstring = QString::fromStdString(tmp_FuncStrentence[1]);

                m_comboParameterLeft->insertItem(tmp_count,tmp_Qstring);
                m_comboParameterLeft->setCurrentIndex(tmp_count);
            }else{
                m_comboParameterLeft->setCurrentIndex(find(m_comboParameterLeft,tmp_FuncStrentence[2]));
            }
        }
            break;
        case COM:{
            m_radioCom->setChecked(true);
            radioProcessTypeChanged();
            m_comboSubMenuCom->setCurrentIndex(0);
            comboSubMenuComChanged(0);

            string tmp = "";
            for(int i = 1;i<tmp_FuncStrentence.size();i++){
                tmp+=+" "+tmp_FuncStrentence[i];
            }

            m_editCom->setText(QString::fromStdString(tmp));
            m_editCom->show();
        }
            break;
        case CALL:{
            m_radioFunc->setChecked(true);
            radioProcessTypeChanged();
            m_comboSubMenuFunc->setCurrentIndex(0);
            comboSubMenuFuncChanged(0);

            m_comboParameterLeft->setCurrentIndex(find(m_comboParameterLeft,tmp_FuncStrentence[1]));
        }
            break;
        default:
            break;
        }

        groupProcessType->setEnabled(false);
        m_comboSubMenuFunc->setEnabled(false);
        m_comboSubMenuFor->setEnabled(false);
        m_comboSubMenuWhile->setEnabled(false);
        m_comboSubMenuIf->setEnabled(false);
        m_comboSubMenuIO->setEnabled(false);
        m_comboSubMenuTime->setEnabled(false);
        m_comboSubMenuCom->setEnabled(false);
    }
    else{
        clearState();
    }
}

void RobotProgramProcessWidget::passSentenceIndex(int func, int line){
    m_func = func;
    m_line = line;
}

void RobotProgramProcessWidget::initUI(){
    m_radioFunc = new QRadioButton(tr("函数"));
    m_radioFor = new QRadioButton(tr("循环"));
    m_radioWhile = new QRadioButton(tr("条件循环"));
    m_radioIf = new QRadioButton(tr("条件判断"));
    m_radioIO = new QRadioButton(tr("输入输出"));
    m_radioTime = new QRadioButton(tr("延时"));
    m_radioCom = new QRadioButton(tr("注释"));

    QVBoxLayout* layoutProcessType1 = new QVBoxLayout();
    layoutProcessType1->addWidget(m_radioFor);
    layoutProcessType1->addWidget(m_radioFunc);
    QVBoxLayout* layoutProcessType2 = new QVBoxLayout();
    layoutProcessType2->addWidget(m_radioWhile);
    layoutProcessType2->addWidget(m_radioTime);
    QVBoxLayout* layoutProcessType3 = new QVBoxLayout();
    layoutProcessType3->addWidget(m_radioIf);
    layoutProcessType3->addWidget(m_radioCom);
    QVBoxLayout* layoutProcessType4 = new QVBoxLayout();
    layoutProcessType4->addWidget(m_radioIO);
    QHBoxLayout* layoutProcessType = new QHBoxLayout();
    layoutProcessType->addLayout(layoutProcessType1);
    layoutProcessType->addLayout(layoutProcessType2);
    layoutProcessType->addLayout(layoutProcessType3);
    layoutProcessType->addLayout(layoutProcessType4);
    groupProcessType = new QGroupBox(tr("过程"));
    groupProcessType->setLayout(layoutProcessType);

    m_labelParameterLeft = new QLabel("");
    m_labelParameterOperator = new QLabel("");
    m_labelParameterRight = new QLabel("");
    m_comboOperator = new ComboBox();

    m_editCom = new QLineEdit();
    m_editCom->setFixedSize(0.3*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_comboParameterLeft = new QComboBox();
    m_comboParameterRight = new QComboBox();
    m_comboParameterLeft->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboParameterRight->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboParameterLeft->setMaxVisibleItems(7);
    m_comboParameterRight->setMaxVisibleItems(7);
    m_comboParameterLeft->setEditable(true);
    m_comboParameterRight->setEditable(true);
    m_comboParameterLeft->setStyleSheet("QComboBox QAbstractItemView{"
                                           "font-size: 18px;"
                                       "}");
    m_comboParameterLeft->setView(new  QListView());
    m_comboParameterRight->setStyleSheet("QComboBox QAbstractItemView{"
                                           "font-size: 18px;"
                                       "}");
    m_comboParameterRight->setView(new  QListView());

    m_comboSubMenuFunc = new ComboBox();
    m_comboSubMenuFor = new ComboBox();
    m_comboSubMenuWhile = new ComboBox();
    m_comboSubMenuIf = new ComboBox();
    m_comboSubMenuIO = new ComboBox();
    m_comboSubMenuTime = new ComboBox();
    m_comboSubMenuCom = new ComboBox();

    m_comboOperator->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboSubMenuFunc->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboSubMenuFor->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboSubMenuWhile->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboSubMenuIf->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboSubMenuIO->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboSubMenuTime->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_comboSubMenuCom->setFixedSize(0.1*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutParameterContent1 = new QVBoxLayout();
    layoutParameterContent1->addWidget(m_labelParameterLeft);
    layoutParameterContent1->addWidget(m_comboParameterLeft);
    layoutParameterContent1->addWidget(m_editCom);

    QVBoxLayout* layoutParameterContent2 = new QVBoxLayout();
    layoutParameterContent2->addWidget(m_labelParameterOperator);
    layoutParameterContent2->addWidget(m_comboOperator);

    QVBoxLayout* layoutParameterContent3 = new QVBoxLayout();
    layoutParameterContent3->addWidget(m_labelParameterRight);
    layoutParameterContent3->addWidget(m_comboParameterRight);

    QHBoxLayout* layoutParameterContent = new QHBoxLayout();
    layoutParameterContent->addLayout(layoutParameterContent1);
    layoutParameterContent->addLayout(layoutParameterContent2);
    layoutParameterContent->addLayout(layoutParameterContent3);

    QHBoxLayout* layoutProcessChoice = new QHBoxLayout();
    layoutProcessChoice->addWidget(m_comboSubMenuFunc, 0, Qt::AlignLeft | Qt::AlignTop);
    layoutProcessChoice->addWidget(m_comboSubMenuFor, 0, Qt::AlignLeft | Qt::AlignTop);
    layoutProcessChoice->addWidget(m_comboSubMenuWhile, 0, Qt::AlignLeft | Qt::AlignTop);
    layoutProcessChoice->addWidget(m_comboSubMenuIf, 0, Qt::AlignLeft | Qt::AlignTop);
    layoutProcessChoice->addWidget(m_comboSubMenuIO, 0, Qt::AlignLeft | Qt::AlignTop);
    layoutProcessChoice->addWidget(m_comboSubMenuTime, 0, Qt::AlignLeft | Qt::AlignTop);
    layoutProcessChoice->addWidget(m_comboSubMenuCom, 0, Qt::AlignLeft | Qt::AlignTop);

    QVBoxLayout* layoutProcessContent = new QVBoxLayout();
    layoutProcessContent->addLayout(layoutProcessChoice);
    layoutProcessContent->addLayout(layoutParameterContent);
    QGroupBox* groupProcessContent = new QGroupBox();
    groupProcessContent->setLayout(layoutProcessContent);

    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    m_buttonYes->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QVBoxLayout* layoutSure = new QVBoxLayout();
    layoutSure->addWidget(m_buttonYes);
    layoutSure->addWidget(m_buttonNo);
    QGroupBox* groupSure = new QGroupBox();
    groupSure->setLayout(layoutSure);

    QVBoxLayout* layoutProcess = new QVBoxLayout(this);
    layoutProcess->addWidget(groupProcessType);
    layoutProcess->addWidget(groupProcessContent);
    layoutProcess->addWidget(groupSure, 0, Qt::AlignRight);
    layoutProcess->setStretchFactor(groupProcessType, 2);
    layoutProcess->setStretchFactor(groupProcessContent, 4);
    layoutProcess->setStretchFactor(groupSure, 2);

    m_editCom->hide();
}

void RobotProgramProcessWidget::initState(){
    m_comboOperator->insertItem(0, "==");
    m_comboOperator->insertItem(1, "!=");
    m_comboOperator->insertItem(2, ">=");
    m_comboOperator->insertItem(3, "<=");
    m_comboOperator->insertItem(4, ">");
    m_comboOperator->insertItem(5, "<");

    m_comboSubMenuFunc->insertItem(0, "CALL\n调用");
    m_comboSubMenuFunc->insertItem(1, "RETURN\n返回");

    m_comboSubMenuFor->insertItem(0, "FOR\n循环");
    m_comboSubMenuFor->insertItem(1, "ENDFOR\n结束循环");

    m_comboSubMenuWhile->insertItem(0, "WHILE\n条件循环");
    m_comboSubMenuWhile->insertItem(1, "ENDWHILE\n结束条件循环");

    m_comboSubMenuIf->insertItem(0, "IF\n判断");
    m_comboSubMenuIf->insertItem(1, "ELSE\n或者");
    m_comboSubMenuIf->insertItem(2, "ENDIF\n结束判断");
    m_comboSubMenuIf->insertItem(3, "BREAK\n跳出");
    m_comboSubMenuIf->insertItem(4, "CONTINUE\n继续");

    m_comboSubMenuIO->insertItem(0, "WAITDIN\n等待数字输入");
    m_comboSubMenuIO->insertItem(1, "DOUT\n数字输出");
    m_comboSubMenuIO->insertItem(2, "AOUT\n模拟输出");

    m_comboSubMenuTime->insertItem(0, "DELAY\n延时");

    m_comboSubMenuCom->insertItem(0, "COM\n注释");

    clearState();
}

void RobotProgramProcessWidget::connectSignalSlot(){
    QObject::connect(m_radioFunc, SIGNAL(toggled(bool)), this, SLOT(radioProcessTypeChanged()));
    QObject::connect(m_radioFor, SIGNAL(toggled(bool)), this, SLOT(radioProcessTypeChanged()));
    QObject::connect(m_radioWhile, SIGNAL(toggled(bool)), this, SLOT(radioProcessTypeChanged()));
    QObject::connect(m_radioIf, SIGNAL(toggled(bool)), this, SLOT(radioProcessTypeChanged()));
    QObject::connect(m_radioIO, SIGNAL(toggled(bool)), this, SLOT(radioProcessTypeChanged()));
    QObject::connect(m_radioTime, SIGNAL(toggled(bool)), this, SLOT(radioProcessTypeChanged()));
    QObject::connect(m_radioCom, SIGNAL(toggled(bool)), this, SLOT(radioProcessTypeChanged()));

    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));

    QObject::connect(m_comboSubMenuFunc, SIGNAL(activated(int)), this, SLOT(comboSubMenuFuncChanged(int)));
    QObject::connect(m_comboSubMenuFor, SIGNAL(activated(int)), this, SLOT(comboSubMenuForChanged(int)));
    QObject::connect(m_comboSubMenuWhile, SIGNAL(activated(int)), this, SLOT(comboSubMenuWhileChanged(int)));
    QObject::connect(m_comboSubMenuIf, SIGNAL(activated(int)), this, SLOT(comboSubMenuIfChanged(int)));
    QObject::connect(m_comboSubMenuIO, SIGNAL(activated(int)), this, SLOT(comboSubMenuIOChanged(int)));
    QObject::connect(m_comboSubMenuTime, SIGNAL(activated(int)), this, SLOT(comboSubMenuTimeChanged(int)));
    QObject::connect(m_comboSubMenuCom, SIGNAL(activated(int)), this, SLOT(comboSubMenuComChanged(int)));
}

void RobotProgramProcessWidget::clearState(){
    m_radioFor->setChecked(true);
    radioProcessTypeChanged();

    groupProcessType->setEnabled(true);
    m_comboSubMenuFunc->setEnabled(true);
    m_comboSubMenuFor->setEnabled(true);
    m_comboSubMenuWhile->setEnabled(true);
    m_comboSubMenuIf->setEnabled(true);
    m_comboSubMenuIO->setEnabled(true);
    m_comboSubMenuTime->setEnabled(true);
    m_comboSubMenuCom->setEnabled(true);
}

bool RobotProgramProcessWidget::grammarCheck(){
    if(m_radioFor->isChecked()){
        if(m_comboSubMenuFor->currentIndex() == 0){
            if(m_comboParameterLeft->currentText().toStdString() == ""){
                QMessageBox::question(this, tr("提示"),
                                      tr("参数不能为空!"),
                                      QMessageBox::Yes);
                return false;
            }
        }
    }
    else if(m_radioWhile->isChecked()){
        if(m_comboSubMenuWhile->currentIndex() == 0){
            if(m_comboParameterLeft->currentText().toStdString() == "" || m_comboParameterRight->currentText().toStdString() == ""){
                QMessageBox::question(this, tr("提示"),
                                      tr("参数不能为空!"),
                                      QMessageBox::Yes);
                return false;
            }
            QString tmp_nameL = m_comboParameterLeft->currentText();
            if(digitalCheck(tmp_nameL))
            {
                if(!variableCheck(tmp_nameL))
                    return false;
            }

            QString tmp_nameR = m_comboParameterRight->currentText();
            if(digitalCheck(tmp_nameR))
            {
                if(!variableCheck(tmp_nameR))
                    return false;
            }
        }
    }
    else if(m_radioIf->isChecked()){
        if(m_comboSubMenuIf->currentIndex() == 0){
            if(m_comboParameterLeft->currentText().toStdString() == ""  || m_comboParameterRight->currentText().toStdString() == "" ){
                QMessageBox::question(this, tr("提示"),
                                      tr("参数不能为空!"),
                                      QMessageBox::Yes);
                return false;
            }

            QString tmp_nameL = m_comboParameterLeft->currentText();
            if(digitalCheck(tmp_nameL))
            {
                if(!variableCheck(tmp_nameL))
                    return false;
            }

            QString tmp_nameR = m_comboParameterRight->currentText();
            if(digitalCheck(tmp_nameR))
            {
                if(!variableCheck(tmp_nameR))
                    return false;
            }
        }
    }
    else if(m_radioIO->isChecked()){
        if(m_comboParameterLeft->currentText().toStdString() == "" || m_comboParameterRight->currentText().toStdString() == "" ){
            QMessageBox::question(this, tr("提示"),
                                  tr("参数不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }
    }
    else if(m_radioFunc->isChecked()){
        if(m_comboSubMenuFunc->currentIndex() == 0 || m_comboSubMenuFor->currentIndex() == 2){
            if(m_comboParameterLeft->currentText().toStdString() == "" ){
                QMessageBox::question(this, tr("提示"),
                                      tr("参数不能为空!"),
                                      QMessageBox::Yes);
                return false;
            }
           funcCheck(m_comboParameterLeft->currentText());
        }
    }
    else if(m_radioTime->isChecked()){
        if(m_comboParameterLeft->currentText().toStdString() == "" ){
            QMessageBox::question(this, tr("提示"),
                                  tr("参数不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }
    }
    else if(m_radioCom->isChecked()){
        if(m_editCom->text().toStdString() == "" ){
            QMessageBox::question(this, tr("提示"),
                                  tr("参数不能为空!"),
                                  QMessageBox::Yes);
            return false;
        }
    }

    return true;
}

bool RobotProgramProcessWidget::digitalCheck(const QString& varName)
{
        const char *s = varName.toUtf8().data();
        while(*s && *s>='0' && *s<='9')
            s++;
        return bool(*s);
}

bool RobotProgramProcessWidget::variableCheck(const QString &varName)
{    
    if(varName == QString("ON") || varName == QString("OFF")){
        return true;
    }

    QString IO =varName.mid(0,2);
    int index = varName.mid(2).toInt();

    if(IO == QString("DI") || IO == QString("DO")){
        if((index >= 1) && (index <= 16)){
            return true;
        }
    }else if(IO == QString("AI")){
        if((index >= 1) && (index <= 4)){
            return true;
        }
    }

    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType= m_fileSrc->getVariableMap();
    if(tmp_VarType.find(varName.toStdString()) == tmp_VarType.end()){
        QMessageBox::question(this, tr("提示"),
                              tr("变量不存在于变量列表中!"),
                              QMessageBox::Yes);
        return false;
    }
    return true;
}

bool RobotProgramProcessWidget::funcCheck(const QString &funcName)
{
    vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();

    bool JUD = false;
    for(int i=1; i<tmp_FuncTable.size(); i++){
        VECTOR_STRING tmp_Strentence=Common::ToVectors(tmp_FuncTable[i][0].strSentence," ");
        if(funcName.toStdString() == tmp_Strentence[1]){
            JUD = true;
            break;
        }
    }

    if(!JUD){
        QMessageBox::question(this, tr("提示"),
                              tr("该函数不存在于函数列表中!"),
                              QMessageBox::Yes);
        return false;
    }
    return true;
}

void RobotProgramProcessWidget::updateComboList()
{
    if(m_radioFunc->isChecked()){
        updateFunc();
    }

    if(m_radioFor->isChecked()){
        updateFor();
    }

    if(m_radioWhile->isChecked() || m_radioIf->isChecked()){
        updateWhile();
    }

    if(m_radioIO->isChecked()){
       updateIO();
    }

    if(m_radioTime->isChecked()){
        updateTime();
    }
}

int RobotProgramProcessWidget::find(QComboBox *combox, string tmp)
{
    int count = combox->count();
    int i = 0;
    for(;i < count;i++){
        if(tmp == combox->itemText(i).toStdString())
            return i;
    }
    if(i >= combox->count())
        return -1;
}

void RobotProgramProcessWidget::updateFor()
{
    m_comboParameterLeft->clear();
    m_comboParameterRight->clear();

    if(m_fileSrc == NULL)
        return;

    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType= m_fileSrc->getVariableMap();
    map<string, PROGRAM_KEY_WORD>::iterator it_varType = tmp_VarType.begin();

    int i = 0;
    while(it_varType != tmp_VarType.end()){
        string variableName = it_varType->first;
        int KeyWord=it_varType->second;
        if(KeyWord == 2){
            m_comboParameterLeft->insertItem(i,variableName.c_str());
            i++;
        }
        it_varType++;
    }

    if(i == 0)
        m_comboParameterLeft->insertItem(0,"");

    m_comboParameterLeft->setCurrentIndex(0);
    return;
}

void RobotProgramProcessWidget::updateWhile()
{
    m_comboParameterLeft->clear();
    m_comboParameterRight->clear();

    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType= m_fileSrc->getVariableMap();
    map<string, PROGRAM_KEY_WORD>::iterator it_varType = tmp_VarType.begin();

    int i = 0;
    while(it_varType != tmp_VarType.end()){
        string variableName = it_varType->first;
        int KeyWord=it_varType->second;
        if((KeyWord == 4) || (KeyWord == 5) || (KeyWord == 28)){
            it_varType++;
            continue;
        }

        m_comboParameterLeft->insertItem(i,variableName.c_str());
        m_comboParameterRight->insertItem(i,variableName.c_str());

        i++;
        it_varType++;
    }

    m_comboParameterLeft->insertItem(i,"DI");
    m_comboParameterRight->insertItem(i,"ON");
    i++;
    m_comboParameterLeft->insertItem(i,"DO");
    m_comboParameterRight->insertItem(i,"OFF");
    i++;
    m_comboParameterLeft->insertItem(i,"AO");

    if(i == 0){
        m_comboParameterLeft->insertItem(0,"");
        m_comboParameterRight->insertItem(0,"");

        m_comboParameterLeft->setCurrentIndex(0);
        m_comboParameterRight->setCurrentIndex(0);
        return;
    }

}

void RobotProgramProcessWidget::updateIO()
{
    m_comboParameterLeft->clear();
    m_comboParameterRight->clear();

    if(m_comboSubMenuIO->currentIndex() == 0){
        for(int i = 1;i<17;i++){
            string tmp = string("DI")+QString::number(i, 10).toStdString();
            m_comboParameterLeft->insertItem(i-1,tmp.c_str());
        }

        m_comboParameterRight->insertItem(0,"ON");
        m_comboParameterRight->insertItem(1,"OFF");
        m_comboParameterRight->insertItem(2,"FLIP");


        m_comboParameterLeft->setCurrentIndex(0);
        m_comboParameterRight->setCurrentIndex(0);
        return;
    }else if(m_comboSubMenuIO->currentIndex() == 1){
        for(int i = 1;i<17;i++){
            string tmp = string("DO")+QString::number(i, 10).toStdString();
            m_comboParameterLeft->insertItem(i-1,tmp.c_str());
        }

        m_comboParameterRight->insertItem(0,"ON");
        m_comboParameterRight->insertItem(1,"OFF");
        m_comboParameterRight->insertItem(2,"FLIP");


        m_comboParameterLeft->setCurrentIndex(0);
        m_comboParameterRight->setCurrentIndex(0);
        return;

    }else if(m_comboSubMenuIO->currentIndex() == 2){
        for(int i = 1;i<5;i++){
            string tmp = string("AO")+QString::number(i, 10).toStdString();
            m_comboParameterLeft->insertItem(i-1,tmp.c_str());
        }

        std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType= m_fileSrc->getVariableMap();
        map<string, PROGRAM_KEY_WORD>::iterator it_varType = tmp_VarType.begin();

        int i = 0;
        while(it_varType != tmp_VarType.end()){
            string variableName = it_varType->first;
            int KeyWord=it_varType->second;
            if(KeyWord == 2 || KeyWord == 3){
                m_comboParameterLeft->insertItem(i,variableName.c_str());
                i++;
            }
            it_varType++;
        }
    }

    m_comboParameterRight->insertItem(0,"");

    m_comboParameterLeft->setCurrentIndex(0);
    m_comboParameterRight->setCurrentIndex(0);
    return;
}

void RobotProgramProcessWidget::updateFunc()
{
    m_comboParameterLeft->clear();
    m_comboParameterRight->clear();

    std::vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();
    if(/*tmp_VarType == NULL|| */tmp_FuncTable.size() < 2){
        m_comboParameterLeft->insertItem(0,"");
        return;
    }
    for(int i=1; i<tmp_FuncTable.size(); i++){
        string funcName = Common::ToVectors(tmp_FuncTable[i][0].strSentence," ")[1];
        m_comboParameterLeft->insertItem(i-1,funcName.c_str());
    }
    m_comboParameterLeft->setCurrentIndex(0);
    return;
}

void RobotProgramProcessWidget::updateTime()
{
    m_comboParameterLeft->clear();
    m_comboParameterRight->clear();

    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType= m_fileSrc->getVariableMap();
    map<string, PROGRAM_KEY_WORD>::iterator it_varType = tmp_VarType.begin();

    int i = 0;
    while(it_varType != tmp_VarType.end()){
        string variableName = it_varType->first;
        int KeyWord=it_varType->second;
        if(KeyWord == 2 || KeyWord == 3){
            m_comboParameterLeft->insertItem(i,variableName.c_str());
            i++;
        }
        it_varType++;
    }

    if(i == 0)
        m_comboParameterLeft->insertItem(0,"");

    m_comboParameterLeft->setCurrentIndex(0);
    return;
}

void RobotProgramProcessWidget::buttonYesClicked(){
    if(!grammarCheck()){
        return;
    }

    string tempSentence;

    if(m_radioFor->isChecked()){
        tempSentence="";
        tempSentence=tempSentence+m_comboSubMenuFor->currentText().toStdString();
        if(m_comboSubMenuFor->currentIndex() == 0){
            tempSentence=tempSentence+" "+m_comboParameterLeft->currentText().toStdString();
        }
    }
    else if(m_radioWhile->isChecked()){
        tempSentence="";
        tempSentence=tempSentence+m_comboSubMenuWhile->currentText().toStdString();
        if(m_comboSubMenuWhile->currentIndex() == 0){
            tempSentence=tempSentence+" "+m_comboParameterLeft->currentText().toStdString()+
                    " "+m_comboOperator->currentText().toStdString()+" "+m_comboParameterRight->currentText().toStdString();
        }
    }
    else if(m_radioIf->isChecked()){
        tempSentence="";
        tempSentence=tempSentence+m_comboSubMenuIf->currentText().toStdString();
        if(m_comboSubMenuIf->currentIndex() == 0){
            tempSentence=tempSentence+" "+m_comboParameterLeft->currentText().toStdString()+" "+
                    m_comboOperator->currentText().toStdString()+" "+m_comboParameterRight->currentText().toStdString();
        }
    }
    else if(m_radioIO->isChecked()){
        tempSentence="";
        tempSentence=tempSentence+m_comboSubMenuIO->currentText().toStdString();
        tempSentence=tempSentence+" "+m_comboParameterLeft->currentText().toStdString()+" "+
                m_comboParameterRight->currentText().toStdString();
    }
    else if(m_radioFunc->isChecked()){
        tempSentence="";
        tempSentence=tempSentence+m_comboSubMenuFunc->currentText().toStdString();
        if(m_comboSubMenuFunc->currentIndex() == 0 || m_comboSubMenuFor->currentIndex() == 2){
            tempSentence=tempSentence+" "+m_comboParameterLeft->currentText().toStdString();
        }
    }
    else if(m_radioTime->isChecked()){
        tempSentence="";
        tempSentence=tempSentence+m_comboSubMenuTime->currentText().toStdString()+" "+m_comboParameterLeft->currentText().toStdString();
    }
    else if(m_radioCom->isChecked()){
        tempSentence=""; 
        tempSentence=tempSentence+m_comboSubMenuCom->currentText().toStdString()+" "+m_editCom->text().toStdString();
    }

    ProgramPointer tmp_Pointer;
    tmp_Pointer.nFunction=m_func;
    tmp_Pointer.nSentence=m_line;

    ProgramSentence tmp_Strentence;
    tmp_Strentence.strSentence=tempSentence;

    if(m_mode == TEACH_MODIFY){
        m_fileSrc->modifyProgramSentence(tmp_Pointer,tmp_Strentence);
        tag_ADD = false;
    }
    else{
        tmp_Pointer.nSentence=m_line+1;
        m_fileSrc->addProgramSentence(tmp_Pointer,tmp_Strentence);

        VECTOR_STRING tmp_KeyWord=Common::ToVectors(tempSentence," ");
        if(tmp_KeyWord[0] == string("FOR")){
            tmp_Strentence.strSentence=string("ENDFOR");
            tmp_Pointer.nSentence=m_line+2;
            m_fileSrc->addProgramSentence(tmp_Pointer,tmp_Strentence);
        }
        else if(tmp_KeyWord[0] == string("WHILE")){;
            tmp_Strentence.strSentence=string("ENDWHILE");
            tmp_Pointer.nSentence=m_line+2;
            m_fileSrc->addProgramSentence(tmp_Pointer,tmp_Strentence);
        }
        else if(tmp_KeyWord[0] == string("IF")){
            tmp_Strentence.strSentence=string("ENDIF");
            tmp_Pointer.nSentence=m_line+2;
            m_fileSrc->addProgramSentence(tmp_Pointer,tmp_Strentence);
        }

        tag_ADD = true;
    }

    emit programOperationFinishedSignal(true);
}

void RobotProgramProcessWidget::buttonNoClicked(){
    emit programOperationFinishedSignal(false);
}

void RobotProgramProcessWidget::radioProcessTypeChanged(){
    m_comboOperator->setEnabled(true);

    updateComboList();

    if(m_radioFor->isChecked()){
        m_comboSubMenuFunc->hide();
        m_comboSubMenuFor->show();
        m_comboSubMenuWhile->hide();
        m_comboSubMenuIf->hide();
        m_comboSubMenuIO->hide();
        m_comboSubMenuTime->hide();
        m_comboSubMenuCom->hide();

        m_labelParameterLeft->show();
        m_labelParameterOperator->hide();
        m_labelParameterRight->hide();
        m_comboParameterLeft->show();
        m_comboParameterRight->hide();
        m_comboOperator->hide();
        m_editCom->hide();

        m_comboSubMenuFor->setCurrentIndex(0);
        comboSubMenuForChanged(0);
    }
    if(m_radioWhile->isChecked()){
        m_comboSubMenuFunc->hide();
        m_comboSubMenuFor->hide();
        m_comboSubMenuWhile->show();
        m_comboSubMenuIf->hide();
        m_comboSubMenuIO->hide();
        m_comboSubMenuTime->hide();
        m_comboSubMenuCom->hide();

        m_labelParameterLeft->show();
        m_labelParameterOperator->show();
        m_labelParameterRight->show();
        m_comboParameterLeft->show();
        m_comboParameterRight->show();
        m_comboOperator->show();
        m_editCom->hide();

        m_comboSubMenuWhile->setCurrentIndex(0);
        comboSubMenuWhileChanged(0);
    }
    if(m_radioIf->isChecked()){
        m_comboSubMenuFunc->hide();
        m_comboSubMenuFor->hide();
        m_comboSubMenuWhile->hide();
        m_comboSubMenuIf->show();
        m_comboSubMenuIO->hide();
        m_comboSubMenuTime->hide();
        m_comboSubMenuCom->hide();

        m_labelParameterLeft->show();
        m_labelParameterOperator->show();
        m_labelParameterRight->show();
        m_comboParameterLeft->show();
        m_comboParameterRight->show();
        m_comboOperator->show();
        m_editCom->hide();

        m_comboSubMenuIf->setCurrentIndex(0);
        comboSubMenuIfChanged(0);
    }
    if(m_radioIO->isChecked()){
        m_comboSubMenuFunc->hide();
        m_comboSubMenuFor->hide();
        m_comboSubMenuWhile->hide();
        m_comboSubMenuIf->hide();
        m_comboSubMenuIO->show();
        m_comboSubMenuTime->hide();
        m_comboSubMenuCom->hide();

        m_labelParameterLeft->show();
        m_labelParameterOperator->show();
        m_labelParameterRight->show();
        m_comboParameterLeft->show();
        m_comboParameterRight->hide();
        m_comboOperator->show();
        m_editCom->hide();

        m_comboOperator->setCurrentIndex(0);
        m_comboOperator->setEnabled(false);

        m_comboSubMenuIO->setCurrentIndex(0);
        comboSubMenuIOChanged(0);
    }
    if(m_radioFunc->isChecked()){
        m_comboSubMenuFunc->show();
        m_comboSubMenuFor->hide();
        m_comboSubMenuWhile->hide();
        m_comboSubMenuIf->hide();
        m_comboSubMenuIO->hide();
        m_comboSubMenuTime->hide();
        m_comboSubMenuCom->hide();

        m_labelParameterLeft->show();
        m_labelParameterOperator->hide();
        m_labelParameterRight->hide();
        m_comboParameterLeft->show();
        m_comboParameterRight->hide();
        m_comboOperator->hide();
        m_editCom->hide();

        m_comboSubMenuFunc->setCurrentIndex(0);
        comboSubMenuFuncChanged(0);
    }
    if(m_radioTime->isChecked()){
        m_comboSubMenuFunc->hide();
        m_comboSubMenuFor->hide();
        m_comboSubMenuWhile->hide();
        m_comboSubMenuIf->hide();
        m_comboSubMenuIO->hide();
        m_comboSubMenuTime->show();
        m_comboSubMenuCom->hide();

        m_labelParameterLeft->show();
        m_labelParameterOperator->hide();
        m_labelParameterRight->hide();
        m_comboParameterLeft->show();
        m_comboParameterRight->hide();
        m_comboOperator->hide();
        m_editCom->hide();

        m_comboSubMenuTime->setCurrentIndex(0);
        comboSubMenuTimeChanged(0);
    }
    if(m_radioCom->isChecked()){
        m_comboSubMenuFunc->hide();
        m_comboSubMenuFor->hide();
        m_comboSubMenuWhile->hide();
        m_comboSubMenuIf->hide();
        m_comboSubMenuIO->hide();
        m_comboSubMenuTime->hide();
        m_comboSubMenuCom->show();

        m_labelParameterLeft->show();
        m_labelParameterOperator->hide();
        m_labelParameterRight->hide();
        m_comboParameterLeft->hide();
        m_comboParameterRight->hide();
        m_comboOperator->hide();
        m_editCom->show();

        m_comboSubMenuCom->setCurrentIndex(0);
        comboSubMenuComChanged(0);
    }
}

void RobotProgramProcessWidget::comboSubMenuFuncChanged(int){
    switch(m_comboSubMenuFunc->currentIndex()){
    case 0:{
        m_labelParameterLeft->setText(tr("函数名"));
        m_labelParameterLeft->show();
        m_comboParameterLeft->show();
        m_comboParameterRight->hide();
    }
        break;
    default:{
        m_labelParameterLeft->hide();
        m_comboParameterLeft->hide();
        m_comboParameterRight->hide();
    }
        break;
    }
}

void RobotProgramProcessWidget::comboSubMenuForChanged(int){
    switch(m_comboSubMenuFor->currentIndex()){
    case 0:{
        m_labelParameterLeft->setText(tr("循环次数"));
        m_labelParameterLeft->show();
        m_comboParameterLeft->show();
        m_comboParameterRight->hide();
    }
        break;
    case 1:{
        m_labelParameterLeft->hide();
        m_comboParameterLeft->hide();
        m_comboParameterRight->hide();
    }
        break;
    default:
        break;
    }
}

void RobotProgramProcessWidget::comboSubMenuWhileChanged(int){
    switch(m_comboSubMenuWhile->currentIndex()){
    case 0:{
        m_labelParameterLeft->setText(tr("判断左值"));
        m_labelParameterRight->setText(tr("判断右值"));
        m_labelParameterLeft->show();
        m_labelParameterOperator->show();
        m_labelParameterRight->show();
        m_comboParameterLeft->show();
        m_comboParameterRight->show();
        m_comboOperator->show();
    }
        break;
    case 1:{
        m_labelParameterLeft->hide();
        m_labelParameterOperator->hide();
        m_labelParameterRight->hide();
        m_comboParameterLeft->hide();
        m_comboParameterRight->hide();
        m_comboOperator->hide();
    }
        break;
    default:
        break;
    }
}

void RobotProgramProcessWidget::comboSubMenuIfChanged(int){
    switch(m_comboSubMenuIf->currentIndex()){
    case 0:{
        m_labelParameterLeft->setText(tr("判断左值"));
        m_labelParameterRight->setText(tr("判断右值"));
        m_labelParameterLeft->show();
        m_labelParameterOperator->show();
        m_labelParameterRight->show();
        m_comboParameterLeft->show();
        m_comboParameterRight->show();
        m_comboOperator->show();
    }
        break;
    default:{
        m_labelParameterLeft->hide();
        m_labelParameterOperator->hide();
        m_labelParameterRight->hide();
        m_comboParameterLeft->hide();
        m_comboParameterRight->hide();

        m_comboOperator->hide();
    }
        break;
    }
}

void RobotProgramProcessWidget::comboSubMenuIOChanged(int){
    updateIO();
    switch(m_comboSubMenuIO->currentIndex()){
    case 0:{
        m_labelParameterLeft->setText(tr("输入端口号"));
        m_labelParameterRight->setText(tr("输入端口状态"));
        m_comboParameterRight->show();
    }
        break;
    case 1:{
        m_labelParameterLeft->setText(tr("输出端口号"));
        m_labelParameterRight->setText(tr("输出端口状态"));
        m_comboParameterRight->show();
    }
        break;
    case 2:{
        m_labelParameterLeft->setText(tr("输出端口号"));
        m_labelParameterRight->setText(tr("输出端口数值(0-10)"));
        m_comboParameterRight->show();
    }
        break;
    default:
        break;
    }
}

void RobotProgramProcessWidget::comboSubMenuTimeChanged(int){
    m_labelParameterLeft->setText(tr("延时时长/s"));
}

void RobotProgramProcessWidget::comboSubMenuComChanged(int){
    m_labelParameterLeft->setText(tr("注释内容"));
}
