#include "robotProgramWidget.h"

using namespace std;
using namespace rclib;

extern bool tag_ADD;

RobotProgramWidget::RobotProgramWidget(QWidget *parent){
    this->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, "#808080");
    this->setPalette(pe);

    initTreeStyle();
    initUI();
    initState();

    connectSignalSlot();
}

RobotProgramWidget::~RobotProgramWidget(){
}

void RobotProgramWidget::initTreeStyle()
{
    s_styleTree = QString("QTreeView::branch:has-siblings:!adjoins-item {\
                          border-image: url(%1) 0;\
                  }\
                  \
                  QTreeView::branch:has-siblings:adjoins-item {\
                                        border-image: url(%2) 0;\
                  }\
                  \
                  QTreeView::branch:!has-children:!has-siblings:adjoins-item {\
                                         border-image: url(%3) 0;\
                  }\
                  \
                  QTreeView::branch:has-children:!has-siblings:closed,\
                                    QTreeView::branch:closed:has-children:has-siblings {\
                                        border-image: none;\
                  image: url(%4);\
                  }\
                  \
                  QTreeView::branch:open:has-children:!has-siblings,\
                                    QTreeView::branch:open:has-children:has-siblings  {\
                                        border-image: none;\
                  image: url(%5);\
                  }\
                  \
                  QTreeView{\
                      font-size:20px;\
                  }").arg(WINDOWICONPATH+QString("vline.png")).arg(WINDOWICONPATH+QString("branch-more.png"))
                  .arg(WINDOWICONPATH+QString("branch-end.png")).arg(WINDOWICONPATH+QString("branch-closed.png"))
                  .arg(WINDOWICONPATH+QString("branch-open.png"));
}


bool RobotProgramWidget::programLoad(){
    return buttonLoadProgramClicked();
}

void RobotProgramWidget::initUI(){
    m_tabProgramTree = new QTabWidget();
    m_tabProgramTree->setMinimumWidth(300);
    m_tabProgramTree->setStyleSheet("QTabBar::tab { height: 30px; width: 80px; }");
    m_tabProgramTree->setCurrentIndex(1);

    m_widgetVariable = new QWidget();
    m_widgetMainFunc = new QWidget();
    m_widgetSubFunc = new QWidget();
    m_widgetVariable->setAutoFillBackground(true);
    m_widgetMainFunc->setAutoFillBackground(true);
    m_widgetSubFunc->setAutoFillBackground(true);
    QPalette pe;
    pe.setColor(QPalette::Background, "#808080");
    m_widgetVariable->setPalette(pe);
    m_widgetMainFunc->setPalette(pe);
    m_widgetSubFunc->setPalette(pe);


    m_treeVariable = new QTreeWidget();
    m_treeMainFunc = new QTreeWidget();
    m_treeSubFunc = new QTreeWidget();
    QHBoxLayout* layoutVariable = new QHBoxLayout(m_widgetVariable);
    layoutVariable->addWidget(m_treeVariable);
    QHBoxLayout* layoutMainFunc = new QHBoxLayout(m_widgetMainFunc);
    layoutMainFunc->addWidget(m_treeMainFunc);
    QHBoxLayout* layoutSubFunc = new QHBoxLayout(m_widgetSubFunc);
    layoutSubFunc->addWidget(m_treeSubFunc);

    m_treeVariable->setHeaderHidden(true);
    m_treeMainFunc->setHeaderHidden(true);
    m_treeSubFunc->setHeaderHidden(true);

    m_treeVariable->setStyleSheet(s_styleTree);
    m_treeMainFunc->setStyleSheet(s_styleTree);
    m_treeSubFunc->setStyleSheet(s_styleTree);

    m_treeVariable->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_treeVariable->header()->setStretchLastSection(false);
    m_treeMainFunc->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_treeMainFunc->header()->setStretchLastSection(false);
    m_treeSubFunc->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_treeSubFunc->header()->setStretchLastSection(false);

    m_tabProgramTree->addTab(m_widgetVariable, tr("变量"));
    m_tabProgramTree->addTab(m_widgetMainFunc, tr("主函数"));
    m_tabProgramTree->addTab(m_widgetSubFunc, tr("子函数"));

    m_labelProgramNamel = new QLabel(tr("未打开文件"));
    m_checkProgramChinese = new QCheckBox(tr("中文编程"));
    m_checkProgramTrack = new QCheckBox(tr("跟踪显示"));

    m_widgetProgramEdit = new QWidget();
    m_widgetProgramEdit->setFixedWidth(500);

    QHBoxLayout* layoutProgramOption = new QHBoxLayout();
    layoutProgramOption->addWidget(m_labelProgramNamel, 0, Qt::AlignLeft);
    layoutProgramOption->addWidget(m_checkProgramChinese, 0, Qt::AlignCenter);
    layoutProgramOption->addWidget(m_checkProgramTrack, 0, Qt::AlignRight);

    QVBoxLayout* layoutProgramList = new QVBoxLayout();
    layoutProgramList->addWidget(m_tabProgramTree);
    layoutProgramList->addLayout(layoutProgramOption);

    QHBoxLayout* layoutProgram = new QHBoxLayout(this);
    layoutProgram->addLayout(layoutProgramList);
    layoutProgram->addWidget(m_widgetProgramEdit);
    layoutProgram->setStretchFactor(layoutProgramList, 1);
    layoutProgram->setStretchFactor(layoutProgramList, 1);

    m_widgetRobotProgramFile = new RobotProgramFileWidget();
    m_widgetRobotProgramVariable = new RobotProgramVariableWidget();
    m_widgetRobotProgramCalculate = new RobotProgramCalculateWidget();
    m_widgetRobotProgramMode = new RobotProgramModeWidget();
    m_widgetRobotProgramMotion = new RobotProgramMotionWidget();
    m_widgetRobotProgramProcess = new RobotProgramProcessWidget();
    m_widgetRobotProgramFrame = new RobotProgramFrameWidget();
    m_widgetRobotProgramSubFunc = new RobotProgramSubFuncWidget();
    m_widgetRobotProgramVision = new RobotProgramVisionWidget();

    m_widgetProgramEditMenu = new QWidget();
    m_widgetProgramEditMenu->resize(m_widgetProgramEdit->size());

    QVBoxLayout* layoutProgramEdit = new QVBoxLayout(m_widgetProgramEdit);
    layoutProgramEdit->addWidget(m_widgetProgramEditMenu);
    layoutProgramEdit->addWidget(m_widgetRobotProgramFile);
    layoutProgramEdit->addWidget(m_widgetRobotProgramVariable);
    layoutProgramEdit->addWidget(m_widgetRobotProgramCalculate);
    layoutProgramEdit->addWidget(m_widgetRobotProgramMode);
    layoutProgramEdit->addWidget(m_widgetRobotProgramMotion);
    layoutProgramEdit->addWidget(m_widgetRobotProgramProcess);
    layoutProgramEdit->addWidget(m_widgetRobotProgramFrame);
    layoutProgramEdit->addWidget(m_widgetRobotProgramSubFunc);
    layoutProgramEdit->addWidget(m_widgetRobotProgramVision);

    m_buttonNewFile = new PushButton(tr("新建文件"));
    m_buttonOpenFile = new PushButton(tr("打开文件"));
    m_buttonSaveFile = new PushButton(tr("保存文件"));
    m_buttonSaveasFile = new PushButton(tr("另存文件"));
    m_buttonUSB = new PushButton(tr("U盘操作"));
    m_buttonNewFile->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonOpenFile->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonSaveFile->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonSaveasFile->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonUSB->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_buttonCopySentence = new PushButton(tr("复制语句"));
    m_buttonPasteSentence = new PushButton(tr("粘贴语句"));
    m_buttonDeleteSentence = new PushButton(tr("删除语句"));
    m_buttonModifySentence = new PushButton(tr("修改语句"));
    m_buttonComSentence = new PushButton(tr("注释语句"));
    m_buttonShearSentence = new PushButton(tr("剪切语句"));
    m_buttonCopySentence->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonPasteSentence->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonDeleteSentence->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonModifySentence->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonComSentence->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonShearSentence->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    m_buttonVariablePage = new PushButton(tr("变量操作"));
    m_buttonCalculatePage = new PushButton(tr("计算指令"));
    m_buttonProcessPage = new PushButton(tr("过程指令"));
    m_buttonMotionPage = new PushButton(tr("运动指令"));
    m_buttonFramePage = new PushButton(tr("坐标系指令"));
    m_buttonAdvanceModePage = new PushButton(tr("模式指令"));
    m_buttonSubFunc = new PushButton(tr("添加子函数"));
    m_buttonVision = new PushButton(tr("视觉指令"));
    m_buttonVariablePage->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonCalculatePage->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonProcessPage->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonMotionPage->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonFramePage->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonAdvanceModePage->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonSubFunc->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonVision->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutFileOperation = new QHBoxLayout();
    layoutFileOperation->addWidget(m_buttonNewFile);
    layoutFileOperation->addWidget(m_buttonOpenFile);
    layoutFileOperation->addWidget(m_buttonSaveFile);
    layoutFileOperation->addWidget(m_buttonSaveasFile);
    layoutFileOperation->addWidget(m_buttonUSB);

    QGroupBox* groupFileOperation = new QGroupBox(tr("文件操作"));
    groupFileOperation->setLayout(layoutFileOperation);

    QHBoxLayout* layoutSentenceOperation1 = new QHBoxLayout();
    layoutSentenceOperation1->addWidget(m_buttonCopySentence);
    layoutSentenceOperation1->addWidget(m_buttonPasteSentence);
    layoutSentenceOperation1->addWidget(m_buttonDeleteSentence);
    QHBoxLayout* layoutSentenceOperation2 = new QHBoxLayout();
    layoutSentenceOperation2->addWidget(m_buttonModifySentence);
    layoutSentenceOperation2->addWidget(m_buttonComSentence);
    layoutSentenceOperation2->addWidget(m_buttonShearSentence);

    QVBoxLayout* layoutSentenceOperation = new QVBoxLayout();
    layoutSentenceOperation->addLayout(layoutSentenceOperation1);
    layoutSentenceOperation->addLayout(layoutSentenceOperation2);

    QGroupBox* groupSentenceOperation = new QGroupBox(tr("语句操作"));
    groupSentenceOperation->setLayout(layoutSentenceOperation);

    QHBoxLayout* layoutAddSentence1 = new QHBoxLayout();
    layoutAddSentence1->addWidget(m_buttonVariablePage);
    layoutAddSentence1->addWidget(m_buttonCalculatePage);
    layoutAddSentence1->addWidget(m_buttonSubFunc);
    layoutAddSentence1->addWidget(m_buttonAdvanceModePage);

    QHBoxLayout* layoutAddSentence2 = new QHBoxLayout();
    layoutAddSentence2->addWidget(m_buttonMotionPage);
    layoutAddSentence2->addWidget(m_buttonProcessPage);
    layoutAddSentence2->addWidget(m_buttonFramePage);
    layoutAddSentence2->addWidget(m_buttonVision);

    QVBoxLayout* layoutAddSentence = new QVBoxLayout();
    layoutAddSentence->addLayout(layoutAddSentence1);
    layoutAddSentence->addLayout(layoutAddSentence2);

    QGroupBox* groupAddSentence = new QGroupBox(tr("添加指令"));
    groupAddSentence->setLayout(layoutAddSentence);

    QVBoxLayout* layoutProgramEditMenu = new QVBoxLayout(m_widgetProgramEditMenu);
    layoutProgramEditMenu->addWidget(groupFileOperation);
    layoutProgramEditMenu->addWidget(groupSentenceOperation);
    layoutProgramEditMenu->addWidget(groupAddSentence);
    layoutProgramEditMenu->setStretchFactor(groupFileOperation, 1);
    layoutProgramEditMenu->setStretchFactor(groupSentenceOperation, 1);
    layoutProgramEditMenu->setStretchFactor(groupAddSentence, 2);

    m_buttonAdvanceModePage->hide();

    m_fileSrc = new rclib::RobotInterpreter();
}

