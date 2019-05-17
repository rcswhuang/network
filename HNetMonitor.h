#ifndef HNETMONITOR_H
#define HNETMONITOR_H

#include <QMainWindow>
#include "hnetworkdef.h"
class QMutex;
class QTimer;
class HMyHighligher;

/*
 * 界面显示主线程、网络线程、规约处理线程是主要部分。
 * 线程之间的通讯都是采用共享队列来处理
*/
namespace Ui {
class netMonitor;
}

class HNetMonitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit HNetMonitor(QWidget *parent = 0);
    ~HNetMonitor();
    void showMsgToTextEdit(ShowMsg* msg);
    QString getJKTypeByMsg(ShowMsg* msg);
    QString getWFTypeByMsg(ShowMsg* msg);

    void init();
public slots:
    void procShowMsgList();
public:
    HMyHighligher* h;

private:
    Ui::netMonitor *ui;
    int m_nShowMsgCount;
};

#endif // HNETMANAGER1_H
