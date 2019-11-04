#ifndef ROBOTMODEL_H
#define ROBOTMODEL_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QRadioButton>
#include <QTimer>
#include <QProcess>

#include "../normalFunc/macroDefine.h"
#include "../pushButton/pushButton.h"

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotStructure/robotStructure.h"

class RobotModelWidget;
class PushButton;
class QRadioButton;
class QVBoxLayout;
class QHBoxLayout;
class RobotTeachWidget;
class PushButton;
class QRadioButton;
class QVBoxLayout;
class QHBoxLayout;
class RobotModel : public QWidget{
    Q_OBJECT
public:
    RobotModel(QWidget *parent = 0);
    ~RobotModel();
    QProcess p;
    QString str;
};

#endif