void RobotProgramWidget::initState(){
    m_sentenceCopyedFlag = 0;
    tag_COPY = false;

    QString programName;
    read_cache_program(programName);

    string temp = programName.toStdString();

    if(temp == ""){
        ProgramSentence temp1,temp2;
        temp1.strSentence="MAIN";
        temp2.strSentence="ENDMAIN";

        ProgramPointer m_addres;
        m_addres.nFunction=0;
        m_addres.nSentence=0;

        m_fileSrc->addProgramSentence(m_addres,temp1);

        m_addres.nSentence=1;
        m_fileSrc->addProgramSentence(m_addres,temp2);
    }
    else {
        m_fileName = temp;
        string programPath = string(WINDOWPROGRAMPATH) + m_fileName + string(".program");

        QFileInfo file  = QString::fromStdString(programPath);

        if(file.exists()){
            m_fileSrc->readRobotProgram(programPath.c_str());
        }else{
            m_fileName = "";

            ProgramSentence temp1,temp2;
            temp1.strSentence="MAIN";
            temp2.strSentence="ENDMAIN";

            ProgramPointer m_addres;
            m_addres.nFunction=0;
            m_addres.nSentence=0;

            m_fileSrc->addProgramSentence(m_addres,temp1);

            m_addres.nSentence=1;
            m_fileSrc->addProgramSentence(m_addres,temp2);

            QString name("");
            write_cache_program(name);
        }
    }

    m_widgetRobotProgramFile->passParameter(m_fileSrc,&m_fileName);
    m_widgetRobotProgramVariable->passParameter(m_fileSrc);
    m_widgetRobotProgramCalculate->passParameter(m_fileSrc);
    m_widgetRobotProgramMode->passParameter(m_fileSrc);
    m_widgetRobotProgramMotion->passParameter(m_fileSrc);
    m_widgetRobotProgramProcess->passParameter(m_fileSrc);
    m_widgetRobotProgramFrame->passParameter(m_fileSrc);
    m_widgetRobotProgramSubFunc->passParameter(m_fileSrc);
    m_widgetRobotProgramVision->passParameter(m_fileSrc);

    m_checkProgramChinese->setChecked(isProgramChinese());
    m_checkProgramTrack->setChecked(true);

    m_widgetRobotProgramFile->hide();
    m_widgetRobotProgramVariable->hide();
    m_widgetRobotProgramCalculate->hide();
    m_widgetRobotProgramMode->hide();
    m_widgetRobotProgramMotion->hide();
    m_widgetRobotProgramProcess->hide();
    m_widgetRobotProgramFrame->hide();
    m_widgetRobotProgramSubFunc->hide();
    m_widgetRobotProgramVision->hide();

    updateProgramTree();

    QString tmp("true");

    write_cache_Error(tmp);

    m_tabProgramTree->setCurrentIndex(1);

    m_widgetRobotProgramMotion->resetWayPointRandomIndex();

    m_sentenceEdit.nFunction = 1;
    m_sentenceEdit.nSentence = 0;
}

