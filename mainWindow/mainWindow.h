#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>
#include <QProcess>
#include <QTimer>

#include <iostream>
#include <string>
#include <map>

#include "../normalFunc/globalParameter.h"
#include "../normalFunc/qtNormalFunction.h"
#include "../robotTeachWidget/robotTeachWidget.h"
#include "../robotProgramWidget/robotProgramWidget.h"
#include "../robotSettingWidget/robotSettingWidget.h"
#include "../robotIOWidget/robotIOWidget.h"
#include "../robotInfoWidget/robotInfoWidget.h"
#include "../customTabStyle/customTabStyle.h"

#include "../pushButton/pushButton.h"
#include "../paintWidget/paintWidget.h"

#include "../normalFunc/macroDefine.h"
#include "../normalFunc/rwFile.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotClient/robotClientBase.h"
#include "../robotServer/RobotStructure/robotStructure.h"

class QDialog;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QGroupBox;
class QLabel;
class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
class QScrollBar;
class QListView;
class QMessageBox;
class QProcess;
class QTimer;
class RobotTeachWidget;
class RobotSettingWidget;
class RobotIOWidget;
class RobotInfoWidget;
class RobotProgramWidget;
class SoftAboutWidget;
class ForShowWidget;
class UpdateStateThread;
class CustomTabStyle;
class SystemOperationWidget;
class PushButton;
class PaintWidget;
class MainWindow:public QDialog{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
    void showEvent(QShowEvent *);

private:
    void updateTeachPlayMode(rclib::SYSPLAYSTATE);  //0:teach 1:play
    void updateNewInfo(rclib::INFOSTATE, string);
    void updateRobotState();
    void updateSendState(rclib::SYSRUNSTATE);
    void updateDebugState(rclib::SWITCHSTATE);
    void updateServoState(rclib::SWITCHSTATE);

    void updateTeachPlayButton();
    void updateMainProgram();
    void updateMainMotion();

    void updateVelSlider();
    void updateDebugButton();
    void updateStepButton();
    void updateMoveModeButton();

    void updateClearErrorButton();

    RobotIOWidget* m_widgetRobotIO;
    RobotInfoWidget* m_widgetRobotInfo;
    RobotTeachWidget* m_widgetRobotTeach;
    RobotSettingWidget* m_widgetRobotSetting;
    RobotProgramWidget* m_widgetRobotPorgram;
    SoftAboutWidget* m_widgetSoftAbout;
    PaintWidget* m_widgetPaint;

private:
    void initUI();
    void initState();
    void initParameter();
    void connectSignalSlot();

    void initNewInfoTable();
    void updateCurrentTimeShow();

    bool is_str_utf8(const char*);

private:
    QLabel* m_version;
    PushButton* m_buttonShutDownSystem;

    QTabWidget* m_mainFunc;

    PushButton *m_buttonMoveMode;
    PushButton* m_buttonJointMinus[6];
    PushButton* m_buttonJointPlus[6];
    QLabel* m_labelJoint[6];
    QLabel *m_labelStepDistance;
    QLineEdit *m_editStepDistance;
    PushButton* m_buttonMoveSpace;

    PushButton* m_buttonProgramRun;
    PushButton* m_buttonProgramStepMode;
    PushButton* m_buttonProgramStep;
    PushButton* m_buttonProgramBack;
    PushButton* m_buttonProgramStepPause;
    PushButton* m_buttonPauseContinue;
    PushButton* m_buttonStop;

    PushButton* m_buttonClearError;
    PushButton* m_buttonStartDragTeach;
    PushButton* m_buttonEndDragTeach;
    PushButton* m_buttonKeyboard;

    QPixmap m_pixLogo;
    QLabel* m_labelLogo;
    QWidget* m_widgetLogo;

    QPixmap m_pixScreenSaverLogo;
    QLabel* m_labelScreenSaverLogo;
    QWidget* m_widgetScreenSaver;

    PushButton* m_buttonTeachPlayMode;
    PushButton* m_buttonNormalDebugMode;
    PushButton* m_buttonVirtualMahcineMode;
    PushButton* m_buttonServo;

    QLabel* m_labelVel;
    QSlider* m_sliderVel;
    QLineEdit* m_editVel;
    PushButton* m_buttonVelMinus;
    PushButton* m_buttonVelPlus;

    QWidget* m_widgetMainMotion;
    QWidget* m_widgetMainProgram;
    QWidget* m_widgetMainMenu;
    QWidget* m_widgetMainMode;
    QWidget* m_widgetMainFunc;

    QTableWidget* m_tableNewInformation;
    QLabel* m_labelTime;

    QTimer* m_timer;

    rclib::RobotState local_State;
    rclib::SYSRUNSTATE local_runStateFlag;         //运行状态
    rclib::SYSPLAYSTATE local_playStateFlag;       //运行模式
    rclib::SWITCHSTATE local_servoStateFlag;       //伺服状态
    rclib::SWITCHSTATE local_virtualStateFlag;     //虚拟模式
    rclib::SWITCHSTATE local_debugStateFlag;       //调试模式
    rclib::SWITCHSTATE local_controlStateFlag;

    rclib::RobotState m_RobotState;
    rclib::SYSRUNSTATE m_runStateFlag;         //运行状态
    rclib::SYSPLAYSTATE m_playStateFlag;       //运行模式
    rclib::SWITCHSTATE m_servoStateFlag;       //伺服状态
    rclib::SWITCHSTATE m_virtualStateFlag;     //虚拟模式
    rclib::SWITCHSTATE m_debugStateFlag;       //调试模式
    rclib::SWITCHSTATE m_controlStateFlag;

    bool m_stepMode = false;

private slots:
    void updateAllState();     // update all status

    void showRecomInfoSlot(QString);
    void showWarnInfoSlot(QString);
    void showErrorInfoSlot(QString);

    void buttonProgramRunClicked();
    void buttonProgramStepModeClicked();
    void buttonProgramBackClicked();
    void buttonProgramStepClicked();
    void buttonProgramStepPauseClicked();
    void buttonPauseContinueClicked();
    void buttonStopClicked();
    void buttonServoClicked();
    void buttonClearErrorClicked();
    void buttonStartDragTeachClicked();
    void buttonEndDragTeachClicked();

    void buttonTeachPlayModeClicked();
    void buttonNormalDebugModeClicked();
//    void buttonVirtualMahcineModeClicked();
    void buttonMoveModeClicked();

    void sliderVelChanged();
    void buttonVelMinusClicked();
    void buttonVelPlusClicked();

    void buttonMoveSpaceClicked();
    void buttonJointMinusPressed(int);
    void buttonJointPlusPressed(int);
    void buttonJointReleased(int);

    void buttonshutdownSystemClicked();

    void RobotStateChange();

signals:
    void signalRobotStateChange();

private:
    void jointTerminalJogStep(int index, rclib::MOVEDIRECTION direction);
};

#endif
