#ifndef ROBOTTEACHWIDGET_H
#define ROBOTTEACHWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListView>
#include <QCloseEvent>

#include "../QCustomPlot/qcustomplot.h"

#include "../mainWindow/mainWindow.h"
#include "../robotModel/robotModel.h"
#include "../pushButton/pushButton.h"
#include "../normalFunc/macroDefine.h"
#include "../normalFunc/rwFile.h"

#include "../robotServer/RobotStructure/robotStructure.h"
#include "../robotServer/RobotClient/robotClient.h"

class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class QGroupBox;
class QListView;
class PushButton;
class RobotModel;
class MainWindow;
class RobotTeachWidget:public QWidget{
    Q_OBJECT
public:
    RobotTeachWidget(QWidget *parent = NULL);
    ~RobotTeachWidget();

    void setMoveSpace(int);

    void passState(rclib::SYSPLAYSTATE,rclib::SWITCHSTATE);
    void updateSystemState();

    void updateRobotMotionWidget();

    void messagesCloseEvent();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

private:
    QWidget* m_widgetBackGround;

    QWidget* m_seriesCheck;

    QLabel* m_x_color;
    QLabel* m_y_color;
    QLabel* m_z_color;
    QLabel* m_a_color;
    QLabel* m_b_color;
    QLabel* m_c_color;

    QLabel* m_x_text;
    QLabel* m_y_text;
    QLabel* m_z_text;
    QLabel* m_a_text;
    QLabel* m_b_text;
    QLabel* m_c_text;

    QCheckBox* m_x_check;
    QCheckBox* m_y_check;
    QCheckBox* m_z_check;
    QCheckBox* m_a_check;
    QCheckBox* m_b_check;
    QCheckBox* m_c_check;


    QCustomPlot* m_chart;

    QPointer<QCPGraph> m_x_Graph;
    QPointer<QCPGraph> m_y_Graph;
    QPointer<QCPGraph> m_z_Graph;
    QPointer<QCPGraph> m_a_Graph;
    QPointer<QCPGraph> m_b_Graph;
    QPointer<QCPGraph> m_c_Graph;

    RobotModel *m_robotModel;

    QWidget *m_widgetTeach;

    QWidget* m_robotCircular;

    QLabel* m_labelCurrentJointType;
    QLabel* labelCurrentTerminal;
    QLabel* labelCurrentUserTerminal;
    QLabel* m_labelCurrentJoint[6];
    QLabel* m_labelCurrentTerminal[6];
    QLabel* m_labelCurrentUserTerminal[6];

    QLabel* m_currentToolFrameName;
    QLabel* m_ToolFrameName;
    QLabel* m_currentWorkFrameName;
    QLabel* m_WorkFrameName;

    PushButton* m_buttonReturnZero;
    PushButton* m_buttonCalibrateZero;

    int m_moveSpace = 0;

    rclib::SYSPLAYSTATE local_playStateFlag;
    rclib::SWITCHSTATE local_controlStateFlag;

    bool JUD;

    QWidget* m_widgetMessageBox;
    QLabel* m_labelMessagebox;
    PushButton* m_buttonDeafultVel;
    PushButton* m_buttonCurrntVel;
    PushButton* m_buttonCancel;
private slots:
    void buttonReturnZeroClicked();
    void buttonCalibrateZeroClicked();

    void buttonDeafultClicked();
    void buttonCurrentClicked();
    void butonCancelClicked();
};

#endif
