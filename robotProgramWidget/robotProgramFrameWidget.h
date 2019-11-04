#ifndef ROBOTPROGRAMFRAMEWIDGET_H
#define ROBOTPROGRAMFRAMEWIDGET_H

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
#include <QComboBox>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "publicstate.h"

#include "../robotProgramWidget/robotProgramFileWidget.h"

#include "../robotServer/RobotInterpreter/robotInterpreter.h"
#include "../robotServer/RobotStructure/robotStructure.h"
#include "../robotServer/RobotClient/robotClient.h"

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
class QLabel;
class QListView;
class QMessageBox;
class RobotProgramFrameWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramFrameWidget(QWidget* parent = NULL);
    ~RobotProgramFrameWidget();

    void passParameter(rclib::RobotInterpreter* p_file);

    enum OPERATIONMODE {TEACH_ADD, TEACH_MODIFY};

    void setMode(OPERATIONMODE mode);
    void passSentenceIndex(int func, int line);

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void clearState();

    void updateFrameList();

private:
    QRadioButton* m_radioToolFrame;
    QRadioButton* m_radioUserFrame;

    QGroupBox* groupFrameType;

    ComboBox* m_comboFrameName;

    PushButton* m_buttonYes;
    PushButton* m_buttonNo;

    OPERATIONMODE m_mode;
    int m_func;
    int m_line;

    rclib::RobotInterpreter* m_fileSrc;

    std::string m_toolFrameName;
    std::string m_userFrameName;

    std::map<std::string, robsoft::Terminal> m_toolFrame;
    std::map<std::string, robsoft::Terminal> m_userFrame;

    rclib::RobotFrame m_RobotFrame;

private slots:
    void radioFrameTypeChanged();

    void buttonYesClicked();
    void buttonNoClicked();

signals:
    void programOperationFinishedSignal(bool);
};


#endif
