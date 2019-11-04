#ifndef ROBOTPROGRAMWIDGET_H
#define ROBOTPROGRAMWIDGET_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGroupBox>
#include <QFileInfo>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "robotProgramFileWidget.h"
#include "robotProgramVariableWidget.h"
#include "robotProgramCalculateWidget.h"
#include "robotProgramModeWidget.h"
#include "robotProgramMotionWidget.h"
#include "robotProgramProcessWidget.h"
#include "robotProgramFrameWidget.h"
#include "robotProgramSubFuncWidget.h"
#include "robotProgramVisionWidget.h"
#include "publicstate.h"

#include "../normalFunc/macroDefine.h"
#include "../normalFunc/rwFile.h"
#include "../normalFunc/globalParameter.h"
#include "../pushButton/pushButton.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotStructure/robotStructure.h"
#include "../robotServer/RobotInterpreter/robotInterpreter.h"
#include "../robotServer/RobotInterpreter/functions.h"
#include "../robotServer/RobotStructure/robotFile.h"


class QDialog;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QTabWidget;
class QLabel;
class QTreeWidget;
class QTreeWidgetItem;
class QCheckBox;
class PushButton;
class QGroupBox;
class RobotProgramFileWidget;
class RobotProgramVariableWidget;
class RobotProgramCalculateWidget;
class RobotProgramModeWidget;
class RobotProgramMotionWidget;
class RobotProgramProcessWidget;
class RobotProgramFrameWidget;
class RobotProgramSubFuncWidget;
class RobotProgramVisionWidget;
class RobotInterpreter;
class robotFile;
class RobotProgramWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramWidget(QWidget *parent = NULL);
    ~RobotProgramWidget();

    void updateDragPointList();
    bool programLoad();

    void updateRobotProgramWidget();

    void updateSystemState();

    void passState(rclib::SYSPLAYSTATE,rclib::SWITCHSTATE);

    void highlight(int,int);

    void updateWidget();

    void setBreakPoint(bool);

    void findBegin(int&,int&,string);
private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void initTreeStyle();

    bool getSentenceKeyWord(std::string& keyword);
    bool getVariableName(std::string& name);
    bool getSentenceIndex(rclib::ProgramPointer& pointer);
    int getTreeChildItemCount(int level, std::vector<int>& indexList, QTreeWidgetItem* item);
    rclib::ProgramPointer findMapFromTree(rclib::ProgramPointer);

    void updateProgramEditMenuState(bool state);

    string m_toEn(string);

    void findNextProgram(int&,int&);

private:
    QTabWidget* m_tabProgramTree;
    QWidget* m_widgetVariable;
    QWidget* m_widgetMainFunc;
    QWidget* m_widgetSubFunc;
    QTreeWidget* m_treeVariable;
    QTreeWidget* m_treeMainFunc;
    QTreeWidget* m_treeSubFunc;

    QLabel* m_labelProgramNamel;
    QCheckBox* m_checkProgramTrack;
    QCheckBox* m_checkProgramChinese;

    QWidget* m_widgetProgramEdit;

    QWidget* m_widgetProgramEditMenu;

    PushButton* m_buttonNewFile;
    PushButton* m_buttonOpenFile;
    PushButton* m_buttonSaveFile;
    PushButton* m_buttonSaveasFile;
    PushButton* m_buttonUSB;

    PushButton* m_buttonCopySentence;
    PushButton* m_buttonPasteSentence;
    PushButton* m_buttonDeleteSentence;
    PushButton* m_buttonModifySentence;
    PushButton* m_buttonComSentence;
    PushButton* m_buttonShearSentence;

    PushButton* m_buttonVariablePage;
    PushButton* m_buttonCalculatePage;
    PushButton* m_buttonProcessPage;
    PushButton* m_buttonMotionPage;
    PushButton* m_buttonFramePage;
    PushButton* m_buttonAdvanceModePage;
    PushButton* m_buttonSubFunc;
    PushButton* m_buttonVision;

    RobotProgramFileWidget* m_widgetRobotProgramFile;
    RobotProgramVariableWidget* m_widgetRobotProgramVariable;
    RobotProgramCalculateWidget* m_widgetRobotProgramCalculate;
    RobotProgramModeWidget* m_widgetRobotProgramMode;
    RobotProgramMotionWidget* m_widgetRobotProgramMotion;
    RobotProgramProcessWidget* m_widgetRobotProgramProcess;
    RobotProgramFrameWidget* m_widgetRobotProgramFrame;
    RobotProgramSubFuncWidget* m_widgetRobotProgramSubFunc;
    RobotProgramVisionWidget* m_widgetRobotProgramVision;

    QString s_styleTree;

    std::string m_fileName;

    rclib::RobotInterpreter* m_fileSrc;

    std::vector<rclib::ProgramPointer> m_treeLine;
    std::vector<rclib::ProgramPointer> m_mapLine;

    rclib::ProgramPointer m_sentenceLast;

    int last_func;
    int last_sentence;

    std::map<std::string, int>              testIndex;

    int m_mainTreeItemCount;
    int m_subTreeItemCount;

    int m_sentenceCopyedFlag;
    rclib::ProgramSentence m_sentenceCopyed;
    rclib::ProgramPointer m_sentenceEdit;

    rclib::SYSPLAYSTATE local_playStateFlag;
    rclib::SWITCHSTATE local_controlStateFlag;

    bool tag_COPY;
    vector<rclib::ProgramSentence> vectorCopy;

    rclib::ProgramPointer mapBegin;
    rclib::ProgramPointer mapEnd;
private slots:
    void checkProgramChineseChanged();

    void buttonNewFileClicked();
    void buttonOpenFileClicked();
    void buttonSaveFileClicked();
    void buttonSaveasFileClicked();
    void buttonUSBClicked();

    void buttonCopySentenceClicked();
    void buttonPasteSentenceClicked();
    void buttonDeleteSentenceClicked();
    void buttonModifySentenceClicked();
    void buttonComSentenceClicked();
    void buttonShearSentenceClicked();

    bool buttonLoadProgramClicked();

    void buttonVariablePageClicked();
    void buttonCalculatePageClicked();
    void buttonProcessPageClicked();
    void buttonMotionPageClicked();
    void buttonFramePageClicked();
    void buttonAdvanceModePageClicked();
    void buttonSubFuncPageClicked();
    void buttonVisionPageClicked();

    void programOperationFinished(bool);

    void updateProgramTree();

    void showRecomInfoSlot(QString);

    void findNextSub();

signals:
    void loadProgramFile();
    void showRecomInfoSignal(QString);

    void programOperationFinishedSignal(bool);
};
#endif