void RobotProgramWidget::connectSignalSlot(){
    QObject::connect(m_buttonNewFile, SIGNAL(clicked()), this, SLOT(buttonNewFileClicked()));
    QObject::connect(m_buttonOpenFile, SIGNAL(clicked()), this, SLOT(buttonOpenFileClicked()));
    QObject::connect(m_buttonSaveFile, SIGNAL(clicked()), this, SLOT(buttonSaveFileClicked()));
    QObject::connect(m_buttonSaveasFile, SIGNAL(clicked()), this, SLOT(buttonSaveasFileClicked()));
    QObject::connect(m_buttonUSB, SIGNAL(clicked()), this, SLOT(buttonUSBClicked()));
    QObject::connect(m_buttonCopySentence, SIGNAL(clicked()), this, SLOT(buttonCopySentenceClicked()));
    QObject::connect(m_buttonPasteSentence, SIGNAL(clicked()), this, SLOT(buttonPasteSentenceClicked()));
    QObject::connect(m_buttonDeleteSentence, SIGNAL(clicked()), this, SLOT(buttonDeleteSentenceClicked()));
    QObject::connect(m_buttonModifySentence, SIGNAL(clicked()), this, SLOT(buttonModifySentenceClicked()));
    QObject::connect(m_buttonComSentence, SIGNAL(clicked()), this, SLOT(buttonComSentenceClicked()));
    QObject::connect(m_buttonShearSentence, SIGNAL(clicked()), this, SLOT(buttonShearSentenceClicked()));
    QObject::connect(m_buttonVariablePage, SIGNAL(clicked()), this, SLOT(buttonVariablePageClicked()));
    QObject::connect(m_buttonCalculatePage, SIGNAL(clicked()), this, SLOT(buttonCalculatePageClicked()));
    QObject::connect(m_buttonProcessPage, SIGNAL(clicked()), this, SLOT(buttonProcessPageClicked()));
    QObject::connect(m_buttonMotionPage, SIGNAL(clicked()), this, SLOT(buttonMotionPageClicked()));
    QObject::connect(m_buttonFramePage, SIGNAL(clicked()), this, SLOT(buttonFramePageClicked()));
    QObject::connect(m_buttonAdvanceModePage, SIGNAL(clicked()), this, SLOT(buttonAdvanceModePageClicked()));
    QObject::connect(m_buttonSubFunc, SIGNAL(clicked()), this, SLOT(buttonSubFuncPageClicked()));
    QObject::connect(m_buttonVision, SIGNAL(clicked()), this, SLOT(buttonVisionPageClicked()));

    QObject::connect(m_widgetRobotProgramFile, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));
    QObject::connect(m_widgetRobotProgramVariable, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));
    QObject::connect(m_widgetRobotProgramCalculate, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));
    QObject::connect(m_widgetRobotProgramMode, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));
    QObject::connect(m_widgetRobotProgramMotion, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));
    QObject::connect(m_widgetRobotProgramProcess, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));
    QObject::connect(m_widgetRobotProgramFrame, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));
    QObject::connect(m_widgetRobotProgramSubFunc, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));
    QObject::connect(m_widgetRobotProgramVision, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));

    QObject::connect(this, SIGNAL(programOperationFinishedSignal(bool)), this, SLOT(programOperationFinished(bool)));

    QObject::connect(m_widgetRobotProgramFile, SIGNAL(showRecomInfoSignal(QString)), this, SLOT(showRecomInfoSlot(QString)));

    QObject::connect(m_checkProgramChinese, SIGNAL(stateChanged(int)), this, SLOT(checkProgramChineseChanged()));

    QObject::connect(m_widgetRobotProgramSubFunc, SIGNAL(editSubSignal()), this, SLOT(findNextSub()));
}


void RobotProgramWidget::checkProgramChineseChanged(){
    setProgramChinese(m_checkProgramChinese->checkState());
    updateProgramTree();
}

void RobotProgramWidget::buttonNewFileClicked(){
    m_widgetRobotProgramFile->setMode(m_widgetRobotProgramFile->TEACH_NEWFILE);
    m_widgetRobotProgramFile->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramMotion->resetWayPointRandomIndex();
}

void RobotProgramWidget::buttonOpenFileClicked(){
    m_widgetRobotProgramFile->setMode(m_widgetRobotProgramFile->TEACH_OPENFILE);
    m_widgetRobotProgramFile->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramMotion->resetWayPointRandomIndex();
}

void RobotProgramWidget::buttonSaveFileClicked(){
    if(m_fileName.empty()){
        m_widgetRobotProgramFile->setMode(m_widgetRobotProgramFile->TEACH_SAVEFILE);
        m_widgetRobotProgramFile->show();
        m_widgetProgramEditMenu->hide();
    }
    else{ 
        m_fileSrc->writeRobotProgram((string(WINDOWPROGRAMPATH) + m_fileName + string(".program")).c_str());

        QString programName = QString::fromStdString(m_fileName);
        write_cache_program(programName);
    }
}

void RobotProgramWidget::buttonSaveasFileClicked(){
    m_widgetRobotProgramFile->setMode(m_widgetRobotProgramFile->TEACH_SAVEFILE);
    m_widgetRobotProgramFile->show();
    m_widgetProgramEditMenu->hide();
}

void RobotProgramWidget::buttonUSBClicked(){
    m_widgetRobotProgramFile->setMode(m_widgetRobotProgramFile->TEACH_USB);
    m_widgetRobotProgramFile->show();
    m_widgetProgramEditMenu->hide();
}

void RobotProgramWidget::buttonCopySentenceClicked(){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("MAINFUNC") || keyword == string("MAIN") || keyword == string("ENDMAIN") ||
            keyword == string("SUBFUNC") || keyword == string("FUNC") || keyword == string("ENDFUNC")
            || keyword == string("VALUE")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);

    if(tag_COPY == false){
        m_sentenceCopyed=m_fileSrc->getProgramSentence(mapIndex);
        vectorCopy.clear();
        vectorCopy.push_back(m_sentenceCopyed);

        mapBegin = mapIndex;

        m_sentenceCopyedFlag = 1;
        tag_COPY = true;
    }else{
        mapEnd = mapIndex;

        if(mapBegin.nFunction != mapEnd.nFunction){
            m_sentenceCopyedFlag = 0;
            tag_COPY = false;
            vectorCopy.clear();

            return;
        }

        ProgramPointer tmp;
        tmp.nFunction = mapBegin.nFunction;
        for(int i = mapBegin.nSentence + 1 ; i <= mapEnd.nSentence ; i++){
            tmp.nSentence = i;
            m_sentenceCopyed=m_fileSrc->getProgramSentence(tmp);
            vectorCopy.push_back(m_sentenceCopyed);
        }

        m_sentenceCopyedFlag = 1;
        tag_COPY = false;
    }
    updateProgramTree();
    highlight(m_sentenceEdit.nFunction, m_sentenceEdit.nSentence);
}

void RobotProgramWidget::buttonPasteSentenceClicked(){
    if(m_sentenceCopyedFlag == 0){
        QMessageBox::question(this, tr("提示"),
                              tr("请先复制一条语句!"),
                              QMessageBox::Yes);
        return;
    }

    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("MAINFUNC") || keyword == string("ENDMAIN")
            || keyword == string("SUBFUNC") || keyword == string("ENDFUNC")
            || keyword == string("VALUE")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);
    mapIndex.nSentence=mapIndex.nSentence+1;

    for(auto it = vectorCopy.begin() ; it != vectorCopy.end() ; it++){
        m_sentenceCopyed = it->strSentence;
        m_fileSrc->addProgramSentence(mapIndex,m_sentenceCopyed);
        mapIndex.nSentence=mapIndex.nSentence+1;
    }

    m_sentenceCopyedFlag = 0;
    tag_COPY = false;
    vectorCopy.clear();

    updateProgramTree();
    findNextProgram(m_sentenceEdit.nFunction, m_sentenceEdit.nSentence);
    highlight(m_sentenceEdit.nFunction, m_sentenceEdit.nSentence);
}

void RobotProgramWidget::buttonDeleteSentenceClicked(){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("MAINFUNC") || keyword == string("MAIN") || keyword == string("ENDMAIN")
            || keyword == string("SUBFUNC")){
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("提示"),
                                    tr("确定删除该语句？"),
                                    QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
        return;

    PROGRAM_KEY_WORD keyvalue = m_fileSrc->getKeyWordIndex()[keyword];

    if(keyvalue == VALUE){
        if(m_tabProgramTree->currentIndex() == 0){
            string name;
            getVariableName(name);

            m_fileSrc->delVariable(name);
        }
    }
    else if(keyvalue == FUNC){
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        int m_func = mapIndex.nFunction;
        int m_line = mapIndex.nSentence;

        vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();
        VECTOR_STRING tmp_FuncStrentence=Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");
        string name = tmp_FuncStrentence[1];

        m_fileSrc->deleteFunction(name);
    }
    else{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_fileSrc->deleteProgramSentence(mapIndex);
    }

    m_sentenceEdit.nFunction = 1;
    m_sentenceEdit.nSentence = 0;

    updateProgramTree();
}

