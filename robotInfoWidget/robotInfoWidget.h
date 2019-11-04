#ifndef ROBOTINFOWIDGET_H
#define ROBOTINFOWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>
#include <QDateTime>

#include <iostream>
#include <string>

#include "../normalFunc/macroDefine.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotClient/robotClientBase.h"
#include "../robotServer/RobotStructure/RobSoft/CDataStructure.hpp"

class QWidget;
class QHBoxLayout;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
class QScrollBar;
class QDateTime;
class RobotInfoWidget:public QWidget{
    Q_OBJECT
public:
    RobotInfoWidget(QWidget *parent = NULL);
    ~RobotInfoWidget();

    void updateNewInfo(rclib::INFOSTATE, QString);

private:
    void initUI();
    void initState();

private:
    QTableWidget* m_tableInfomation;
};

#endif
