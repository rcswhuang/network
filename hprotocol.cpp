#include "hprotocol.h"
#include "hkernelapi.h"
/*
 * 规约解析模块，以后所有的其他规约，都可以用这种模式来进行构建
 * 不需要考虑网络层的转发情况
 * 也可以把规约解析模块独立出来做一个，这样可能更好一点。
 * 这个时候需要把规约层和网络层的关系要独立开。特别是多个连接的时候。
 * 这里是把规约层和网络层写在一起了。缺点是每个新规约都要拷贝重复的网络层代码
*/
extern void add_msg_for_show(unsigned short type, RecvData* data,std::string info);

HProtocol* HProtocol::m_pInstance = NULL;

HProtocol* HProtocol::instance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new HProtocol;
        //m_pInstance->loadVirtualYx();
    }

    return m_pInstance;
}


HProtocol::HProtocol()
{
    //这里需要加载发送虚遥信的配置信息

}

HProtocol::~HProtocol()
{
    for(int i = 0; i < p_sendToScadaYXStationCounts;i++)
    {
        while((p_sendToScadaYXList+i)->size() > 0)
            (p_sendToScadaYXList+i)->removeFirst();
    }

    if(p_sendToScadaYXList != NULL)
        delete[] p_sendToScadaYXList;
}

void HProtocol::start()
{
    loadVirtualYx();
    wSendVYXTimes = 0;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerProcessor()));
    timer->start(1000);
}

void HProtocol::loadVirtualYx()
{
    p_sendToScadaYXStationCounts = 0;
    int nIndex = 0;
    DBHANDLE dbHandle = ::findDbHandle(0,TYPE_NULL,nIndex++);
    while(::isValidDbHandle(dbHandle))
    {
        p_sendToScadaYXStationCounts++;
        dbHandle = ::findDbHandle(0,TYPE_NULL,nIndex++);
    }


    if(p_sendToScadaYXStationCounts > 0)
    {
        p_sendToScadaYXList = new QUshortArray[p_sendToScadaYXStationCounts];
        p_sendToScadaStationList = new ushort[p_sendToScadaYXStationCounts];
        nIndex = 0;
        dbHandle = ::findDbHandle(0,TYPE_NULL,nIndex++);
        while(::isValidDbHandle(dbHandle))
        {
            ushort wStationNo;
            ::getAttr(dbHandle,ATTR_ST_NO,&wStationNo);
            *(p_sendToScadaStationList+nIndex-1)= wStationNo;

            int index = 0;
            DBHANDLE handle;
            handle = ::findDbHandle(wStationNo,TYPE_DIGITAL,index++);
            while(::isValidDbHandle(handle))
            {
                ushort wDgtNo;
                ushort wDgtIndex = (ushort)-1;
                ::getAttr(handle,ATTR_DGT_TOSCADAINDEX,&wDgtIndex);
                if(wDgtIndex != (ushort)-1)
                {
                   ::getAttr(handle,ATTR_DGT_NO,&wDgtNo);
                    //按照索引的位置进行存放遥信ID
                   if((p_sendToScadaYXList+nIndex-1)->size() == wDgtIndex)
                   {
                       (p_sendToScadaYXList+nIndex-1)->append(wDgtNo);
                   }
                   else if((p_sendToScadaYXList+nIndex-1)->size() < wDgtIndex)
                   {
                       for(int i = (p_sendToScadaYXList+nIndex-1)->size();i < wDgtIndex;i++)
                           (p_sendToScadaYXList+nIndex-1)->append(wDgtNo);//把size到index插入同样值，后面需要替换
                   }
                   else
                   {
                       (p_sendToScadaYXList+nIndex-1)->replace(wDgtIndex,wDgtNo);
                   }
                }
                handle = ::findDbHandle(wStationNo,TYPE_DIGITAL,index++);
            }
            dbHandle = ::findDbHandle(0,TYPE_NULL,nIndex++);
        }
    }

}