void RobotProgramWidget::buttonModifySentenceClicked(){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("MAINFUNC") || keyword == string("MAIN") || keyword == string("ENDMAIN")
            || keyword == string("SUBFUNC") || keyword == string("ENDFUNC")){
        return;
    }

    switch(m_fileSrc->getKeyWordIndex()[keyword]){
    case VALUE:{
        string name;
        getVariableName(name);

        m_widgetRobotProgramVariable->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramVariable->passVariableName(name);
        m_widgetRobotProgramVariable->setMode(RobotProgramVariableWidget::TEACH_MODIFY);
    }
        break;
    case FUNC:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramSubFunc->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramSubFunc->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramSubFunc->setMode(RobotProgramSubFuncWidget::TEACH_MODIFY);
    }
        break;
    case CALCU:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramCalculate->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramCalculate->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramCalculate->setMode(RobotProgramCalculateWidget::TEACH_MODIFY);
    }
        break;
    case FOR:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case WHILE:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case IF:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case WAITDIN:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case DOUT:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case AOUT:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case DELAY:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case COM:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case CALL:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramProcess->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_MODIFY);
    }
        break;
    case VLOCATE:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramVision->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramVision->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramVision->setMode(RobotProgramVisionWidget::TEACH_MODIFY);
    }
        break;
    case TOOL:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramFrame->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramFrame->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramFrame->setMode(RobotProgramFrameWidget::TEACH_MODIFY);
    }
        break;
    case WORK:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramFrame->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramFrame->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramFrame->setMode(RobotProgramFrameWidget::TEACH_MODIFY);
    }
        break;
    case MOVEABSJ:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMotion->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_MODIFY);
    }
        break;
    case MOVEABSJR:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMotion->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_MODIFY);
    }
        break;
    case MOVEJ:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMotion->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_MODIFY);
    }
        break;
    case MOVEJR:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMotion->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_MODIFY);
    }
        break;
    case MOVEL:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMotion->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_MODIFY);
    }
        break;
    case MOVELR:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMotion->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_MODIFY);
    }
        break;
    case MOVEC:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMotion->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_MODIFY);
    }
        break;
    case MOVECR:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMotion->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_MODIFY);
    }
        break;
    case MOVEB:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMode->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMode->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMode->setMode(RobotProgramModeWidget::TEACH_MODIFY);
    }
        break;
    case MOVEJT:{
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        m_widgetRobotProgramMode->show();
        m_widgetProgramEditMenu->hide();
        m_widgetRobotProgramMode->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
        m_widgetRobotProgramMode->setMode(RobotProgramModeWidget::TEACH_MODIFY);
    }
        break;
    default:
        break;
    }
}

void RobotProgramWidget::buttonComSentenceClicked()
{
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("MAINFUNC") || keyword == string("MAIN") || keyword == string("ENDMAIN")
            || keyword == string("SUBFUNC") || keyword == string("FUNC") || keyword == string("ENDFUNC")
            || keyword == string("VALUE") ){
        return;
    }

    PROGRAM_KEY_WORD keyvalue = m_fileSrc->getKeyWordIndex()[keyword];

    ProgramPointer tmp_Pointer;
    getSentenceIndex(tmp_Pointer);

    int m_func = tmp_Pointer.nFunction;
    int m_line = tmp_Pointer.nSentence;

    string tempSentence = "";
    ProgramSentence tmp_Strentence;

    vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();

    if(keyvalue == COM){
        VECTOR_STRING tmp_FuncStrentence=Common::ToVectors(tmp_FuncTable[m_func][m_line].strSentence," ");
        for(int i = 1;i<tmp_FuncStrentence.size();i++){
            if(i == tmp_FuncStrentence.size() - 1)
                tempSentence += tmp_FuncStrentence[i];
            else
                tempSentence += tmp_FuncStrentence[i] + " ";
        }
        tmp_Strentence.strSentence = tempSentence;
        m_fileSrc->modifyProgramSentence(tmp_Pointer,tmp_Strentence);
    }
    else{
        tempSentence = string("COM")+ " " + tmp_FuncTable[m_func][m_line].strSentence;
        tmp_Strentence.strSentence = tempSentence;
        m_fileSrc->modifyProgramSentence(tmp_Pointer,tmp_Strentence);
    }

    updateProgramTree();

    highlight(m_sentenceEdit.nFunction, m_sentenceEdit.nSentence);
}

void RobotProgramWidget::buttonShearSentenceClicked()
{
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("MAINFUNC") || keyword == string("MAIN") || keyword == string("ENDMAIN") ||
            keyword == string("SUBFUNC") || keyword == string("FUNC") || keyword == string("ENDFUNC")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);

    m_sentenceCopyed=m_fileSrc->getProgramSentence(mapIndex);
    vectorCopy.clear();
    vectorCopy.push_back(m_sentenceCopyed);

    m_sentenceCopyedFlag = 1;
    getSentenceIndex(mapIndex);
    m_fileSrc->deleteProgramSentence(mapIndex);

    programOperationFinishedSignal(true);
}

bool RobotProgramWidget::buttonLoadProgramClicked(){
    m_fileSrc->writeRobotProgram((string(WINDOWPROGRAMPATH) + m_fileName + string(".program")).c_str());
    RobotClient::initance()->programLoad((string(WINDOWPROGRAMPATH) + m_fileName + string(".program")).c_str());

    return true;
}

void RobotProgramWidget::buttonVariablePageClicked(){
    m_widgetRobotProgramVariable->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramVariable->setMode(RobotProgramVariableWidget::TEACH_ADD);
}

void RobotProgramWidget::buttonSubFuncPageClicked(){
    m_widgetRobotProgramSubFunc->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramSubFunc->setMode(RobotProgramSubFuncWidget::TEACH_ADD);
}

void RobotProgramWidget::buttonCalculatePageClicked( ){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("VALUE") || keyword == string("MAINFUNC") || keyword == string("ENDMAIN") || keyword == string("SUBFUNC") || keyword == string("ENDFUNC")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);

    m_widgetRobotProgramCalculate->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramCalculate->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
    m_widgetRobotProgramCalculate->setMode(RobotProgramCalculateWidget::TEACH_ADD);
}

void RobotProgramWidget::buttonProcessPageClicked(){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("VALUE") || keyword == string("MAINFUNC") || keyword == string("ENDMAIN") || keyword == string("SUBFUNC") || keyword == string("ENDFUNC")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);

    m_widgetRobotProgramProcess->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramProcess->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
    m_widgetRobotProgramProcess->setMode(RobotProgramProcessWidget::TEACH_ADD);
}

void RobotProgramWidget::buttonMotionPageClicked(){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("VALUE") || keyword == string("MAINFUNC") || keyword == string("ENDMAIN") || keyword == string("SUBFUNC") || keyword == string("ENDFUNC")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);

    m_widgetRobotProgramMotion->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramMotion->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
    m_widgetRobotProgramMotion->setMode(RobotProgramMotionWidget::TEACH_ADD);
}

void RobotProgramWidget::buttonFramePageClicked(){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("VALUE") || keyword == string("MAINFUNC") || keyword == string("ENDMAIN") || keyword == string("SUBFUNC") || keyword == string("ENDFUNC")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);  //指针

    m_widgetRobotProgramFrame->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramFrame->passSentenceIndex(mapIndex.nFunction,mapIndex.nSentence);
    m_widgetRobotProgramFrame->setMode(RobotProgramFrameWidget::TEACH_ADD);
}

void RobotProgramWidget::buttonAdvanceModePageClicked(){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("VALUE") || keyword == string("MAINFUNC") || keyword == string("ENDMAIN") || keyword == string("SUBFUNC") || keyword == string("ENDFUNC")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);

    m_widgetRobotProgramMode->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramMode->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
    m_widgetRobotProgramMode->setMode(RobotProgramModeWidget::TEACH_ADD);
}

void RobotProgramWidget::buttonVisionPageClicked(){
    string keyword;
    if(!getSentenceKeyWord(keyword)){
        return;
    }

    if(keyword == string("VALUE") || keyword == string("MAINFUNC") || keyword == string("ENDMAIN") || keyword == string("SUBFUNC") || keyword == string("ENDFUNC")){
        return;
    }

    ProgramPointer mapIndex;
    getSentenceIndex(mapIndex);

    m_widgetRobotProgramVision->show();
    m_widgetProgramEditMenu->hide();
    m_widgetRobotProgramVision->passSentenceIndex(mapIndex.nFunction, mapIndex.nSentence);
    m_widgetRobotProgramVision->setMode(RobotProgramVisionWidget::TEACH_ADD);
}

