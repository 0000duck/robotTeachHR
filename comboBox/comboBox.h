#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMenu>
#include <QMouseEvent>

#include <iostream>
#include <vector>

#include "../action/action.h"
#include "../normalFunc/qtNormalFunction.h"

class QWidget;
class QPushButton;
class QMenu;
class Action;
class ComboBox:public QPushButton{
    Q_OBJECT
public:
    ComboBox(QWidget *parent = NULL);
    ~ComboBox();

    void insertItem(int index, QString text);
    void clear();
    int currentIndex();
    void setCurrentIndex(int index);
    QString currentText();
    void setCurrentText(QString text);
    void setEnable(bool state);

private:
    QMenu *m_menu;
    std::vector<Action*> m_action;

    int m_index;

protected slots:
    void actionSlot(int);

signals:
    void activated(int);
};

#endif
