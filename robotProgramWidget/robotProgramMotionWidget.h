#ifndef ROBOTPROGRAMMOTIONWIDGET_H
#define ROBOTPROGRAMMOTIONWIDGET_H

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
#include <QComboBox>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "publicstate.h"

#include "../robotServer/RobotInterpreter/robotInterpreter.h"
#include "../robotServer/RobotInterpreter/functions.h"

#include "../normalFunc/globalParameter.h"
#include "../normalFunc/macroDefine.h"

#include "../pushButton/pushButton.h"
#include "../comboBox/comboBox.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotStructure/robotStructure.h"

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
class RobotProgramMotionWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramMotionWidget(QWidget* parent = NULL);
    ~RobotProgramMotionWidget();

    void passParameter(rclib::RobotInterpreter* p_file);

    enum OPERATIONMODE {TEACH_ADD, TEACH_MODIFY};

    void setMode(OPERATIONMODE mode);
    void passSentenceIndex(int func, int line);

    void resetWayPointRandomIndex();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void clearState();
    bool grammarCheck();

    void initWayPointTable();

    void insertWayPoint(int row);

    QString randomWayPointName(int type);   // 0:joint 1:terminal

    void updateComboList();

    string getCurrentText();

private:
    QRadioButton* m_radioMovABSJ;
    QRadioButton* m_radioMovABSJR;
    QRadioButton* m_radioMovJ;
    QRadioButton* m_radioMovJR;
    QRadioButton* m_radioMovL;
    QRadioButton* m_radioMovLR;
    QRadioButton* m_radioMovC;
    QRadioButton* m_radioMovCR;

    QGroupBox* groupMOV;

    QLabel* m_labelCircleType;
    QLabel* m_labelVel;
    QLabel* m_labelAcc;
    QLabel* m_labelJerk;
    QLabel* m_labelRadius;
    QLabel* m_labelFrame;

    QGroupBox* groupParameter;

    ComboBox* m_comboCircleType;
    ComboBox* m_comboFrame;

    QLineEdit* m_editVel;
    QLineEdit* m_editAcc;
    QLineEdit* m_editJerk;
    QLineEdit* m_editRadius;
    PushButton* m_buttonFrame;

    QTableWidget* m_tableWayPoint;

    PushButton* m_buttonPointSource;
    PushButton* m_buttonAddPoint;
    PushButton* m_buttonInsertPoint;
    PushButton* m_buttonInsertCurrentPoint;
    PushButton* m_buttonDeletePoint;
    PushButton* m_buttonPointSourceFrame;
    QLineEdit* m_editPointName;
    QLineEdit* m_editPointValue[6];

    QComboBox* m_comboPointName;

    QGroupBox* groupWayPoint;

    PushButton* m_buttonPrevious;
    PushButton* m_buttonNext;

    PushButton* m_buttonYes;
    PushButton* m_buttonNo;

    OPERATIONMODE m_mode;
    int m_func;
    int m_line;

    rclib::RobotInterpreter* m_fileSrc;

    int m_jointRandomIndex;
    int m_terminalRandomIndex;

private slots:
    void buttonYesClicked();
    void buttonNoClicked();

    void buttonPreviousClicked();
    void buttonNextClicked();

    void buttonAddPointClicked();
    void buttonInsertPointClicked();
    void buttonInsertCurrentPointClicked();
    void buttonDeletePointClicked();

    void buttonPointSourceFrameClicked();
    void buttonPointSourceClicked();

    void radioMotionTypeChanged();

signals:
    void programOperationFinishedSignal(bool);
};


#endif
