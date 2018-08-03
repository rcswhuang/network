#include "hnethandle.h"
#include "hnetworkapi.h"
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

}


bool  HNetHandle::netInit()
{
    pNetMonitor = new HNetMonitor;
    pNetMonitor->hide();
    pNetMonitor->start();
    //规约也要初始化
}

void  HNetHandle::netExit()
{
    if(pNetMonitor)
        delete pNetMonitor;
}

void  HNetHandle::sendWfYk(ushort wStationIndex,ushort wYxIndex, uchar btExceptValue)
{
    //调用规约
}

void  HNetHandle::sendReqYk(ushort wStationIndex,ushort wYxIndex,uchar btExceptValue)
{

}

void  HNetHandle::sendReqAllYx(ushort wStationIndex,ushort wYxIndex)
{

}

void  HNetHandle::sendYkCheckBack(ushort wStationIndex,ushort wYxIndex,uchar btValue,bool bCheckStatus)
{

}

void  HNetHandle::sendLockAll(ushort wStationIndex,uchar btLockType)
{

}

void  HNetHandle::sendLockOne(ushort wStationIndex,ushort wYxIndex,uchar btLockType)
{

}

void  HNetHandle::sendMeasureYx(ushort* wStationIndex,ushort *wYxIndex,ushort* btYxValue,ushort wYxNum)
{

}

ushort  HNetHandle::stationIndex2Addr(ushort wStationIndex)
{

}

ushort  HNetHandle::stationAddr2Index(ushort wStaitonAddr)
{

}


////////////////////////////////////对外接口部分//////////////////////////////////
bool NETWORK_EXPORT netInit()
{
    HNetHandle::pInstance = HNetHandle::instance();
    HNetHandle::pInstance->netInit();
}

void NETWORK_EXPORT netExit()
{
    HNetHandle::pInstance->netExit();
}

void NETWORK_EXPORT sendWfYk(ushort wStationIndex,ushort wYxIndex, uchar btExceptValue)
{
    HNetHandle::pInstance->sendWfYk(wStationIndex,wYxIndex,btExceptValue);
}

void NETWORK_EXPORT sendReqYk(ushort wStationIndex,ushort wYxIndex,uchar btExceptValue)
{
    HNetHandle::pInstance->sendReqYk(wStationIndex,wYxIndex,btExceptValue);
}

void NETWORK_EXPORT sendReqAllYx(ushort wStationIndex,ushort wYxIndex)
{
    HNetHandle::pInstance->sendReqAllYx(wStationIndex,wYxIndex);
}

void NETWORK_EXPORT sendYkCheckBack(ushort wStationIndex,ushort wYxIndex,uchar btValue,bool bCheckStatus)
{
    HNetHandle::pInstance->sendYkCheckBack(wStationIndex,wYxIndex,btValue,bCheckStatus);
}

void NETWORK_EXPORT sendLockAll(ushort wStationIndex,uchar btLockType)
{
    HNetHandle::pInstance->sendLockAll(wStationIndex,btLockType);
}

void NETWORK_EXPORT sendLockOne(ushort wStationIndex,ushort wYxIndex,uchar btLockType)
{
    HNetHandle::pInstance->sendLockOne(wStationIndex,wYxIndex,btLockType);
}

void NETWORK_EXPORT sendMeasureYx(ushort* wStationIndex,ushort *wYxIndex,ushort* btYxValue,ushort wYxNum)
{
    HNetHandle::pInstance->sendMeasureYx(wStationIndex,wYxIndex,btYxValue,wYxNum);
}

ushort NETWORK_EXPORT stationIndex2Addr(ushort wStationIndex)
{
    return HNetHandle::pInstance->stationIndex2Addr(wStationIndex);
}

ushort NETWORK_EXPORT stationAddr2Index(ushort wStationAddr)
{
    return HNetHandle::pInstance->stationAddr2Index(wStationAddr);
}