void RobotProgramWidget::programOperationFinished(bool state){
    m_widgetRobotProgramFile->hide();
    m_widgetRobotProgramVariable->hide();
    m_widgetRobotProgramCalculate->hide();
    m_widgetRobotProgramMode->hide();
    m_widgetRobotProgramMotion->hide();
    m_widgetRobotProgramProcess->hide();
    m_widgetRobotProgramFrame->hide();
    m_widgetRobotProgramSubFunc->hide();
    m_widgetRobotProgramVision->hide();
    m_widgetProgramEditMenu->show();
    if(state){
        updateProgramTree();
        if(tag_ADD){
            findNextProgram(m_sentenceEdit.nFunction, m_sentenceEdit.nSentence);
        }
    }

    highlight(m_sentenceEdit.nFunction, m_sentenceEdit.nSentence);
}

void RobotProgramWidget:: updateProgramTree(){
    if(!m_fileName.empty())
    {
        m_labelProgramNamel->setText(QString::fromStdString(m_fileName));
    }

    m_treeVariable->clear();
    m_treeMainFunc->clear();
    m_treeSubFunc->clear();

    for(int i=0; i<1; i++){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        if(isProgramChinese())
            item->setText(0, "变量");
        else
            item->setText(0, "VALUE");
        m_treeVariable->addTopLevelItem(item);
    }

    for(int i=0; i<1; i++){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        if(isProgramChinese())
            item->setText(0, "主函数");
        else
            item->setText(0, "MAINFUNC");
        m_treeMainFunc->addTopLevelItem(item);
    }

    for(int i=0; i<1; i++){
        QTreeWidgetItem* item = new QTreeWidgetItem();\
        if(isProgramChinese())
            item->setText(0, "子函数");
        else
            item->setText(0, "SUBFUNC");
        m_treeSubFunc->addTopLevelItem(item);
    }

    QTreeWidgetItem* currentItem =  m_treeVariable->topLevelItem(0);

    std::map<std::string, PROGRAM_KEY_WORD> tmp_VarType= m_fileSrc->getVariableMap();
    map<string, PROGRAM_KEY_WORD>::iterator it_varType = tmp_VarType.begin();
    while(it_varType!= tmp_VarType.end()){
        QTreeWidgetItem* item = new QTreeWidgetItem();
        int KeyWord=it_varType->second;
        QString str;
        switch (KeyWord) {
        case 1:
            if(isProgramChinese())
                str = "布尔型";
            else
                str = "BOOL";
            break;
        case 2:
            if(isProgramChinese())
                str = "整型";
            else
                str = "INT";
            break;
        case 3:
            if(isProgramChinese())
                str = "双精度型";
            else
                str = "DOUBLE";
            break;
        case 4:
            if(isProgramChinese())
                str = "关节型";
            else
                str = "JOINT";
            break;
        case 5:
            if(isProgramChinese())
                str = "末端型";
            else
                str = "TERMINAL";
            break;
        case 28:
            if(isProgramChinese())
                str = "坐标型";
            else
                str = "FRAME";
            break;
        default:
            break;
        }

        item->setText(0,str + QString(" ") + QString::fromStdString(it_varType->first));
        string tmp  = it_varType->first;
        if(tmp == "VPOS1" || tmp == "VPOS2" || tmp == "VPOS3"){
            it_varType++;
            continue;
        }

        currentItem->addChild(item);
        it_varType++;
    }

    m_treeLine.clear();
    m_mapLine.clear();
    ProgramPointer treeIndex;
    ProgramPointer mapIndex;

    std::vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();

    for(int i=0; i<tmp_FuncTable.size(); i++){
        if(i==0){
            currentItem = m_treeMainFunc->topLevelItem(0);
        }
        else{
            currentItem = m_treeSubFunc->topLevelItem(0);
        }

        ProgramPointer m_addres;
        m_addres.nFunction=0;
        m_addres.nSentence=0;


        for(int j=0; j<tmp_FuncTable[i].size(); j++){
            VECTOR_STRING temp_Vector;
            string temp="";
            QString text="";

            temp_Vector=Common::ToVectors(tmp_FuncTable[i][j].strSentence," ");

            temp=temp_Vector[0];

            PROGRAM_KEY_WORD keyWord;
            map<std::string, PROGRAM_KEY_WORD> tmp_KeyWord=m_fileSrc->getKeyWordIndex();
            keyWord = tmp_KeyWord[temp];

            if(keyWord == MAIN){
                QTreeWidgetItem* item = new QTreeWidgetItem();

                if(isProgramChinese())
                    text = "MAIN函数";
                else
                    text = "MAIN";
                item->setText(0, text);
                currentItem->addChild(item);

                currentItem = item;

                treeIndex.nFunction = 1;
                treeIndex.nSentence = 2;
                mapIndex.nFunction = 0;
                mapIndex.nSentence = 0;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
            }
            else if(keyWord == FUNC || keyWord == FOR || keyWord == WHILE || keyWord == IF){
                QTreeWidgetItem* item = new QTreeWidgetItem();
                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();
                if(isProgramChinese())
                {
                    if(!strcmp(temp,"FUNC"))
                        text="函数";
                    else if(!strcmp(temp,"FOR"))
                        text="循环";
                    else if(!strcmp(temp,"WHILE"))
                        text="条件循环";
                    else if(!strcmp(temp,"IF"))
                        text="判断";
                }
                text.append(" ");
                for(auto it=temp_Vector.begin()+1;it<temp_Vector.end();it++)
                {
                    text.append(QString::fromStdString(*it));
                    text.append(" ");
                }

                item->setText(0, text);
                currentItem->addChild(item);

                currentItem = item;

                if(keyWord == FUNC){
                    treeIndex.nFunction = 2;
                    if(i == 1){
                        treeIndex.nSentence = 2;
                    }
                    else{
                        treeIndex.nSentence += 1;
                    }
                    mapIndex.nFunction += 1;
                    mapIndex.nSentence = 0;
                }
                else{
                    treeIndex.nSentence += 1;
                    mapIndex.nSentence += 1;
                }
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
            }
            else if(keyWord == ELSEIF || keyWord == ELSE){
                if(currentItem->parent()){
                    currentItem = currentItem->parent();
                }

                QTreeWidgetItem* item = new QTreeWidgetItem();

                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();
                if(isProgramChinese())
                {
                    if(!strcmp(temp,"ELSEIF"))
                        text="或者判断";
                    else if(!strcmp(temp,"ELSE"))
                        text="继续判断";
                }
                text.append(" ");
                for(auto it=temp_Vector.begin()+1;it<temp_Vector.end();it++)
                {
                    text.append(QString::fromStdString(*it));
                    text.append(" ");
                }

                item->setText(0, text);
                currentItem->addChild(item);

                currentItem = item;

                treeIndex.nSentence += 1;
                mapIndex.nSentence += 1;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
            }
            else if(keyWord == ENDMAIN || keyWord == ENDFUNC || keyWord == ENDFOR || keyWord == ENDWHILE || keyWord == ENDIF){
                if(currentItem->parent()){
                    currentItem = currentItem->parent();
                }

                QTreeWidgetItem* item = new QTreeWidgetItem();

                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();
                if(isProgramChinese())
                {
                    if(!strcmp(temp,"ENDMAIN"))
                        text="结束MAIN函数";
                    else if(!strcmp(temp,"ENDFUNC"))
                        text="结束函数";
                    if(!strcmp(temp,"ENDFOR"))
                        text="结束循环";
                    else if(!strcmp(temp,"ENDWHILE"))
                        text="结束条件循环";
                    else if(!strcmp(temp,"ENDIF"))
                        text="结束判断";
                }
                text.append(" ");
                for(auto it=temp_Vector.begin()+1;it<temp_Vector.end();it++)
                {
                    text.append(QString::fromStdString(*it));
                    text.append(" ");
                }

                item->setText(0, text);
                currentItem->addChild(item);

                treeIndex.nSentence += 1;
                mapIndex.nSentence += 1;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
            }
            else if(keyWord >= MOVEABSJ && keyWord <= MOVEABSJR){
                QTreeWidgetItem* item = new QTreeWidgetItem();

                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();
                if(isProgramChinese())
                {
                    if(!strcmp(temp,"MOVABSJ"))
                        text="ABS关节 ";
                    else if(!strcmp(temp,"MOVABSJR"))
                        text="相对ABS关节 ";
                }

                item->setText(0, text);
                currentItem->addChild(item);

                currentItem = item;

                for(int k=2;k<temp_Vector.size();k++)
                {
                    QTreeWidgetItem* item1 = new QTreeWidgetItem();
                    QString text1 = "";
                    if(isProgramChinese())
                    {
                        text1.append(QString("关节型 ") + QString::fromStdString(temp_Vector[k]));
                    }
                    else
                    {
                        text1.append(QString("JOINT ") + QString::fromStdString(temp_Vector[k]));
                    }

                    item1->setText(0, text1);
                    currentItem->addChild(item1);
                }

                if(currentItem->parent()){
                    currentItem = currentItem->parent();
                }

                treeIndex.nSentence += 1;
                mapIndex.nSentence += 1;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
                treeIndex.nSentence += temp_Vector.size() - 2;
            }
            else if(keyWord >= MOVEJ && keyWord <= MOVEJR){
                QTreeWidgetItem* item = new QTreeWidgetItem();

                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();
                if(isProgramChinese())
                {
                    if(!strcmp(temp,"MOVJ"))
                        text="关节 ";
                    else if(!strcmp(temp,"MOVJR"))
                        text="相对关节 ";
                }

                item->setText(0, text);
                currentItem->addChild(item);

                currentItem = item;

                for(int k=3;k<temp_Vector.size();k++)
                {
                QTreeWidgetItem* item1 = new QTreeWidgetItem();
                QString text1 = "";
                if(isProgramChinese())
                {
                    text1.append(QString("末端型 ") + QString::fromStdString(temp_Vector[k]));
                }
                else
                {
                    text1.append(QString("TERMINAL ") + QString::fromStdString(temp_Vector[k]));
                }

                item1->setText(0, text1);
                currentItem->addChild(item1);
                }

                if(currentItem->parent()){
                    currentItem = currentItem->parent();
                }

                treeIndex.nSentence += 1;
                mapIndex.nSentence += 1;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
                treeIndex.nSentence += temp_Vector.size() - 3;
            }
            else if(keyWord >= MOVEL && keyWord <= MOVELR){
                QTreeWidgetItem* item = new QTreeWidgetItem();

                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();
                if(isProgramChinese())
                {
                    if(!strcmp(temp,"MOVL"))
                        text="直线 ";
                    else if(!strcmp(temp,"MOVLR"))
                        text="相对直线 ";
                }

                item->setText(0, text);
                currentItem->addChild(item);

                currentItem = item;

                QTreeWidgetItem* item1 = new QTreeWidgetItem();
                QString text1 = "";

                if(isProgramChinese())
                {
                    text1.append(QString("末端型 ") + QString::fromStdString(temp_Vector[6]));
                }
                else
                {
                    text1.append(QString("TERMINAL ") + QString::fromStdString(temp_Vector[6]));
                }

                item1->setText(0, text1);
                currentItem->addChild(item1);

                if(currentItem->parent()){
                    currentItem = currentItem->parent();
                }

                treeIndex.nSentence += 1;
                mapIndex.nSentence += 1;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
                treeIndex.nSentence += temp_Vector.size() - 6;
            }
            else if(keyWord >= MOVEC && keyWord <= MOVECR){
                QTreeWidgetItem* item = new QTreeWidgetItem();

                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();
                if(isProgramChinese())
                {
                    if(!strcmp(temp,"MOVC"))
                        text="圆弧 ";
                    else if(!strcmp(temp,"MOVCR"))
                        text="相对圆弧 ";
                }

                item->setText(0, text);
                currentItem->addChild(item);

                currentItem = item;

                for(int k=7;k<temp_Vector.size();k++)
                {
                QTreeWidgetItem* item1 = new QTreeWidgetItem();
                QString text1 = "";          
                if(isProgramChinese())
                {
                    text1.append(QString("末端型 ") + QString::fromStdString(temp_Vector[k]));
                }
                else
                {
                    text1.append(QString("TERMINAL ") + QString::fromStdString(temp_Vector[k]));
                }
                item1->setText(0, text1);
                currentItem->addChild(item1);
                }

                if(currentItem->parent()){
                    currentItem = currentItem->parent();
                }

                treeIndex.nSentence += 1;
                mapIndex.nSentence += 1;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
                treeIndex.nSentence += temp_Vector.size() - 7;
            }
            else if(keyWord >= MOVEB && keyWord <= MOVEJT){
                QTreeWidgetItem* item = new QTreeWidgetItem();

                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();
                if(isProgramChinese())
                {
                    if(!strcmp(temp,"MOVB"))
                        text="曲线 ";
                    else if(!strcmp(temp,"MOVJT"))
                        text="关节时间 ";
                }

                item->setText(0, text);
                currentItem->addChild(item);

                treeIndex.nSentence += 1;
                mapIndex.nSentence += 1;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
            }
            else{
                QTreeWidgetItem* item = new QTreeWidgetItem();

                text=QString::fromStdString(temp_Vector[0]);
                const char* temp=text.toStdString().c_str();

                if(isProgramChinese())
                {
                    if(!strcmp(temp,"RETURN"))
                        text="返回";
                    else if(!strcmp(temp,"CALL"))
                        text="调用";
                    else if(!strcmp(temp,"CALCU"))
                        text="计算";
                    else if(!strcmp(temp,"DELAY"))
                        text="延时";
                    else if(!strcmp(temp,"WAITDIN"))
                        text="等待数字输入";
                    else if(!strcmp(temp,"DOUT"))
                        text="数字输出";
                    else if(!strcmp(temp,"AOUT"))
                        text="模拟输出";
                    else if(!strcmp(temp,"COM"))
                        text="注释";
                    else if(!strcmp(temp,"BOOL"))
                        text="布尔型";
                    else if(!strcmp(temp,"INT"))
                        text="整型";
                    else if(!strcmp(temp,"DOUBLE"))
                        text="双精度型";
                    else if(!strcmp(temp,"JOINT"))
                        text="关节型";
                    else if(!strcmp(temp,"TERMINAL"))
                        text="末端型";
                    else if(!strcmp(temp,"ENDFOR"))
                        text="结束循环";
                    else if(!strcmp(temp,"TOOLFRAME"))
                        text="修改工具";
                    else if(!strcmp(temp,"USERFRAME"))
                        text="修改工件";
                    else if(!strcmp(temp,"VLOCATE"))
                        text="视觉定位";
                    else if(!strcmp(temp,"TOOL"))
                        text="工具坐标系";
                    else if(!strcmp(temp,"WORK"))
                        text="工件坐标系";
                    else if(!strcmp(temp,"VALUE"))
                        text="变量";
                    else if(!strcmp(temp,"FRAME"))
                        text="坐标系";
                    else if(!strcmp(temp,"CONTINUE"))
                        text="继续";
                    else if(!strcmp(temp,"BREAK"))
                        text="跳出";

                }
                text.append(" ");

                for(int k=1;k<temp_Vector.size();k++)
                {

                    text.append(QString::fromStdString(temp_Vector[k]));
                    text.append(" ");
                }

                item->setText(0, text);
                currentItem->addChild(item);

                treeIndex.nSentence += 1;
                mapIndex.nSentence += 1;
                m_treeLine.push_back(treeIndex);
                m_mapLine.push_back(mapIndex);
            }
        }
    }

    m_treeVariable->expandAll();
    m_treeMainFunc->expandAll();
    m_treeSubFunc->expandAll();

    m_mainTreeItemCount = 0;
    m_subTreeItemCount = 0;
    for(int i=0; i<1; i++){
        QTreeWidgetItemIterator it(m_treeMainFunc);
        while(*it){
            m_mainTreeItemCount++;
            it++;
        }
    }
    for(int i=0; i<1; i++){
        QTreeWidgetItemIterator it(m_treeSubFunc);
        while(*it){
            m_subTreeItemCount++;
            it++;
        }
    }  
}

