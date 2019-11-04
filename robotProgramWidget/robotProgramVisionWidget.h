#ifndef ROBOTPROGRAMVISIONWIDGET_H
#define ROBOTPROGRAMVISIONWIDGET_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
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

#include "../robotServer/RobotStructure/robotStructure.h"

class QDialog;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QRadioButton;
class QLabel;
class QGroupBox;
class QListView;
class QMessageBox;
class RobotProgramVisionWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramVisionWidget(QWidget* parent = NULL);
    ~RobotProgramVisionWidget();

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
    QRadioButton* m_radioVisionLocate;

    QLabel* m_labelVariableName;
    QLineEdit* m_editVariableName;
    PushButton* m_buttonVariableName[3];

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
    void radioVisionTypeChanged();
    void buttonVariableNameClicked(int);

    void buttonYesClicked();
    void buttonNoClicked();

signals:
    void programOperationFinishedSignal(bool);
};

#endif