void HProtocol::handleReceive(RecvData* recvData)
{
    if(!recvData) return;
    add_msg_for_show(MSG_APP_RECV,recvData,"no");
    char* pStart = recvData->data;
    uchar funNo = *(uchar*)pStart;
    pStart += sizeof(uchar);
    ushort length = *(ushort*)pStart;
    pStart+= sizeof(ushort);
    switch(funNo)
    {
    case 0x85: processHand(pStart,length);break;
    case 0x86: processAllYx(pStart,length);break;
    case 0x87: processChangeYx(pStart,length);break;
    case 0x88: processYKCheck(pStart,length);break;
    case 0x91: processAllYc(pStart,length);break;
    case 0x96: processTimer(pStart,length);break;
    }

}

void HProtocol::handleSend(char* pData,int length)
{
    //HNetApp::instance()->handle_send(pData,length);
}

void HProtocol::handleSend(RecvData* recvData)
{
    //如果规约处理较多，需要互斥
    //HNetApp::instance()->handle_send(recvData);
    add_msg_for_show(MSG_APP_SEND,recvData,"no");
}

//hand
void HProtocol::processHand(char* pData,int length)
{
    if(!pData) return;
    char* pStart = pData;
    uchar btType = *(uchar*)pStart;
    if(btState == 0x00 && 0x01 == btType)
    {
        int length = 5;
        char* pSndData = new char[length];
        char* pSndStart = pSndData;
        *(uchar*)pSndStart = 0x85;
        pSndStart += sizeof(uchar);
        *(ushort*)pSndStart = 0x02;
        pSndStart += sizeof(ushort);
        *(uchar*)pSndStart = 0x00;
        pSndStart += sizeof(uchar);
        *(uchar*)pSndStart = 0x03;
        pSndStart += sizeof(uchar);

        handleSend(pSndData,length);
        if(pSndData)
        {
            delete pSndData;
            pSndData = NULL;
        }
    }
}

void HProtocol::processAllYx(char* pData,int length)
{
    if(!pData) return;
    char* pStart = pData;
    ushort wYXFrame = length - 2;
    ushort addr = *(ushort*)pStart;
    ushort wStationIndex = stationAddr2Index(addr);
    pStart += sizeof(ushort);
    ushort wYXIndex = (ushort)-1;
    uchar btValue;
    for(ushort i = 0; i < wYXFrame; i++)
    {
        for(uchar j = 0; j < 8; j++)
        {
            wYXIndex++;
            btValue=((pStart[i]&(0x01<<j))!=0);
            kernelEnterDB(TYPE_DIGITAL,wStationIndex,wYXIndex,&btValue);
        }
    }
}

void HProtocol::processChangeYx(char* pData,int length)
{
    if(!pData) return;
    char* pStart = pData;
    ushort wYXNum = (length - 2)/5;
    if((ushort)-1 != wYXNum)
        return;
    ushort addr = *(ushort*)pStart;
    ushort wStationIndex = stationAddr2Index(addr);
    pStart += sizeof(ushort);
    ushort wYXIndex = *(ushort*)pStart;
    pStart += sizeof(ushort);
    uchar btValue = *(uchar*)pStart;
    kernelEnterDB(TYPE_DIGITAL,wStationIndex,wYXIndex,&btValue);
}

void HProtocol::processAllYc(char* pData,int length)
{
    if(!pData) return;
    char* pStart = pData;
    ushort wYCNum = (length - 2)/sizeof(float);
    ushort addr = *(ushort*)pStart;
    ushort wStationIndex = stationAddr2Index(addr);
    pStart += sizeof(ushort);
    float fValue;
    for(ushort i = 0; i < wYCNum; i++)
    {
        fValue = *(float*)pStart;
        kernelEnterDB(TYPE_ANALOGUE,wStationIndex,i,&fValue);
        pStart += sizeof(float);
    }
}

