#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QtWidgets/QPushButton>

#include "../normalFunc/qtNormalFunction.h"
#include "../robotServer/RobotClient/robotClient.h"

extern bool SOUND;

class PushButton;
class PushButton:public QPushButton{
    Q_OBJECT
public:
    PushButton(QWidget *parent = NULL);
    PushButton(const QString &text, QWidget *parent = NULL);
    ~PushButton();

    void setIndex(int index);
    void setEnable(bool state);

    void setStateSum(int sum);
    void setState(int state);
    void incState();
    void decState();
    int returnState();

private:
    void connectSignalSlot();

private slots:
    void pushButtonClicked();
    void pushButtonPressed();
    void pushButtonReleased();

    void pushButtonSound();

signals:
    void pushButtonClicked(int);
    void pushButtonPressed(int);
    void pushButtonReleased(int);

private:
    int m_index;
    int m_state = 0;
    int m_sum = 1;
};
#endif
