#ifndef MACRODEFINE_H
#define MACRODEFINE_H

#include <QString>
#include <QApplication>
#include <QDir>

#define WINDOWWIDTH 1024
#define WINDOWHIGHT 768

#define WINDOWICONPATH (qApp->applicationDirPath() + QString("/config/robotIcon/")).toStdString().c_str()
#define WINDOWPROGRAMPATH (qApp->applicationDirPath() + QString("/config/robotProgram/")).toStdString().c_str()
#define WINDOWCONFIGPATH (qApp->applicationDirPath() + QString("/config/robotConfig/")).toStdString().c_str()
//#define WINDOWCACHECONFIGPATH (qApp->applicationDirPath() + QString("/config/robotConfig/robotCache.robotf")).toStdString().c_str()
#define ROBOTMODELWINDOWPATH (qApp->applicationDirPath() + QString("/robotModelWindow "))
#define WINDOWCACHECONFIGPATH_INI (qApp->applicationDirPath() + QString("/config/robotConfig/robotCache.ini"))

#define NUMPRECISION 6

#define WINDOWTEXTCOLOR "#FFFFFF"
#define BUTTONTEXTCOLOR "#FFFFFF"
#define TEXTCOLOR "#FFFFFF"
#define BACKGROUNDCOLOR "#808080"
#define BASECOLOR "#F6F5FE"
#define BUTTONCOLOR "#242424"

#endif
