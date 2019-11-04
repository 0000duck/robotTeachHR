#ifndef ACTION_H
#define ACTION_H

#include <QAction>

class Action;
class Action:public QAction{
    Q_OBJECT
public:
    Action(QWidget *parent = NULL);
    ~Action();

    void setIndex(int index);

private:
    void connectSignalSlot();

    int m_index;

private slots:
    void actionSlot();

signals:
    void triggeredIndex(int index);
};

#endif
