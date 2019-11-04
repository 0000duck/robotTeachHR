#ifndef ROBOTFRAMESETTINGWIDGET_H
#define ROBOTFRAMESETTINGWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QListView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QRadioButton>

#include <iostream>
#include <string>
#include <map>

#include "frameEditWidget.h"
#include "../normalFunc/macroDefine.h"

#include "../pushButton/pushButton.h"


#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotStructure/robotStructure.h"

class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
class QScrollBar;
class QListView;
class QGroupBox;
class QMessageBox;
class QRadioButton;
class FrameEditWidget;
class RobotFrameSettingWidget:public QWidget{
    Q_OBJECT
public:
    RobotFrameSettingWidget(QWidget *parent = NULL);
    ~RobotFrameSettingWidget();

    void updateOperationEnabled(bool state);

    void updateAllFrame();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void initFrameTable();

    void updateToolFrameTable();
    void updateUserFrameTable();

private:
    QRadioButton* m_radioChooseToolFrame;
    QRadioButton* m_radioChooseUserFrame;
    PushButton* m_buttonAddFrame;
    PushButton* m_buttonModifyFrame;
    PushButton* m_buttonDeleteFrame;
    PushButton* m_buttonCalibrateFrame;
    PushButton* m_buttonChooseFrame;

    QTableWidget* m_tableFrame;

    QWidget* m_widgetFrame;
    FrameEditWidget* m_widgetFrameEdit;

    int m_chooseFrameFlag = 0;

    std::string m_toolFrameName/* = "Default"*/;
    std::string m_userFrameName/* = "Default"*/;


    std::map<std::string, robsoft::Terminal> m_toolFrame;
    std::map<std::string, robsoft::Terminal> m_userFrame;

    rclib::RobotFrame m_RobotFrame;

private slots:
    void buttonAddFrameClicked();
    void buttonDeleteFrameClicked();
    void buttonModifyFrameClicked();
    void buttonCalibrateFrameClicked();
    void buttonChooseFrameClicked();

    void frameEditFinished(bool);

    void updateFrame();
};

#endif
