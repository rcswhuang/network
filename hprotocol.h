#ifndef HPROTOCOL_H
#define HPROTOCOL_H

//规约解析部分
#include <QThread>
#include <QTimer>
#include <QVector>
#include "hnetworkdef.h"
class HNetManager;
class HProtocol : public QThread
{
    Q_OBJECT
public:
    typedef QVector<ushort> QUshortArray;
public:
    HProtocol();
    ~HProtocol();
public:
    virtual void run();


public:
    //void start();
    void loadVirtualYx();//加载虚遥信
    void handleReceive(RecvData* recvData);
    void handleSend(char* pData,int length);

    //proc
    void proc_recv_data();
    void processHand(char* pData,int length);
    void processAllYx(char* pData,int length);
    void processChangeYx(char* pData,int length);
    void processAllYc(char* pData,int length);
    void processYKCheck(char* pData,int length);
    void processTimer(char* pData,int length);

    //send
    void sendHeartBeat();
    void sendAllVYx();
    void sendReqAllYx(ushort wStationIndex,ushort wYxIndex);
    void sendYKCheckBack(ushort wStationIndex,ushort wYxIndex,uchar btValue,bool bCheck);
    void sendWFYK(ushort wStationIndex,ushort wYxIndex,uchar btValue);
    void sendReqYK(ushort wStationIndex,ushort wYxIndex,uchar btValue);
    void sendLockAll(ushort wStationIndex,uchar btLock);
    void sendLockOne(ushort wStationIndex,ushort wYxIndex,uchar btLock);
    void sendMeasureYx();

public slots:
    void timerProcessor();//定时处理

protected:
    HNetManager* m_pNetManager;
    QTimer* timer;//定时器
    ushort wSendVYXTimes;
    ushort p_sendToScadaYXStationCounts;
    ushort* p_sendToScadaStationList;
    QUshortArray *p_sendToScadaYXList;
};

#endif // HPROTOCOL_H
