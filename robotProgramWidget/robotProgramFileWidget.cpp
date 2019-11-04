#include "robotProgramFileWidget.h"

using namespace std;
using namespace rclib;

RobotProgramFileWidget::RobotProgramFileWidget(QWidget *parent): QWidget(parent){
    initUI();
    initState();
    connectSignalSlot();
}

RobotProgramFileWidget::~RobotProgramFileWidget(){

}

void RobotProgramFileWidget::passParameter(RobotInterpreter* p_file,
                                           std::string *p_fileName){
    m_fileSrc = p_file;
    m_fileName = p_fileName;
}

void RobotProgramFileWidget::setMode(FILEOPERATIONMODE mode){
    m_mode = mode;
    updateProgramFileList();
    switch(m_mode){
    case TEACH_NEWFILE:{
        m_labelProgramFileName->show();
        m_editProgramFileName->show();
        m_buttonYes->show();
        m_buttonNo->show();
        m_buttonUSBCopy->hide();
        m_buttonUSBSave->hide();
    }
        break;
    case TEACH_OPENFILE:{
        m_labelProgramFileName->hide();
        m_editProgramFileName->hide();
        m_buttonYes->show();
        m_buttonNo->show();
        m_buttonUSBCopy->hide();
        m_buttonUSBSave->hide();
    }
        break;
    case TEACH_SAVEFILE:{
        m_labelProgramFileName->show();
        m_editProgramFileName->show();
        m_buttonYes->show();
        m_buttonNo->show();
        m_buttonUSBCopy->hide();
        m_buttonUSBSave->hide();
    }
        break;
    case TEACH_USB:{
        m_labelProgramFileName->hide();
        m_editProgramFileName->hide();
        m_buttonYes->hide();
        m_buttonNo->show();
        m_buttonUSBCopy->show();
        m_buttonUSBSave->show();
    }
        break;
    default:
        break;
    }
}

void RobotProgramFileWidget::showFileInfoList(QFileInfoList* list)
{
    m_widgetList->clear();

    for (unsigned int i = 0; i < list->count(); i++)
    {
        QFileInfo tmpFileInfo = list->at(i);
        if (tmpFileInfo.isDir()){
            QIcon icon = QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon);
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem* tmpListWidgetItem = new QListWidgetItem(icon,fileName);
            m_widgetList->addItem(tmpListWidgetItem);
        }else{
            QIcon icon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem* tmpListWidgetItem = new QListWidgetItem(icon,fileName);
            m_widgetList->addItem(tmpListWidgetItem);
        }
    }
}

void RobotProgramFileWidget::initUI(){
    m_tableProgramFileList = new QTableWidget();

    m_labelProgramFileName = new QLabel(tr("文件名:"));
    m_editProgramFileName = new QLineEdit();
    m_buttonYes = new PushButton(tr("确定"));
    m_buttonNo = new PushButton(tr("取消"));
    m_buttonDelete = new PushButton(tr("删除文件"));
    m_buttonUSBCopy = new PushButton(tr("从U盘复制"));
    m_buttonUSBSave = new PushButton(tr("复制到U盘"));

    m_editProgramFileName->setMinimumSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonYes->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonNo->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonDelete->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonUSBCopy->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);
    m_buttonUSBSave->setFixedSize(0.08*WINDOWWIDTH, 0.04*WINDOWWIDTH);

    QHBoxLayout* layoutFileOperation = new QHBoxLayout();
    layoutFileOperation->addWidget(m_labelProgramFileName);
    layoutFileOperation->addWidget(m_editProgramFileName);
    layoutFileOperation->addWidget(m_buttonYes);
    layoutFileOperation->addWidget(m_buttonNo);
    layoutFileOperation->addWidget(m_buttonUSBCopy);
    layoutFileOperation->addWidget(m_buttonUSBSave);
    layoutFileOperation->addWidget(m_buttonDelete);

    QVBoxLayout* layoutFile = new QVBoxLayout(this);
    layoutFile->addWidget(m_tableProgramFileList);
    layoutFile->addLayout(layoutFileOperation);

    m_widgetUSB = new QWidget(this);
    m_widgetUSB->setWindowFlags(Qt::Dialog);
    m_widgetUSB->setWindowModality(Qt::WindowModal);
    m_widgetUSB->setWindowTitle(tr("从U盘复制"));
    m_widgetUSB->resize(400,400);

    m_widgetList = new QListWidget();
    m_listInfo = new QFileInfoList();

    m_filePath = new QLineEdit();
    m_filePath->setFocusPolicy(Qt::NoFocus);
    m_filePath->setText("/media/");

    QString rootStr = "/media/";
    m_stackPath.push(rootStr);

    QDir rootDir(rootStr);
    rootDir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList tmp;
    tmp = rootDir.entryInfoList();
    m_listInfo = &tmp;

    showFileInfoList(m_listInfo);

    m_buttonReturn = new QPushButton();
    m_buttonReturn->setText(tr("上一级"));
    m_buttonEnter = new QPushButton();
    m_buttonEnter->setText(tr("进入"));
    m_buttonSelect = new QPushButton();
    m_buttonSelect->setText(tr("确定"));
    m_buttonCancel = new QPushButton();
    m_buttonCancel->setText(tr("取消"));

    QVBoxLayout* layoutButton = new QVBoxLayout();
    layoutButton->addWidget(m_buttonReturn);
    layoutButton->addWidget(m_buttonEnter);
    layoutButton->addWidget(m_buttonSelect);
    layoutButton->addWidget(m_buttonCancel);

    QHBoxLayout* layoutMain = new QHBoxLayout();
    layoutMain->addWidget(m_widgetList);
    layoutMain->addLayout(layoutButton);

    QVBoxLayout* layoutWidget = new QVBoxLayout(m_widgetUSB);
    layoutWidget->addWidget(m_filePath);
    layoutWidget->addLayout(layoutMain);

    m_widgetUSB->hide();
}

