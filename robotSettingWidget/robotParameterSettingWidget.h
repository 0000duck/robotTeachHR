#ifndef ROBOTPARAMETERSETTINGWIDGET_H
#define ROBOTPARAMETERSETTINGWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTabWidget>

#include "../comboBox/comboBox.h"
#include "../normalFunc/macroDefine.h"
#include "../normalFunc/globalParameter.h"
#include "../pushButton/pushButton.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotClient/robotClientBase.h"
#include "../robotServer/RobotStructure/robotStructure.h"


class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QLabel;
class QGroupBox;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
class QScrollBar;
class QListView;
class QMessageBox;
class QTabWidget;
class RobotParameterSettingWidget:public QWidget{
    Q_OBJECT
public:
    RobotParameterSettingWidget(QWidget *parent = NULL);
    ~RobotParameterSettingWidget();

    void updateSystemState();
    void updateOperationEnabled(bool state);

    void updateRobotParameterWidget();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void initMotionParameterTable();
    void initRatioParameterTable();
    void initJointParameterTable();
    void initModelParameterTable();

    void fillRatioParameterTableContent();
    void fillMotionParameterTableContent();
    void fillJointParameterTableContent();
    void fillRatioParameterTableValue();
    void fillMotionParameterTableValue();
    void fillJointParameterTableValue();
    void fillModelParameterTableValue();
    void getRatioParameterTableValue();
    void getMotionParameterTableValue();
    void getJointParameterTableValue();
    void getModelParameterTableValue();

private:
    QTabWidget* m_tabRobotParameter;

    QLabel* m_labelRobotType;
    ComboBox* m_comboRobotType;

    QLabel* m_labelRobotPeriod;
    QLineEdit* m_editRobotPeriod;

    QGroupBox* m_groupNormalParameter;

    QTableWidget* m_tableMotionParameter;
    QTableWidget* m_tableRatioParameter;
    QTableWidget* m_tableJointParameter;
    QTableWidget* m_tableModelParameter;

    PushButton* m_buttonSaveRobotParameter;

    rclib::RobotParameter m_robot;
    rclib::ROBOTTYPE m_robotType;

private slots:
    void buttonSaveRobotParameterClicked();
    void comboRobotTypeChanged(int);
};

#endif
