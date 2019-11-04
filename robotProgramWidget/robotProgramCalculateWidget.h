#ifndef ROBOTPROGRAMCALCULATEWIDGET_H
#define ROBOTPROGRAMCALCULATEWIDGET_H

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
#include <QComboBox>
#include <QTimer>
#include <QCompleter>

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
class ComboBox;
class QLabel;
class QGroupBox;
class QListView;
class QMessageBox;
class RobotInterpreter;
class RobotProgramCalculateWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramCalculateWidget(QWidget* parent = NULL);
    ~RobotProgramCalculateWidget();

    void passParameter(rclib::RobotInterpreter* p_file);

    enum OPERATIONMODE {TEACH_ADD, TEACH_MODIFY};

    void setMode(OPERATIONMODE mode);
    void passSentenceIndex(int func, int line);

    int findCombox(string);

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void clearState();
    bool grammarCheck();

    void updateComboList();

    int find(QComboBox*,string);

private:
    QComboBox* m_comboVariableName;
    QComboBox* m_comboVariableLeft;
    QComboBox* m_comboVariableRight;

    ComboBox* m_comboOperator;

    PushButton* m_buttonYes;
    PushButton* m_buttonNo;

    OPERATIONMODE m_mode;
    int m_func;
    int m_line;

    rclib::RobotInterpreter* m_fileSrc;

    QObject* cache = nullptr;
    int num_cache = -1;

private slots:
    void buttonYesClicked();
    void buttonNoClicked();
    void comboOpeatorChanged(int);
signals:
    void programOperationFinishedSignal(bool);
};


#endif