void RobotProgramFileWidget::initState(){
    m_tableProgramFileList->setColumnCount(1);
    m_tableProgramFileList->horizontalHeader()->setSectionsClickable(false);
    QStringList header;
    header<<tr("文件名");
    m_tableProgramFileList->setHorizontalHeaderLabels(header);
    QFont font = m_tableProgramFileList->horizontalHeader()->font();
    font.setBold(true);
    m_tableProgramFileList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:gray;}"); //设置表头背景色
    m_tableProgramFileList->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    m_tableProgramFileList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    m_tableProgramFileList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableProgramFileList->verticalHeader()->setStyleSheet("QHeaderView::section{background:gray;}");
    m_tableProgramFileList->verticalHeader()->setVisible(true); //设置垂直头可见
    m_tableProgramFileList->verticalHeader()->setDefaultSectionSize(30); //固定行高
    m_tableProgramFileList->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    m_tableProgramFileList->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    m_tableProgramFileList->setSelectionMode(QAbstractItemView::SingleSelection);
    //    m_tableProgramFileList->setEditTriggers(QAbstractItemView::SelectedClicked); //no edit
    //设置水平、垂直滚动条样式
    m_tableProgramFileList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                                 "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                                 "QScrollBar::handle:hover{background:gray;}"
                                                                 "QScrollBar::sub-line{background:transparent;}"
                                                                 "QScrollBar::add-line{background:transparent;}");
    m_tableProgramFileList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
                                                               "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                               "QScrollBar::handle:hover{background:gray;}"
                                                               "QScrollBar::sub-line{background:transparent;}"
                                                               "QScrollBar::add-line{background:transparent;}");

    m_tableProgramFileList->setStyleSheet("QTableCornerButton::section{background-color:gray;}");
}

void RobotProgramFileWidget::connectSignalSlot(){
    QObject::connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(buttonYesClicked()));
    QObject::connect(m_buttonNo, SIGNAL(clicked()), this, SLOT(buttonNoClicked()));
    QObject::connect(m_buttonDelete, SIGNAL(clicked()), this, SLOT(buttonDeleteClicked()));
    QObject::connect(m_buttonUSBCopy, SIGNAL(clicked()), this, SLOT(buttonUSBCopyClicked()));
    QObject::connect(m_buttonUSBSave, SIGNAL(clicked()), this, SLOT(buttonUSBSaveClicked()));

    QObject::connect(m_buttonReturn, SIGNAL(clicked()), this, SLOT(buttonReturnClicked()));
    QObject::connect(m_buttonEnter, SIGNAL(clicked()), this, SLOT(buttonEnterClicked()));
    QObject::connect(m_buttonSelect, SIGNAL(clicked()), this, SLOT(buttonSelectClicked()));
    QObject::connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(buttonCancelClicked()));
}

void RobotProgramFileWidget::slotShow(QDir dir)
{
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList InfoList = dir.entryInfoList();
    QFileInfoList* tmp = &InfoList;
    showFileInfoList(tmp);
}