void HProtocol::processYKCheck(char* pData,int length)
{

}

void HProtocol::processTimer(char* pData,int length)
{

}


//send：规约报文是厂站地址
void HProtocol::sendHeartBeat()
{
    int length = 8;
    char* pSndData = new char[length];
    char* pSndStart = pSndData;
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(uchar*)pSndStart = 0x85;
    pSndStart += sizeof(uchar);
    *(uchar*)pSndStart = 0x02;
    pSndStart += sizeof(uchar);
    *(uchar*)pSndStart = 0x00;
    pSndStart += sizeof(uchar);
    *(uchar*)pSndStart = 0x03;
    pSndStart += sizeof(uchar);

    handleSend(pSndData,length);
    if(pSndData)
    {
        delete pSndData;
        pSndData = NULL;
    }
}

void HProtocol::sendAllVYx()
{
    if(p_sendToScadaYXStationCounts <= 0)
        return;

    for(int wStationIndex = 0; wStationIndex < p_sendToScadaYXStationCounts;wStationIndex++)
    {
        if((p_sendToScadaYXList + wStationIndex)->size() <= 0)
            continue;
        ushort wYxBytes = ((p_sendToScadaYXList + wStationIndex)->size() + 7)/8;
        ushort wLength = 9 + wYxBytes;
        char* pData = new char[wLength];
        char* pStart = pData;
        *(ushort*)pStart = 0xEB90;
        pStart += sizeof(ushort);
        *(ushort*)pStart = 0xEB90;
        pStart += sizeof(ushort);
        *(uchar*)pStart = 0x86;
        pStart += sizeof(uchar);
        *(ushort*)pStart = 2 + wYxBytes;
        pStart += sizeof(ushort);
        *(ushort*)pStart = stationIndex2Addr(wStationIndex);
        pStart += sizeof(ushort);

        //get yx value
        DBHANDLE dbHandle;
        uchar btValue;
        ushort wYxIndex = (ushort)-1;
        for(ushort i = 0; i < wYxBytes;i++)
        {
            pStart[i] = 0;
            for(uchar j = 0; j < 8;j++)
            {
                wYxIndex++;
                dbHandle = getDbHandle(*(p_sendToScadaStationList + wStationIndex),TYPE_DIGITAL,(p_sendToScadaYXList+wStationIndex)->at(wYxIndex));
                if(isValidDbHandle(dbHandle))
                {
                    getAttr(dbHandle,ATTR_DGT_VALUE,&btValue);
                    if(btValue)
                        pStart[i] |= 0x01 << j;
                }
            }
        }
        handleSend(pData,wLength);
        if(pData)
        {
            delete pData;
            pData = NULL;
        }
    }
}

void HProtocol::sendReqAllYx(ushort wStationIndex,ushort wYxIndex)
{

}

void HProtocol::sendYKCheckBack(ushort wStationIndex,ushort wYxIndex,uchar btValue,bool bCheck)
{
    if(wStationIndex == (ushort)-1 || (wYxIndex) == (ushort)-1)
        return;
    int length = 12;
    char* pSndData = new char[length];
    char* pSndStart = pSndData;
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);

    *(uchar*)pSndStart = 0x88;
    pSndStart += sizeof(uchar);
    *(ushort*)pSndStart = 0x05;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = stationIndex2Addr(wStationIndex);
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = wYxIndex;
    pSndStart += sizeof(ushort);
    if(bCheck)
    {
        if(0xCC == btValue)
            *(uchar*)pSndStart = 0xAA;
        else
            *(uchar*)pSndStart = 0x55;
    }
    else
        *(uchar*)pSndStart = 0xFF;

    handleSend(pSndData,length);
    if(pSndData)
    {
        delete pSndData;
        pSndData = NULL;
    }
}

