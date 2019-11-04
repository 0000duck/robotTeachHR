#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "mainWindow/mainWindow.h"
#include "normalFunc/rwFile.h"
#include "frmInput/frminput.h"

using namespace std;
extern std::string ip;
extern int port;

int main(int argc, char* argv[]){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));   // support chinese

    QApplication app(argc, argv);
    QFont font = app.font();
    font.setPointSize(10);
    app.setFont(font);

    QPalette pe;
    pe.setColor(QPalette::WindowText, WINDOWTEXTCOLOR);
    pe.setColor(QPalette::ButtonText, BUTTONTEXTCOLOR);
    pe.setColor(QPalette::Button, BUTTONCOLOR);   // 按键
    pe.setColor(QPalette::Background, BACKGROUNDCOLOR);   // 背景
    pe.setColor(QPalette::Base, BASECOLOR); // 可填充文字的控件
    app.setPalette(pe);

    ip = "127.0.0.1";
    port = 8080;

    if(argc == 3){
        ip = QString(argv[1]).toStdString();

        port = QString(argv[2]).toInt();
    }

    if(!rclib::RobotClient::initance()->initSystem(ip.c_str(),port))
    {
        QMessageBox::warning(nullptr, QObject::tr("错误"), QObject::tr("与服务端连接失败"));
        app.quit();
        return -1;
    }

    QString language;
    read_cache_language(language);

    string path;
    if( language == QString("English")){
        path = qApp->applicationDirPath().toStdString() + string("/config/robotConfig/en.qm");
    }
    else{
        path = qApp->applicationDirPath().toStdString() + string("/config/robotConfig/zh.qm");
    }

    QTranslator translator;
    translator.load(QString::fromStdString(path));
    qApp->installTranslator(&translator);

    frmInput::Instance()->Init();
    MainWindow* mainwindow = new MainWindow();

    frmInput::Instance()->setParent(mainwindow);
    frmInput::Instance()->setMain(mainwindow);
    frmInput::Instance()->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
    mainwindow->show();

    return app.exec();
}
