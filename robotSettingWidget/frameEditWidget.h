#ifndef FRAMEEDITWIDGET_H
#define FRAMEEDITWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QRadioButton>

#include <iostream>
#include <string>
#include <map>

#include "../pushButton/pushButton.h"

#include "../normalFunc/macroDefine.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotStructure/robotStructure.h"
#include "../robotServer/RobotStructure/RobSoft/CRobotics.hpp"


class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QLabel;
class QGroupBox;
class QListView;
class QMessageBox;
class QRadioButton;
class FrameEditWidget:public QWidget{
    Q_OBJECT
public:
    FrameEditWidget(QWidget *parent = NULL);
    ~FrameEditWidget();

    void setMode(int type, int frameType);
    void passParameter(rclib::RobotFrame *m_frame,std::string m_name="");

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void setRadioFrameModeChecked(int);
private:
    QRadioButton* m_radioFrameMode[3];
    PushButton* m_buttonJointPoint[10];
    PushButton* m_buttonPointOrigin;
    PushButton* m_buttonPointX;
    PushButton* m_buttonPointY;
    PushButton* m_buttonPointZ;

    PushButton* m_buttonCalibrate;
    PushButton* m_buttonYes;
    PushButton* m_buttonNo;

    QLabel* m_labelPre;
    QLineEdit* m_editPre;

    QLabel* m_labelName;
    QLineEdit* m_editName;
    QLabel* m_labelFrameX;
    QLabel* m_labelFrameY;
    QLabel* m_labelFrameZ;
    QLabel* m_labelFrameA;
    QLabel* m_labelFrameB;
    QLabel* m_labelFrameC;
    QLineEdit* m_editFrameX;
    QLineEdit* m_editFrameY;
    QLineEdit* m_editFrameZ;
    QLineEdit* m_editFrameA;
    QLineEdit* m_editFrameB;
    QLineEdit* m_editFrameC;

    QGroupBox* groupPoint;
    QGroupBox* groupJoint;
    QGroupBox* groupNameFrame;
    QGroupBox* groupPre;

    int m_type; //0:add 1:modify 2:calibrate
    int m_frameType;  //0:tool 1:user
    double m_pre;
    std::string m_FrameName;
    rclib::RobotFrame* m_RobotFrame;
    std::map<std::string,rclib::Terminal> m_temp;

    int m_buttonJointPointFlag[10] = {0};
    int m_buttonPointOriginFlag = 0;
    int m_buttonPointXFlag = 0;
    int m_buttonPointYFlag = 0;
    int m_buttonPointZFlag = 0;

    int m_frameModeFlag;

    rclib::Joints m_jointPoint[10];
    rclib::Joints m_jointOrigin;
    rclib::Joints m_jointX;
    rclib::Joints m_jointZ;
    rclib::Terminal m_pointOrigin;
    rclib::Terminal m_pointX;
    rclib::Terminal m_pointY;
    rclib::Terminal m_pointZ;

private slots:
    void radioFrameModeChanged();

    void buttonJointPointClicked(int);
    void buttonPointOriginClicked();
    void buttonPointXClicked();
    void buttonPointYClicked();
    void buttonPointZClicked();
    void buttonCalibrateClicked();
    void buttonYesClicked();
    void buttonNoClicked();

signals:
    void frameEditFinishedSignal(bool);
};

#endif
