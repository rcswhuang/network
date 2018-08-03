#pragma once
#include <QThread>
#include "hnetworkapi.h"
class QMutex;
class HNetThread :public QThread
{
	Q_OBJECT
public:
	HNetThread(QObject* parent = 0);
	~HNetThread();
public:
	virtual void run();
public:
 /*   void addRecvDatatoMsgList(RecvData* recvdata);
    ShowMsg* getShowMsgFromList();
    void procShowMsgList();
    void procShowMsgList(RecvData* recvdata);

signals:
    void sendShowMsg(ShowMsg* msg);
private:
    QList<ShowMsg*> m_pShowMsgList;//消息队列
    QMutex m_MsgMutex;*/
};