void RobotProgramWidget::passState(rclib::SYSPLAYSTATE playStateFlag,SWITCHSTATE controlStateFlag)
{
    local_playStateFlag = playStateFlag;
    local_controlStateFlag = controlStateFlag;
}

void RobotProgramWidget::findNextSub()
{
    m_widgetRobotProgramSubFunc->hide();
    m_widgetProgramEditMenu->show();
    updateProgramTree();

    int i = 0;
    for (auto c:m_mapLine)
    {
        if((c.nFunction == (m_fileSrc->getFuncIndex().size() - 1))&&(c.nSentence == 0)){
            m_sentenceEdit.nFunction = m_treeLine[i].nFunction;
            m_sentenceEdit.nSentence = m_treeLine[i].nSentence;
            break;
        }
        i++;
    }

    highlight(m_sentenceEdit.nFunction, m_sentenceEdit.nSentence);
}

void RobotProgramWidget::updateSystemState(){
    if(local_controlStateFlag == SWITCHON && local_playStateFlag == SYSPLAY_TEACH){
        updateProgramEditMenuState(true);
    }
    else{
        updateProgramEditMenuState(false);
    }
}

void RobotProgramWidget::updateRobotProgramWidget(){
    ProgramPointer now = RobotClient::initance()->getProgramPointer();

    int func = 0, line = 0;

    int i = 0;
    for (auto c:m_mapLine)
    {
        if((c.nFunction == now.nFunction)&&(c.nSentence == now.nSentence)){
            func = m_treeLine[i].nFunction;
            line = m_treeLine[i].nSentence;
            break;
        }
        i++;
    }

    highlight(func, line);
}

