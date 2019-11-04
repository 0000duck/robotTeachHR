#ifndef ROBOTPROGRAMVARIABLEWIDGET_H
#define ROBOTPROGRAMVARIABLEWIDGET_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "publicstate.h"

#include "../robotServer/RobotInterpreter/robotInterpreter.h"

#include "../normalFunc/globalParameter.h"
#include "../normalFunc/macroDefine.h"

#include "../pushButton/pushButton.h"

#include "../robotServer/RobotStructure/robotStructure.h"
#include "../robotServer/RobotClient/robotClient.h"

#define ROBOTDOF 6

class QDialog;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QRadioButton;
class QGroupBox;
class QLabel;
class QListView;
class QMessageBox;
class RobotProgramVariableWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramVariableWidget(QWidget* parent = NULL);
    ~RobotProgramVariableWidget();

    void passParameter(rclib::RobotInterpreter* p_file);

    enum OPERATIONMODE {TEACH_ADD, TEACH_MODIFY};

    void setMode(OPERATIONMODE mode);
    void passVariableName(std::string varName);

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void clearState();
    bool grammarCheck();

private:
    QRadioButton* m_radioJOINT;
    QRadioButton* m_radioTERMINAL;
//    QRadioButton* m_radioFRAME;
    QRadioButton* m_radioINT;
    QRadioButton* m_radioDOUBLE;
    QRadioButton* m_radioBOOL;

    QGroupBox* groupVariableType;

    QLineEdit* m_editVariableName;
    QLineEdit* m_editVariablePositionValue[ROBOTDOF];
    QLineEdit* m_editVariableValue;

    PushButton* m_buttonBool;

    PushButton* m_buttonFrame;

    PushButton* m_buttonToolFrame;
    PushButton* m_buttonUserFrame;

    PushButton* m_buttonMoveTo;
    PushButton* m_buttonPosition;

    PushButton* m_buttonYes;
    PushButton* m_buttonNo;

    OPERATIONMODE m_mode;

    QGroupBox* m_groupVariableGetValue;

    rclib::RobotInterpreter* m_fileSrc;

    std::string m_varName;

private slots:
    void buttonYesClicked();
    void buttonNoClicked();

    void buttonToolFrameClicked();
    void buttonUserFrameClicked();
    void buttonMoveToClicked();
    void buttonPositionClicked();

    void buttonBoolClicked();
    void buttonFrameClicked();

    void radioVariableTypeChanged();

signals:
    void programOperationFinishedSignal(bool);
};


#endif
