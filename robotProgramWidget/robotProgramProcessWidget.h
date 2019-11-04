#ifndef ROBOTPROGRAMPROCESSWIDGET_H
#define ROBOTPROGRAMPROCESSWIDGET_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QGroupBox>
//#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>
#include <QComboBox>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "publicstate.h"

#include "../robotServer/RobotInterpreter/robotInterpreter.h"
#include "../robotServer/RobotInterpreter/functions.h"

#include "../normalFunc/macroDefine.h"

#include "../pushButton/pushButton.h"
#include "../comboBox/comboBox.h"

class QDialog;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QRadioButton;
class QGroupBox;
//class QComboBox;
class QLabel;
class QListView;
class QMessageBox;
class ComboBox;
class RobotProgramProcessWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramProcessWidget(QWidget* parent = NULL);
    ~RobotProgramProcessWidget();

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

    bool digitalCheck(const QString&);
    bool variableCheck(const QString&);
    bool funcCheck(const QString&);

    void updateComboList();
    int find(QComboBox*,string);

    void updateFor();
    void updateWhile();
    void updateIO();
    void updateFunc();
    void updateTime();

private:
    QLineEdit* m_editCom;
    QRadioButton* m_radioFunc;
    QRadioButton* m_radioFor;
    QRadioButton* m_radioWhile;
    QRadioButton* m_radioIf;
    QRadioButton* m_radioIO;
    QRadioButton* m_radioTime;
    QRadioButton* m_radioCom;

    QGroupBox* groupProcessType;

    QLabel* m_labelParameterLeft;
    QLabel* m_labelParameterOperator;
    QLabel* m_labelParameterRight;

    QComboBox* m_comboParameterLeft;
    QComboBox* m_comboParameterRight;

    ComboBox* m_comboOperator;
    ComboBox* m_comboSubMenuFunc;
    ComboBox* m_comboSubMenuFor;
    ComboBox* m_comboSubMenuWhile;
    ComboBox* m_comboSubMenuIf;
    ComboBox* m_comboSubMenuIO;
    ComboBox* m_comboSubMenuTime;
    ComboBox* m_comboSubMenuCom;

    PushButton* m_buttonYes;
    PushButton* m_buttonNo;

    OPERATIONMODE m_mode;
    int m_func;
    int m_line;

    rclib::RobotInterpreter* m_fileSrc = NULL;

    std::vector<std::vector<rclib::ProgramSentence>>*   m_vecFuncTable;     //存放函数
    std::map<std::string, int>*              m_mapFuncIndex;         //函数索引

    std::map<std::string, rclib::PROGRAM_KEY_WORD>*          m_mapVarType;  //变量类型
    std::map<std::string, std::vector<double>>*  m_mapVarValue;      //变量值

    QObject* cache = nullptr;
    int num_cache = -1;

private slots:
    void buttonYesClicked();
    void buttonNoClicked();

    void comboSubMenuFuncChanged(int);
    void comboSubMenuForChanged(int);
    void comboSubMenuWhileChanged(int);
    void comboSubMenuIfChanged(int);
    void comboSubMenuIOChanged(int);
    void comboSubMenuTimeChanged(int);
    void comboSubMenuComChanged(int);

    void radioProcessTypeChanged();

signals:
    void programOperationFinishedSignal(bool);
};


#endif