void RobotProgramWidget::highlight(int func, int line)
{
    line--;

    for(int i=0; i<1; i++){
        QTreeWidgetItem *item;
        m_tabProgramTree->setCurrentIndex(last_func);
        if(last_func == 1){
            item = m_treeMainFunc->topLevelItem(0);

            if( last_sentence < 0 || last_sentence >= (m_mainTreeItemCount-1)){
                break;
            }
        }
        else if(last_func == 2) {
            item = m_treeSubFunc->topLevelItem(0);

            if( last_sentence < 0 || last_sentence >= (m_subTreeItemCount-1)){
                break;
            }
        }
        else{
            break;
        }

        QTreeWidgetItemIterator it(item, QTreeWidgetItemIterator::All);

        it += last_sentence;
        item = *it;
        item->setBackground(0,QColor(255,255,255,0));
    }

    for(int i=0; i<1; i++){
        QTreeWidgetItem *item;
        if(func == 1){
            item = m_treeMainFunc->topLevelItem(0);

            if(line < 0 || line >= (m_mainTreeItemCount-1)){
                break;
            }

        }
        else if(func == 2){
            item = m_treeSubFunc->topLevelItem(0);

            if(line < 0 || line >= (m_subTreeItemCount-1)){
                break;
            }

        }
        else{
            break;
        }

        QTreeWidgetItemIterator it(item, QTreeWidgetItemIterator::All);
        it += line;

        item = *it;
        item->setBackground(0,QColor(255,255,0));

        if(m_checkProgramTrack->isChecked()){
            if(m_tabProgramTree->currentIndex() != func){
                m_tabProgramTree->setCurrentIndex(func);
            }
            if(func == 1){
                m_treeMainFunc->scrollToItem(*it);
            }
            else if(func == 2){
                m_treeSubFunc->scrollToItem(*it);
            }
        }
    }

    last_func = func;
    last_sentence = line;
}

void RobotProgramWidget::updateWidget()
{
    programLoad();
    updateProgramTree();
}

void RobotProgramWidget::setBreakPoint(bool jud)
{
    int nFunction = 0;
    int nSentence = 0;
    if(jud){
        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

        nFunction = mapIndex.nFunction;
        nSentence = mapIndex.nSentence;

        int func = 0, line = 0;

        int i = 0;
        for (auto c:m_mapLine)
        {
            if((c.nFunction == nFunction)&&(c.nSentence == nSentence)){
                func = m_treeLine[i].nFunction;
                line = m_treeLine[i].nSentence;
                break;
            }
            i++;
        }

        highlight(func, line);
    }else{
        ProgramPointer now = RobotClient::initance()->getProgramPointer();
        nFunction = now.nFunction;
        nSentence = now.nSentence;
        nSentence--;
        if(nSentence < 1){
            nFunction = 0;
            nSentence = 0;
        }
        std::vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();
        string keyword = Common::ToVectors(tmp_FuncTable[nFunction][nSentence].strSentence," ")[0];
        if(keyword == string("ENDFOR")){
            findBegin(nFunction,nSentence,string("FOR"));
        }
        if(keyword == string("ENDWHILE")){
            findBegin(nFunction,nSentence,string("WHILE"));
        }
        if((keyword == string("ENDIF") || (keyword == string("ELSE")) || (keyword == string("ELSEIF")))){
            findBegin(nFunction,nSentence,string("IF"));
        }
        if((keyword == string("FOR") || (keyword == string("WHILE")) || (keyword == string("IF")))){
            nSentence++;
        }

        ProgramPointer tmpIndex;
        tmpIndex.nFunction = nFunction;
        tmpIndex.nSentence = nSentence;
        RobotClient::initance()->sendCXXH(tmpIndex);
    }

    RobotClient::initance()->m_breakPointer.nFunction = nFunction;
    RobotClient::initance()->m_breakPointer.nSentence = nSentence;
}

void RobotProgramWidget::findBegin(int& nFunction, int& nSentence, string keyword)
{
    for(;nSentence >= 1;nSentence--){
        std::vector<std::vector<ProgramSentence>> tmp_FuncTable=m_fileSrc->getFuncTable();
        string tmp_keyword = Common::ToVectors(tmp_FuncTable[nFunction][nSentence].strSentence," ")[0];
        if(tmp_keyword == keyword){
           return;
        }
    }
    nFunction = 0;
    nSentence = 0;
    return;
}

string RobotProgramWidget::m_toEn(string tmp)
{
    string keyword;
    if(tmp == string("主函数"))
        keyword = "MAINFUNC";
    else if(tmp == string("子函数"))
        keyword = "SUBFUNC";
    else if(tmp == string("MAIN函数"))
        keyword = "MAIN";
    else if(tmp == string("函数"))
        keyword = "FUNC";
    else if(tmp == string("循环"))
        keyword = "FOR";
    else if(tmp == string("条件循环"))
        keyword="WHILE";
    else if(tmp == string("判断"))
        keyword = "IF";
    else if(tmp == string("或者判断"))
        keyword = "ELSEIF";
    else if(tmp == string("继续判断"))
        keyword = "ELSE";
    else if(tmp == string("结束MAIN函数"))
        keyword="ENDMAIN";
    else if(tmp == string("结束函数"))
        keyword = "ENDFUNC";
    else if(tmp == string("结束循环"))
        keyword = "ENDFOR";
    else if(tmp == string("结束条件循环"))
        keyword = "ENDWHILE";
    else if(tmp == string("ABS关节"))
        keyword="MOVABSJ";
    else if(tmp == string("相对ABS关节"))
        keyword = "MOVABSJR";
    else if(tmp == string("关节"))
        keyword = "MOVJ";
    else if(tmp == string("相对关节"))
        keyword = "MOVJR";
    else if(tmp == string("直线"))
        keyword="MOVL";
    else if(tmp == string("相对直线"))
        keyword="MOVLR";
    else if(tmp == string("圆弧"))
        keyword = "MOVC";
    else if(tmp == string("相对圆弧"))
        keyword = "MOVCR";
    else if(tmp == string("曲线"))
        keyword="MOVB";
    else if(tmp == string("关节时间"))
        keyword="MOVJT";
    else if(tmp == string("返回"))
        keyword = "RETURN";
    else if(tmp == string("调用"))
        keyword = "CALL";
    else if(tmp == string("计算"))
        keyword="CALCU";
    else if(tmp == string("延时"))
        keyword="DELAY";
    else if(tmp == string("等待数字输入"))
        keyword = "WAITDIN";
    else if(tmp == string("数字输出"))
        keyword = "DOUT";
    else if(tmp == string("模拟输出"))
        keyword="AOUT";
    else if(tmp == string("注释"))
        keyword="COM";
    else if(tmp == string("布尔型"))
        keyword = "BOOL";
    else if(tmp == string("整型"))
        keyword = "INT";
    else if(tmp == string("双精度型"))
        keyword="DOUBLE";
    else if(tmp == string("关节型"))
        keyword="JOINT";
    else if(tmp == string("末端型"))
        keyword = "TERMINAL";
    else if(tmp == string("结束循环"))
        keyword="ENDFOR";
    else if(tmp == string("修改工具"))
        keyword = "TOOLFRAME";
    else if(tmp == string("修改工件"))
        keyword = "VLOCATE";
    else if(tmp == string("工具坐标系"))
        keyword = "TOOL";
    else if(tmp == string("工件坐标系"))
        keyword="WORK";
    else if(tmp == string("变量"))
        keyword="VALUE";
    else if(tmp == string("坐标系"))
        keyword = "FRAME";
    else if(tmp == string("继续"))
        keyword="CONTINUE";
    else if(tmp == string("跳出"))
        keyword="BREAK";

    return keyword;
}

