#pragma once
#include "hnetthread.h"
#include "hnetworkapp.h"

HNetThread::HNetThread(QObject* parent)
{
	
}
HNetThread::~HNetThread()
{
    HNetworkApp::exitinstance();
}

void HNetThread::run()
{
    //HNetworkApp::instance()->setNetThread(this);
    HNetworkApp::instance()->start();
}

/*
//可以不加锁，对方定时发送，此时处理完成
void HNetThread::addRecvDatatoMsgList(RecvData* recvdata)
{
    m_MsgMutex.lock();
    ShowMsg* showMsg = new ShowMsg;
    if(showMsg)
    {
        showMsg->recvData = new RecvData;
        memcpy(showMsg->recvData->data,recvdata->data,recvdata->len);
        showMsg->recvData->len = recvdata->len;
        showMsg->recvData->ip = recvdata->ip;
        showMsg->recvData->time = recvdata->time;
        showMsg->wMsgType = MSG_RECV;
        showMsg->wtMsgError = 0;
        m_pShowMsgList.append(showMsg);
    }
    m_MsgMutex.unlock();
}

ShowMsg* HNetThread::getShowMsgFromList()
{
    ShowMsg* showMsg = NULL;
    m_MsgMutex.lock();
    if(!m_pShowMsgList.isEmpty())
    {
        showMsg = m_pShowMsgList.takeFirst();
    }
    m_MsgMutex.unlock();
    return showMsg;
}

//定时发送处理消息(采用互斥)
void HNetThread::procShowMsgList()
{
    ShowMsg* showMsg = getShowMsgFromList();
    //还是用指针来操作
    emit sendShowMsg(showMsg);

}

//直接发送
void HNetThread::procShowMsgList(RecvData* recvdata)
{
    ShowMsg* showMsg = new ShowMsg;
    if(showMsg)
    {
        showMsg->recvData = new RecvData;
        memcpy(showMsg->recvData->data,recvdata->data,recvdata->len);
        showMsg->recvData->len = recvdata->len;
        showMsg->recvData->ip = recvdata->ip;
        showMsg->recvData->time = recvdata->time;
        showMsg->wMsgType = MSG_RECV;
        showMsg->wtMsgError = 0;
        emit sendShowMsg(showMsg);

        if(showMsg)
        {
            if(showMsg->recvData->data)
                delete showMsg->recvData->data;
            delete showMsg->recvData;
            delete showMsg;
        }
    }
}*/