void RobotProgramFileWidget::buttonYesClicked(){
    switch(m_mode){
    case TEACH_NEWFILE:{
        if(m_editProgramFileName->text().isEmpty()){
            QMessageBox::question(this, tr("提示"),
                                  tr("请先填写文件名!"),
                                  QMessageBox::Yes);
            return;
        }
        if(m_files.contains(m_editProgramFileName->text()+QString(".program"))){
            QMessageBox::question(this, tr("提示"),
                                  tr("该文件名已存在!"),
                                  QMessageBox::Yes);
            return;
        }

        m_fileSrc->clearFuncTable();
        m_fileSrc->clearVarTable();

        ProgramSentence temp1,temp2;
        temp1.strSentence="MAIN";
        temp2.strSentence="ENDMAIN";

        ProgramPointer m_addres;
        m_addres.nFunction=0;
        m_addres.nSentence=0;

        m_fileSrc->addProgramSentence(m_addres,temp1);

        m_addres.nSentence=1;
        m_fileSrc->addProgramSentence(m_addres,temp2);

        *m_fileName = m_editProgramFileName->text().toStdString();
        m_fileSrc->writeRobotProgram((string(WINDOWPROGRAMPATH) + *m_fileName + string(".program")).c_str());

        QString programName = m_editProgramFileName->text();
        write_cache_program(programName);

        updateProgramFileList();
    }
        break;
    case TEACH_OPENFILE:{
        int currentRowIndex = m_tableProgramFileList->currentRow();
        if(currentRowIndex == -1){
            QMessageBox::question(this, tr("提示"),
                                  tr("请先选中需要打开的文件!"),
                                  QMessageBox::Yes);
            return;
        }
        QTableWidgetItem* item = m_tableProgramFileList->item(currentRowIndex, 0);
        *m_fileName = item->text().toStdString();

        QString programName;
        read_cache_program(programName);
        string oldfileName = programName.toStdString();

        if((oldfileName!="")&&(*m_fileName!=oldfileName)){
            QMessageBox::StandardButton reply=QMessageBox::question(this, tr("保存"), tr("是否保存已打开文件？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(reply == QMessageBox::Yes)
            {
                m_fileSrc->writeRobotProgram((string(WINDOWPROGRAMPATH) + oldfileName + string(".program")).c_str());
            }
            else
            {
                emit programOperationFinishedSignal(true);
            }
        }

        m_fileSrc->clearFuncTable();
        m_fileSrc->clearVarTable();

        m_fileSrc->readRobotProgram((string(WINDOWPROGRAMPATH) + *m_fileName + string(".program")).c_str());

        programName = QString::fromStdString(*m_fileName);
        write_cache_program(programName);
    }
        break;
    case TEACH_SAVEFILE:{
        if(m_editProgramFileName->text().isEmpty()){
            QMessageBox::question(this, tr("提示"),
                                  tr("请先填写文件名!"),
                                  QMessageBox::Yes);
            return;
        }
        if(m_files.contains(m_editProgramFileName->text()+QString(".program"))){
            QMessageBox::question(this, tr("提示"),
                                  tr("该文件名已存在!"),
                                  QMessageBox::Yes);
            return;
        }

        *m_fileName = m_editProgramFileName->text().toStdString();

        QString programName = m_editProgramFileName->text();
        write_cache_program(programName);

        m_fileSrc->writeRobotProgram((string(WINDOWPROGRAMPATH) + *m_fileName + string(".program")).c_str());
        updateProgramFileList();
    }
        break;
    default:
        break;
    }

    emit programOperationFinishedSignal(true);
}

void RobotProgramFileWidget::buttonNoClicked(){
    emit programOperationFinishedSignal(false);
}

void RobotProgramFileWidget::buttonDeleteClicked(){
    int currentRowIndex = m_tableProgramFileList->currentRow();
    if(currentRowIndex == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("请先选中需要删除的文件!"),
                              QMessageBox::Yes);
        return;
    }

    QTableWidgetItem* item = m_tableProgramFileList->item(currentRowIndex, 0);

    if((m_fileName != nullptr) && (item->text().toStdString() == (*m_fileName))){
        QMessageBox::question(this, tr("提示"),
                              tr("此文件正在使用!"),
                              QMessageBox::Yes);
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("提示"),
                                  tr("确定要删除该文件吗？"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
        return;

    QFile file(QString(WINDOWPROGRAMPATH)+item->text() + QString(".program"));
    if (file.exists()){
        file.remove();
    }

    updateProgramFileList();
}

void RobotProgramFileWidget::updateProgramFileList(){
    m_tableProgramFileList->clearContents();
    m_tableProgramFileList->setRowCount(0);

    QDir dir(WINDOWPROGRAMPATH);
    QStringList nameFilters;
    nameFilters << QString("*.program");
    m_files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

    for(int i=0; i<m_files.size(); i++){
        int index = m_files[i].lastIndexOf (".program");
        QString text = m_files[i].left(index);

        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(text);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        m_tableProgramFileList->insertRow(i);
        m_tableProgramFileList->setItem(i, 0, item);
    }
}

void RobotProgramFileWidget::buttonReturnClicked()
{
    if(m_stackPath.size()<=1)
        return;
    m_stackPath.pop();
    QString tmp = m_stackPath.top();
    QDir tmpDir(tmp);
    m_filePath->setText(tmpDir.absolutePath());
    slotShow(tmpDir);
}

void RobotProgramFileWidget::buttonEnterClicked()
{
    QListWidgetItem* Item = m_widgetList->currentItem();
    if(Item == nullptr)
        return;
    slotDirShow(Item);
}

void RobotProgramFileWidget::buttonSelectClicked()
{
    if(CopyToUSB){
        QTableWidgetItem* item = m_tableProgramFileList->item(m_currentRowIndex, 0);
        srcPath = QString(WINDOWPROGRAMPATH)+item->text() + QString(".program");

        dstPath = m_filePath->text();
        dstPath += QString("/") + item->text() + QString(".program");

        QFile file(dstPath);
        if (file.exists()){
            emit showRecomInfoSignal(tr("复制文件到U盘失败, 目标文件已存在"));
            return;
        }
    }else{
        QString text = m_widgetList->currentItem()->text();
        int tmp = text.lastIndexOf(".");
        if(tmp == -1){
            return;
        }
        QString TMP = text.mid(tmp+1);
        if(TMP != QString("program"))
            return;

        srcPath = m_filePath->text() + QString("/") + text;
        int index = srcPath.lastIndexOf("/");
        dstPath = QString(WINDOWPROGRAMPATH)+srcPath.mid(index+1);

        QFile file(dstPath);
        if (file.exists()){
            emit showRecomInfoSignal(tr("从U盘复制文件失败, 目标文件已存在"));
            return;
        }
    }

    bool ok = QFile::copy(srcPath, dstPath);

    if(CopyToUSB){
        if(ok){
            QString rootStr = m_stackPath.top();
            QDir rootDir(rootStr);
            slotShow(rootDir);
            emit showRecomInfoSignal(tr("复制文件到U盘成功"));
        }
        else{
            emit showRecomInfoSignal(tr("复制文件到U盘失败"));
        }
    }else{
        if(ok){
            updateProgramFileList();
            emit showRecomInfoSignal(tr("从U盘复制文件成功"));
        }
        else{
            emit showRecomInfoSignal(tr("从U盘复制文件失败"));
        }
    }

    m_widgetUSB->hide();
}

void RobotProgramFileWidget::buttonCancelClicked()
{
    m_widgetUSB->hide();
}

void RobotProgramFileWidget::slotDirShow(QListWidgetItem* Item)
{
    QString string = Item->text();
    QDir dir;
    dir.setPath(m_filePath->text());
    dir.cd(string);
    m_filePath->setText(dir.absolutePath());
    m_stackPath.push(m_filePath->text());
    slotShow(dir);
}

void RobotProgramFileWidget::buttonUSBCopyClicked(){
    CopyToUSB = false;

    QString rootStr = "/media/";
    m_filePath->setText(rootStr);
    m_stackPath.clear();
    m_stackPath.push(rootStr);
    QDir rootDir(rootStr);
    slotShow(rootDir);

    m_widgetUSB->setWindowTitle(tr("从U盘复制"));
    m_widgetUSB->show();
}

void RobotProgramFileWidget::buttonUSBSaveClicked(){
    CopyToUSB = true;

    int currentRowIndex = m_tableProgramFileList->currentRow();
    if(currentRowIndex == -1){
        QMessageBox::question(this, tr("提示"),
                              tr("请先选中需要复制的文件!"),
                              QMessageBox::Yes);
        return;
    }
    m_currentRowIndex = currentRowIndex;

    QString rootStr = "/media/";
    m_filePath->setText(rootStr);
    m_stackPath.clear();
    m_stackPath.push(rootStr);
    QDir rootDir(rootStr);
    slotShow(rootDir);

    m_widgetUSB->setWindowTitle(tr("复制到U盘"));
    m_widgetUSB->show();
}
