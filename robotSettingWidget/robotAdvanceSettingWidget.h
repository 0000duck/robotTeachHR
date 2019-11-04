#ifndef ROBOTADVANCESETTINGWIDGET_H
#define ROBOTADVANCESETTINGWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMessageBox>
#include <QDoubleValidator>
#include <QComboBox>

#include "../pushButton/pushButton.h"
#include "../normalFunc/macroDefine.h"
#include "../normalFunc/globalParameter.h"
#include "../normalFunc/rwFile.h"
#include "../comboBox/comboBox.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotStructure/robotStructure.h"

#include "../robotServer/RobotStructure/RobSoft/CFileIO.hpp"

class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QLabel;
class QGroupBox;
class QMessageBox;
class RobotAdvanceSettingWidget:public QWidget{
    Q_OBJECT
public:
    RobotAdvanceSettingWidget(QWidget *parent = NULL);
    ~RobotAdvanceSettingWidget();

    void updateGroupEnabled(bool state);
    void updateAdvanceSettingWidget();
    void updateSystemState();

    void updateCurrentPosition();

    void updateRobotLight();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

private:
    QLabel* m_labelJointZero[8];
    PushButton* m_buttonJointZero[8];
    QLineEdit* m_editJointZero[8];
    PushButton* m_buttonWholeJointZero;

    PushButton* m_buttonCurrentJoint;
    QLabel* m_labelInitJoint[8];
    QLineEdit* m_editInitJoint[8];

    QLabel* m_labelReturnZeroIndex[8];
    QLineEdit* m_editReturnZeroIndex[8];
    PushButton* m_buttonDefaultIndex;

    QLabel* m_labelSetCurrentJoint[8];
    QLineEdit* m_editSetCurrentJoint[8];
    PushButton* m_buttonSetCurrentJoint;

    QLabel* m_labelJointState[8];
    PushButton* m_buttonStateLight[8];

    robsoft::Joints m_initJoint;
    robsoft::Joints m_setJoint;
    robsoft::Joints m_zeroJointIndex;
    double m_zeroJointVel;

    QGroupBox* m_groupJointZero;
    QGroupBox* m_groupInitJoint;
    QGroupBox* m_groupReturnZero;
    QGroupBox* m_groupSetCurrentJoint;
    QGroupBox* m_groupJointState;
    QGroupBox* m_groupOther;

    rclib::RobotPreference m_robotProference;
    rclib::RobotMotion m_robotMotion;

    rclib::RobotPreference m_oldRobotProference;

    rclib::SWITCHSTATE m_homeState[8];

    PushButton* m_buttonSoundSwitch;

    QComboBox* m_comboxLanguageSwitch;

private slots:
    void buttonJointZeroClicked(int);
    void buttonWholeJointZeroClicked();
    void buttonCurrentJointClicked();
    void buttonSetCurrentJointClicked();
    void buttonSetDefaultIndexClicked();

    void initJointValueChanged();
    void returnZeroValueChanged();
    void setCurrentJointValueChanged();

    void buttonSoundSwitchClicked();

    void comboxChanged();
};

#endif
