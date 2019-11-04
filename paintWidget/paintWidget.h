#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>

#include "../normalFunc/macroDefine.h"
#include "../normalFunc/globalParameter.h"
#include "../normalFunc/rwFile.h"
#include "../pushButton/pushButton.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotClient/robotClientBase.h"
#include "../robotServer/RobotStructure/robotStructure.h"
#include "../robotServer/RobotStructure/RobSoft/CPolynomial.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class QWidget;
class QLabel;
class PushButton;
class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;
class QMessageBox;
class PushButton;
class QMouseEvent;
class QPainter;
class QPoint;
class Terminal;
class PaintWidget:public QWidget{
    Q_OBJECT
public:
    PaintWidget(QWidget *parent = NULL);
    ~PaintWidget();

    void updatePaintWidget();

    void passState(rclib::SYSPLAYSTATE,rclib::SWITCHSTATE);
    void updateSystemState();

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    PushButton* m_buttonClearPaint;
    PushButton* m_buttonMovePaint;
    QLineEdit* m_editRatio;

    rclib::SYSPLAYSTATE local_playStateFlag;
    rclib::SWITCHSTATE local_controlStateFlag;

private slots:
    void buttonClearPaint();
    void buttonMovePaint();
    void ratioConfigChanged();

    static void* moveThread(void*);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);
};

#endif