//五防机直接遥控
void HProtocol::sendWFYK(ushort wStationIndex,ushort wYxIndex,uchar btValue)
{
    if(wStationIndex == (ushort)-1 || (wYxIndex) == (ushort)-1)
        return;
    int length = 12;
    char* pSndData = new char[length];
    char* pSndStart = pSndData;
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(uchar*)pSndStart = 0x89;
    pSndStart += sizeof(uchar);
    *(ushort*)pSndStart = 0x05;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = stationIndex2Addr(wStationIndex);
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = wYxIndex;
    pSndStart += sizeof(ushort);
    if(1 == btValue)
    {
        *(uchar*)pSndStart = 0xCC;
    }
    else if(0 == btValue)
    {
        *(uchar*)pSndStart = 0x33;
    }
    else
    {
        if(pSndData)
        {
            delete pSndStart;
            pSndStart = NULL;
            return;
        }
    }

    handleSend(pSndData,length);
    if(pSndData)
    {
        delete pSndData;
        pSndData = NULL;
    }
}

//五防机请求监控遥控
void HProtocol::sendReqYK(ushort wStationIndex,ushort wYxIndex,uchar btValue)
{
    if(wStationIndex == (ushort)-1 || (wYxIndex) == (ushort)-1)
        return;
    int length = 12;
    char* pSndData = new char[length];
    char* pSndStart = pSndData;
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(uchar*)pSndStart = 0x90;
    pSndStart += sizeof(uchar);
    *(ushort*)pSndStart = 0x05;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = stationIndex2Addr(wStationIndex);
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = wYxIndex;
    pSndStart += sizeof(ushort);
    if(1 == btValue)
    {
        *(uchar*)pSndStart = 0xCC;
    }
    else if(0 == btValue)
    {
        *(uchar*)pSndStart = 0x33;
    }
    else
    {
        if(pSndData)
        {
            delete pSndStart;
            pSndStart = NULL;
            return;
        }
    }

    handleSend(pSndData,length);
    if(pSndData)
    {
        delete pSndData;
        pSndData = NULL;
    }
}

void HProtocol::sendLockAll(ushort wStationIndex,uchar btLock)
{
    if(wStationIndex == (ushort)-1)
        return;
    int length = 10;
    char* pSndData = new char[length];
    char* pSndStart = pSndData;
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(uchar*)pSndStart = 0x93;
    pSndStart += sizeof(uchar);
    *(ushort*)pSndStart = 0x03;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = stationIndex2Addr(wStationIndex);
    pSndStart += sizeof(ushort);
    *(uchar*)pSndStart = btLock;
    pSndStart += sizeof(uchar);


    handleSend(pSndData,length);
    if(pSndData)
    {
        delete pSndData;
        pSndData = NULL;
    }
}

void HProtocol::sendLockOne(ushort wStationIndex,ushort wYxIndex,uchar btLock)
{
    if(wStationIndex == (ushort)-1 || (wYxIndex) == (ushort)-1)
        return;
    int length = 12;
    char* pSndData = new char[length];
    char* pSndStart = pSndData;
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = 0xEB90;
    pSndStart += sizeof(ushort);
    *(uchar*)pSndStart = 0x90;
    pSndStart += sizeof(uchar);
    *(ushort*)pSndStart = 0x05;
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = stationIndex2Addr(wStationIndex);
    pSndStart += sizeof(ushort);
    *(ushort*)pSndStart = wYxIndex;
    pSndStart += sizeof(ushort);
    *(uchar*)pSndStart = btLock;
    pSndStart += sizeof(uchar);

    handleSend(pSndData,length);
    if(pSndData)
    {
        delete pSndData;
        pSndData = NULL;
    }
}

void HProtocol::sendMeasureYx()
{

}


void HProtocol::timerProcessor()//定时处理
{
    sendHeartBeat();
    if(--wSendVYXTimes == 0)
    {
        sendAllVYx();
        wSendVYXTimes = 30;
    }
}
