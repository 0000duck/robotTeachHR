#ifndef ROBOTPROGRAMFILEWIDGET_H
#define ROBOTPROGRAMFILEWIDGET_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QFileDialog>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QListWidget>
#include <QFileInfo>
#include <QStack>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <fstream>

#include "../robotServer/RobotClient/robotClient.h"
#include "../robotServer/RobotInterpreter/robotInterpreter.h"
#include "../robotServer/RobotInterpreter/functions.h"

#include "../normalFunc/macroDefine.h"
#include "../normalFunc/qtNormalFunction.h"
#include "../normalFunc/rwFile.h"

#include "../pushButton/pushButton.h"

class QDialog;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class PushButton;
class QLineEdit;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
class QScrollBar;
class QLabel;
class QMessageBox;
class QDebug;
class QListView;
class QFileDialog;

class RobotProgramFileWidget:public QWidget{
    Q_OBJECT
public:
    RobotProgramFileWidget(QWidget* parent = NULL);
    ~RobotProgramFileWidget();

    void passParameter(rclib::RobotInterpreter* p_file,
                       std::string* p_fileName);

    enum FILEOPERATIONMODE {TEACH_NEWFILE, TEACH_OPENFILE, TEACH_SAVEFILE, TEACH_USB};

    void setMode(FILEOPERATIONMODE mode);

    void showFileInfoList(QFileInfoList*);

private:
    void initUI();
    void initState();
    void connectSignalSlot();

    void slotShow(QDir);

private:
    QTableWidget* m_tableProgramFileList;
    QLabel* m_labelProgramFileName;
    QLineEdit* m_editProgramFileName;
    PushButton* m_buttonYes;
    PushButton* m_buttonNo;
    PushButton* m_buttonDelete;
    PushButton* m_buttonUSBCopy;
    PushButton* m_buttonUSBSave;

    FILEOPERATIONMODE m_mode;

    rclib::RobotInterpreter* m_fileSrc;

    std::string* m_fileName;

    QStringList m_files;

    QWidget* m_widgetUSB;
    QListWidget* m_widgetList;
    QFileInfoList* m_listInfo;

    QLineEdit* m_filePath;

    QPushButton* m_buttonReturn;
    QPushButton* m_buttonEnter;
    QPushButton* m_buttonSelect;
    QPushButton* m_buttonCancel;

    QStack<QString> m_stackPath;

    bool CopyToUSB;
    QString srcPath;
    QString dstPath;

    int m_currentRowIndex = 0;

private slots:
    void buttonYesClicked();
    void buttonNoClicked();
    void buttonDeleteClicked();
    void buttonUSBCopyClicked();
    void buttonUSBSaveClicked();

    void updateProgramFileList();

    void buttonReturnClicked();
    void buttonEnterClicked();
    void buttonSelectClicked();
    void buttonCancelClicked();

    void slotDirShow(QListWidgetItem*);

signals:
    void programOperationFinishedSignal(bool);
    void showRecomInfoSignal(QString);
};

#endif
