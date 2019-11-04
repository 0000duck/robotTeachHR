#ifndef ROBOTPROGRAMMODEWIDGET_H
#define ROBOTPROGRAMMODEWIDGET_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <list>

#include "../robotServer/RobotInterpreter/robotInterpreter.h"
#include "../robotServer/RobotInterpreter/functions.h"

#include "../normalFunc/globalParameter.h"
#include "../normalFunc/macroDefine.h"

#include "../pushButton/pushButton.h"

#include "../robotServer/RobotStructure/robotStructure.h"
#include "../robotServer/RobotClient/robotClient.h"

class QDialog;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QRadioButton;
class QGroupBox;
class QLabel;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
class QScrollBar;
class QListView;
class QMessageBox;
class RobotProgramModeWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramModeWidget(QWidget* parent = NULL);
    ~RobotProgramModeWidget();

    void passParameter(rclib::RobotInterpreter* p_file);

    enum OPERATIONMODE {TEACH_ADD, TEACH_MODIFY};

    void setMode(OPERATIONMODE mode);
    void passSentenceIndex(int func, int line);

    void updateDragPointTable();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void clearState();
    bool grammarCheck();

    void initBCurveTable();
    void initDragTeachTable();

private:
    QRadioButton* m_radioDragTeach;
    QRadioButton* m_radioBCurve;
    QGroupBox* groupModeType;

    QTableWidget* m_tableBCurve;
    QTableWidget* m_tableDragTeach;

    PushButton* m_buttonAddPoint;
    PushButton* m_buttonInsertPoint;
    PushButton* m_buttonDeletePoint;
    PushButton* m_buttonClearPoint;
    PushButton* m_buttonMovePoint;

    QLineEdit* m_editBCurveVel;
    QLineEdit* m_editBCurveAcc;
    QLineEdit* m_editBCurveJerk;
    QLineEdit* m_editBCurveAngleVel;
    QLineEdit* m_editBCurvePre;
    PushButton* m_buttonBCurveFrame;

    QLineEdit* m_editDragTeachSamplePeriod;
    QLineEdit* m_editDragTeachTime;
    QLineEdit* m_editDragTeachPlayPeriod;
    PushButton* m_buttonStartDragRecord;
    PushButton* m_buttonDragPlayPrepare;
    PushButton* m_buttonDragPlay;

    QGroupBox* m_groupPointOperation;
    QGroupBox* m_groupBCurveParameter;
    QGroupBox* m_groupDragTeachParameter;

    PushButton* m_buttonYes;
    PushButton* m_buttonNo;

    OPERATIONMODE m_mode;
    int m_func;
    int m_line;

    rclib::RobotInterpreter* m_fileSrc;

private slots:
    void buttonYesClicked();
    void buttonNoClicked();

    void buttonAddPointClicked();
    void buttonInsertPointClicked();
    void buttonDeletePointClicked();
    void buttonClearPointClicked();
    void buttonMovePointClicked();

    void buttonStartDragRecordClicked();
    void buttonDragPlayPrepareClicked();
    void buttonDragPlayClicked();

    void buttonBCurveFrameClicked();

    void radioModeTypeChanged();
signals:
    void programOperationFinishedSignal(bool);
};


#endif
