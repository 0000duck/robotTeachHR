#ifndef ROBOTPROGRAMSUBFUNCWIDGET_H
#define ROBOTPROGRAMSUBFUNCWIDGET_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "publicstate.h"

#include "../robotServer/RobotInterpreter/robotInterpreter.h"
#include "../robotServer/RobotInterpreter/functions.h"

#include "../normalFunc/macroDefine.h"

#include "../pushButton/pushButton.h"

class QDialog;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QLabel;
class QGroupBox;
class QListView;
class QMessageBox;
class RobotProgramSubFuncWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramSubFuncWidget(QWidget* parent = NULL);
    ~RobotProgramSubFuncWidget();

    void passParameter(rclib::RobotInterpreter* p_file);

    enum OPERATIONMODE {TEACH_ADD, TEACH_MODIFY};

    void setMode(OPERATIONMODE mode);
    void passSentenceIndex(int func, int line);

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void clearState();
    bool grammarCheck();

private:
    QLineEdit* m_editFuncName;

    PushButton* m_buttonYes;
    PushButton* m_buttonNo;

    OPERATIONMODE m_mode;
    int m_func;
    int m_line;

    rclib::RobotInterpreter* m_fileSrc;

    std::vector<std::vector<rclib::ProgramSentence>>*   m_vecFuncTable;     //存放函数
    std::map<std::string, int>*              m_mapFuncIndex;         //函数索引

    std::map<std::string, rclib::PROGRAM_KEY_WORD>*          m_mapVarType;  //变量类型
    std::map<std::string, std::vector<double>>*  m_mapVarValue;      //变量值

private slots:
    void buttonYesClicked();
    void buttonNoClicked();

signals:
    void programOperationFinishedSignal(bool);

    void editSubSignal();
};

#endif
