#ifndef HNETMONITOR_H
#define HNETMONITOR_H

#include <QMainWindow>
#include "networkapi.h"
class QMutex;
class QTimer;
class HMyHighligher;
class HNetThread;
/*
 * 显示消息来源是网络线程发过来，注意线程把经过处理后消息直接发过来
 * 采用主进程和线程之间公用链表的方式来处理
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
    void procShowMsgList();
    QString getJKTypeByMsg(ShowMsg* msg);
    QString getWFTypeByMsg(ShowMsg* msg);

    void init();
public:
    QTimer* refreshTimer;
    HMyHighligher* h;
    HNetThread* m_pNetThread;
private:
    Ui::netMonitor *ui;
    int m_nShowMsgCount;
};

#endif // HNETMANAGER1_H