void RobotProgramWidget::findNextProgram(int &func, int &line)
{
    int i = 0;
    for (auto c:m_treeLine)
    {
        if((c.nFunction == func)&&(c.nSentence == line)){
            func = m_treeLine[i+1].nFunction;
            line = m_treeLine[i+1].nSentence;
            break;
        }
        i++;
    }
}

void RobotProgramWidget::updateDragPointList(){
    m_widgetRobotProgramMode->updateDragPointTable();
}

bool RobotProgramWidget::getSentenceKeyWord(string& keyword){
    switch(m_tabProgramTree->currentIndex()){
    case 0:{
        QTreeWidgetItem* item = m_treeVariable->currentItem();
        if(!item){
            QMessageBox::question(this, tr("提示"),
                                  tr("请先选中一条语句!"),
                                  QMessageBox::Yes);
            return false;
        }

        QString text = item->text(0);

        int index = text.indexOf(" ");
        if(index == -1){
            return false;
        }
        else{
            keyword = string("VALUE");
            return true;
        }
    }
        break;
    case 1:{
        QTreeWidgetItem* item = m_treeMainFunc->currentItem();
        if(!item){
            if((m_sentenceEdit.nFunction == 1) && (m_sentenceEdit.nSentence == 0)){
                QMessageBox::question(this, tr("提示"),
                                      tr("请先选中一条语句!"),
                                      QMessageBox::Yes);
                return false;
            }else{
                QTreeWidgetItemIterator it_main(m_treeMainFunc, QTreeWidgetItemIterator::All);
                it_main += m_sentenceEdit.nSentence - 1;

                item = *it_main;
                m_treeMainFunc->setCurrentItem(item);
            }
        }

        QString text = item->text(0);
        string tmp;

        int index = text.indexOf(" ");
        if(index == -1){
            if(isProgramChinese()){
                tmp = m_toEn(text.toStdString());
                keyword = tmp;
            }
            else
                keyword = text.toStdString();
        }
        else{
            if(isProgramChinese()){
                tmp = m_toEn(text.mid(0, index).toStdString());
                keyword = tmp;
            }
            else
                keyword = text.mid(0, index).toStdString();
        }

        if(keyword == string("JOINT") || keyword == string("TERMINAL")){
            keyword = "VALUE";
        }

        return true;
    }
        break;
    case 2:{
        QTreeWidgetItem* item = m_treeSubFunc->currentItem();
        if(!item){
            if((m_sentenceEdit.nFunction == 1) && (m_sentenceEdit.nSentence == 0)){
                QMessageBox::question(this, tr("提示"),
                                      tr("请先选中一条语句!"),
                                      QMessageBox::Yes);
                return false;
            }else{
                QTreeWidgetItemIterator it_sub(m_treeSubFunc, QTreeWidgetItemIterator::All);
                it_sub += m_sentenceEdit.nSentence - 1;
                item = *it_sub;
                m_treeSubFunc->setCurrentItem(item);
            }
        }

        QString text = item->text(0);
        string tmp;

        int index = text.indexOf(" ");
        if(index == -1){  
            if(isProgramChinese()){
                tmp = m_toEn(text.toStdString());
                keyword = tmp;
            }
            else
                keyword = text.toStdString();
        }
        else{
            if(isProgramChinese()){
                tmp = m_toEn(text.mid(0, index).toStdString());
                keyword = tmp;
            }
            else
                keyword = text.mid(0, index).toStdString();
        }

        if(keyword == string("JOINT") || keyword == string("TERMINAL")){
            keyword = "VALUE";
        }

        return true;
    }
        break;
    default:
        return false;
        break;
    }
}

bool RobotProgramWidget::getVariableName(string& name){
    switch(m_tabProgramTree->currentIndex()){
    case 0:{
        QTreeWidgetItem* item = m_treeVariable->currentItem();
        QString text = item->text(0);

        int index = text.indexOf(" ");
        name = text.mid(index+1).toStdString();
    }
        break;
    case 1:{
        QTreeWidgetItem* item = m_treeMainFunc->currentItem();
        QString text = item->text(0);

        int index = text.indexOf(" ");
        name = text.mid(index+1).toStdString();

        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);

    }
        break;
    case 2:{
        QTreeWidgetItem* item = m_treeSubFunc->currentItem();
        QString text = item->text(0);

        int index = text.indexOf(" ");
        name = text.mid(index+1).toStdString();

        ProgramPointer mapIndex;
        getSentenceIndex(mapIndex);
    }
        break;
    default:
        break;
    }

    return true;
}

bool RobotProgramWidget::getSentenceIndex(rclib::ProgramPointer& pointer){
    switch(m_tabProgramTree->currentIndex()){
    case 0:{
        return false;
    }
        break;
    case 1:{
        QTreeWidgetItem* item = m_treeMainFunc->currentItem();
        if(!item){
            return false;
        }

        vector<int> indexList;
        while(1){
            QTreeWidgetItem* itemP = item->parent();
            if(!itemP){
                break;
            }

            int index = itemP->indexOfChild(item);
            indexList.push_back(index);
            item = itemP;
        }

        int count = getTreeChildItemCount(indexList.size()-1, indexList, item);

        ProgramPointer treeIndex;
        treeIndex.nFunction = 1;
        treeIndex.nSentence = count;

        ProgramPointer mapIndex = findMapFromTree(treeIndex);

        pointer.nFunction = mapIndex.nFunction;
        pointer.nSentence = mapIndex.nSentence;

        m_sentenceEdit.nFunction = 1;
        m_sentenceEdit.nSentence = count;
    }
        break;
    case 2:{
        QTreeWidgetItem* item = m_treeSubFunc->currentItem();
        if(!item){
            return false;
        }

        vector<int> indexList;
        while(1){
            QTreeWidgetItem* itemP = item->parent();
            if(!itemP){
                break;
            }

            int index = itemP->indexOfChild(item);
            indexList.push_back(index);
            item = itemP;
        }

        int count = getTreeChildItemCount(indexList.size()-1, indexList, item);

        ProgramPointer treeIndex;
        treeIndex.nFunction = 2;
        treeIndex.nSentence = count;
        ProgramPointer mapIndex = findMapFromTree(treeIndex);

        pointer.nFunction = mapIndex.nFunction;
        pointer.nSentence = mapIndex.nSentence;

        m_sentenceEdit.nFunction = 2;
        m_sentenceEdit.nSentence = count;
    }
        break;
    default:
        return false;
        break;
    }

    return true;
}

int RobotProgramWidget::getTreeChildItemCount(int level, vector<int>& indexList, QTreeWidgetItem* item){
    int count = 1;
    if(item->childCount() == 0 || indexList.size() == 0){
        return count;
    }

    if(level == -1){
        for(int i=0; i<item->childCount(); i++){
            count += getTreeChildItemCount(-1, indexList, item->child(i));
        }
    }
    else{
        for(int i=0; i<indexList[level]; i++){
            count += getTreeChildItemCount(-1, indexList, item->child(i));
        }

        if(level == 0){
            count ++;
        }
        else{
            count += getTreeChildItemCount(level-1, indexList, item->child(indexList[level]));
        }
    }

    return count;
}

ProgramPointer RobotProgramWidget::findMapFromTree(ProgramPointer treeIndex){
    for(int i=0; i<m_treeLine.size(); i++){
        if ((m_treeLine[i].nFunction == treeIndex.nFunction)&&(m_treeLine[i].nSentence == treeIndex.nSentence)){
            return m_mapLine[i];
        }
    }
}

void RobotProgramWidget::updateProgramEditMenuState(bool state){
    m_widgetProgramEdit->setEnabled(state);
}

void RobotProgramWidget::showRecomInfoSlot(QString info){
    emit showRecomInfoSignal(info);
}
