#include "robotModel.h"
#include "../normalFunc/rwFile.h"
#include <string>
#include <QDebug>

#include<sys/types.h>
#include<unistd.h>

using namespace rclib;
std::string ip;
int port;

RobotModel::RobotModel(QWidget *parent):QWidget(parent){
    this->resize(500,450);
    long PID = getpid();
    qDebug()<<PID;
    int a = this->winId();
    str = ROBOTMODELWINDOWPATH + QString::number(a) + " " + QString::fromStdString(ip) + " " + QString::number(port) + " " + QString::number(PID);
    qDebug() << str.toStdString().c_str() << endl;
    p.start(str);
}

RobotModel::~RobotModel(){
    qDebug() << "close" << endl;
    p.kill();
}

