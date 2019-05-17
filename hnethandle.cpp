#include "hnethandle.h"
#include "hnetworkexport.h"
#include <QList>


HNetHandle* HNetHandle::pInstance = NULL;

HNetHandle* HNetHandle::instance()
{
    if(pInstance == NULL)
    {
        pInstance = new HNetHandle();
    }
    return pInstance;
}

HNetHandle::HNetHandle()
{
    m_pNetThread = new HNetThread;
    m_pNetThread->start();
    m_pNetMonitor = new HNetMonitor;
    m_pNetMonitor->hide();
}


bool HNetHandle::WFNetInit()
{
    return false;
}

void HNetHandle::WFNetEnd()
{

}

void HNetHandle::sendLockAll(ushort wStationIndex,uchar btLockType)
{

}

void HNetHandle::sendLockOne(ushort wStationIndex,uchar btYxIndex,uchar btLockType)
{

}



////////////////////////////////////对外接口部分//////////////////////////////////
/*
bool NET_EXPORT WFNetInit()
{
    return HNetHandle::instance()->WFNetInit();
}

void NET_EXPORT WFNetEnd()
{
    HNetHandle::instance()->WFNetEnd();
}

void NET_EXPORT sendLockAll(ushort wStationIndex,uchar btLockType)
{
    if(NULL == HNetHandle::pInstance)
        return;
    HNetHandle::instance()->sendLockAll(wStationIndex,btLockType);
}

void NET_EXPORT sendLockOne(ushort wStationIndex,uchar btYxIndex,uchar btLockType)
{
    if(NULL == HNetHandle::pInstance)
        return;
    HNetHandle::instance()->sendLockOne(wStationIndex,btYxIndex,btLockType);
}
*/
