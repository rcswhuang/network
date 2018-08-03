<<<<<<< HEAD
﻿#include "hnetmonitor.h"
#include "ui_netmonitor.h"
#include "hmyhighligher.h"
#include "hnetthread.h"
#include <QTimer>
extern QMutex g_msg_mutex;
extern std::list<ShowMsg*> g_msg_list;
extern ShowMsg* remove_msg_from_list();
/*********************************************************************************************************/
HNetMonitor::HNetMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::netMonitor)
{
    ui->setupUi(this);
    m_nShowMsgCount = 0;
    h = new HMyHighligher(ui->msgTextEdit->document());
}

HNetMonitor::~HNetMonitor()
{
    delete ui;
    while(!g_msg_list.empty())
    {
        //显示消息
        ShowMsg *msg = remove_msg_from_list();
        if(msg->data)
            delete msg->data;
        delete msg;
    }
}

void HNetMonitor::init()
{
    refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(procShowMsgList()));
    refreshTimer->start(1000);
    m_pNetThread = new HNetThread(this);
    m_pNetThread->start();
}

//定时处理函数 显示或者加入日志
void HNetMonitor::procShowMsgList()
{
    if(!g_msg_list.empty())
        return;

    ShowMsg *msg = remove_msg_from_list();
    while(!msg)
    {
        //显示消息
        showMsgToTextEdit(msg);
        if(msg->data)
            delete msg->data;
        delete msg;
        msg = remove_msg_from_list();
    }
}

void HNetMonitor::showMsgToTextEdit(ShowMsg *msg)
{
    if(!msg) return;
    QString strType = QString("未知");
    if(MSG_ERROR == msg->type)
    {
        strType = QString("error: "); //主要是链路关闭等异常情况
        QString strMsg = msg->info.c_str();
        strMsg += " \n\n";
        //发给链路层
        ui->linkTextEdit->insertPlainText(strMsg);
    }
    else
    {
        uchar whichTextEdit = 0;
        if(MSG_APP_RECV == msg->type) //规约层只显示
        {
            strType = QString("接收报文(%1):<< ").arg(getJKTypeByMsg(msg)); //接收报文(心跳):<< 85 00 00 00
        }
        else if(MSG_APP_SEND == msg->type)
        {
            strType = QString("发送报文(%1):>> ").arg(getWFTypeByMsg(msg)); //发送报文(心跳):>> 85 00 00 00
        }

        if(MSG_LINK_RECV == msg->type) //规约层只显示
        {
            whichTextEdit = 1;
            QString temp = QString(msg->time.c_str()) + " " + "from：" + QString(ip::address_v4(msg->ip).to_string().c_str());
            strType =temp  + "to " + QString(msg->info.c_str()) + QString(":<< "); //接收报文:
        }
        else if(MSG_LINK_SEND == msg->type)
        {
            whichTextEdit = 1;
            QString temp = QString(msg->time.c_str()) + " " + "from：" + QString(msg->info.c_str());
            strType =temp  + "to " + QString(ip::address_v4(msg->ip).to_string().c_str()) + QString(":>> "); //发送报文:
        }

        QString strShowMsg,strMsg;
        for(int i = 0; i < msg->len;i++)
        {
            QString strTemp = QString("%1 ").arg((int)msg->data[i],0,16);
            strMsg += strTemp;
        }
        strMsg += "\n\n";
        if(0 == whichTextEdit)
        {
            strShowMsg = strType + strMsg;
            strShowMsg += " \n\n";
            ui->msgTextEdit->insertPlainText(strShowMsg);//接收报文(心跳):<< 85 00 00 00
        }
        else if(1 == whichTextEdit)//链路层的报文详细 但不设置颜色等信息
        {
            //2018-11-11 00:12:21 from:198.120.0.101 to linkA:<< 85 00 00 00
            //2018-11-11 00:12:22 from:linkA to 198.120.0.101:<< 85 00 00 00
            strShowMsg = strType + strMsg;
            strShowMsg += " \n\n";
            ui->linkTextEdit->insertPlainText(strShowMsg);
        }
    }

    //后续还可以将报文信息存入日志文件
}

QString HNetMonitor::getJKTypeByMsg(ShowMsg* msg)
{
    QString strType = "未知";
    if(!msg) return strType;
    uchar code = *(uchar*)(&msg->data[0]);
    switch(code)
    {
    case 0x85: strType = "心跳";break;
    case 0x86: strType = "全遥信";break;
    case 0x87: strType = "变位遥信";break;
    case 0x88: strType = "遥控校验请求";break;
    case 0x91: strType = "全遥测";break;
    case 0x96: strType = "对时";break;
    }
    return strType;
}

QString HNetMonitor::getWFTypeByMsg(ShowMsg* msg)
{
    QString strType = "未知";
    if(!msg) return strType;
    uchar code = *(uchar*)(&msg->data[0]);
    switch(code)
    {
    case 0x85: strType = "心跳";break;
    case 0x86: strType = "虚遥信";break;
    case 0x88: strType = "遥控校验应答";break;
    case 0x89: strType = "遥控";break;
    case 0x90: strType = "请求遥控";break;
    case 0x95: strType = "请求全遥信";break;
    }
    return strType;
}
=======
﻿#include "hnetmonitor.h"
#include "ui_netmonitor.h"
#include "hmyhighligher.h"
#include "hnetthread.h"
#include <QTimer>
QMutex g_msg_mutex;
std::list<ShowMsg*> g_msg_list;
void add_msg_to_list(ShowMsg* showmsg);
ShowMsg* remove_msg_from_list();
void clear_msg_list();
void add_msg_for_show(unsigned short type, RecvData* data,std::string info);
void add_msg_for_show(unsigned short type,std::string msg);

/*********************************************************************************************************/
HNetMonitor::HNetMonitor(QWidget *parent) :QDialog(parent),ui(new Ui::netMonitor)
{
    ui->setupUi(this);
    h = new HMyHighligher(ui->msgTextEdit->document());
}

HNetMonitor::~HNetMonitor()
{
    delete ui;
    while(!g_msg_list.empty())
    {
        //显示消息
        ShowMsg *msg = remove_msg_from_list();
        if(msg->data)
            delete msg->data;
        delete msg;
    }
}

void HNetMonitor::start()
{
    refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(procShowMsgList()));
    refreshTimer->start(1000);
    m_pNetThread = new HNetThread(this);
    m_pNetThread->start();
}

//定时处理函数 显示或者加入日志
void HNetMonitor::procShowMsgList()
{
    if(!g_msg_list.empty())
        return;

    ShowMsg *msg = remove_msg_from_list();
    while(!msg)
    {
        //显示消息
        showMsgToTextEdit(msg);
        if(msg->data)
            delete msg->data;
        delete msg;
        msg = remove_msg_from_list();
    }
}

void HNetMonitor::showMsgToTextEdit(ShowMsg *msg)
{
    if(!msg) return;
    QString strType = QString("未知");
    if(MSG_ERROR == msg->type)
    {
        strType = QString("error: "); //主要是链路关闭等异常情况
        QString strMsg = msg->info.c_str();
        strMsg += " \n\n";
        //发给链路层
        ui->linkTextEdit->insertPlainText(strMsg);
    }
    else
    {
        uchar whichTextEdit = 0;
        if(MSG_APP_RECV == msg->type) //规约层只显示
        {
            strType = QString("接收报文(%1):<< ").arg(getJKTypeByMsg(msg)); //接收报文(心跳):<< 85 00 00 00
        }
        else if(MSG_APP_SEND == msg->type)
        {
            strType = QString("发送报文(%1):>> ").arg(getWFTypeByMsg(msg)); //发送报文(心跳):>> 85 00 00 00
        }

        if(MSG_LINK_RECV == msg->type) //规约层只显示
        {
            whichTextEdit = 1;
            QString temp = QString(msg->time.c_str()) + " " + "from：" + QString(ip::address_v4(msg->ip).to_string().c_str());
            strType =temp  + "to " + QString(msg->info.c_str()) + QString(":<< "); //接收报文:
        }
        else if(MSG_LINK_SEND == msg->type)
        {
            whichTextEdit = 1;
            QString temp = QString(msg->time.c_str()) + " " + "from：" + QString(msg->info.c_str());
            strType =temp  + "to " + QString(ip::address_v4(msg->ip).to_string().c_str()) + QString(":>> "); //发送报文:
        }

        QString strShowMsg,strMsg;
        for(int i = 0; i < msg->len;i++)
        {
            QString strTemp = QString("%1 ").arg((int)msg->data[i],0,16);
            strMsg += strTemp;
        }
        strMsg += "\n\n";
        if(0 == whichTextEdit)
        {
            strShowMsg = strType + strMsg;
            strShowMsg += " \n\n";
            ui->msgTextEdit->insertPlainText(strShowMsg);//接收报文(心跳):<< 85 00 00 00
        }
        else if(1 == whichTextEdit)//链路层的报文详细 但不设置颜色等信息
        {
            //2018-11-11 00:12:21 from:198.120.0.101 to linkA:<< 85 00 00 00
            //2018-11-11 00:12:22 from:linkA to 198.120.0.101:<< 85 00 00 00
            strShowMsg = strType + strMsg;
            strShowMsg += " \n\n";
            ui->linkTextEdit->insertPlainText(strShowMsg);
        }
    }

    //后续还可以将报文信息存入日志文件
}

QString HNetMonitor::getJKTypeByMsg(ShowMsg* msg)
{
    QString strType = "未知";
    if(!msg) return strType;
    uchar code = *(uchar*)(&msg->data[0]);
    switch(code)
    {
    case 0x85: strType = "心跳";break;
    case 0x86: strType = "全遥信";break;
    case 0x87: strType = "变位遥信";break;
    case 0x88: strType = "遥控校验请求";break;
    case 0x91: strType = "全遥测";break;
    case 0x96: strType = "对时";break;
    }
    return strType;
}

QString HNetMonitor::getWFTypeByMsg(ShowMsg* msg)
{
    QString strType = "未知";
    if(!msg) return strType;
    uchar code = *(uchar*)(&msg->data[0]);
    switch(code)
    {
    case 0x85: strType = "心跳";break;
    case 0x86: strType = "虚遥信";break;
    case 0x88: strType = "遥控校验应答";break;
    case 0x89: strType = "遥控";break;
    case 0x90: strType = "请求遥控";break;
    case 0x95: strType = "请求全遥信";break;
    }
    return strType;
}


/**************************************************************************************************************************************/
//直接发送显示消息(不含报文)
void add_msg_for_show(unsigned short type,std::string show)
{
    int len = show.length();
    if(len <= 0) return;
    ShowMsg* msg = new ShowMsg;
    msg->type = type;
    msg->info = show;
    add_msg_to_list(msg);

}

//发送显示消息(含报文)
void add_msg_for_show(unsigned short type, RecvData* rdata,std::string info)
{
    if(!rdata) return;
    ShowMsg* msg = new ShowMsg;
    msg->type = type;
    msg->len = rdata->len;
    memcpy(msg->data,rdata->data,rdata->len);
    msg->ip = rdata->ip;
    msg->time = rdata->time;
    msg->info = info;
    add_msg_to_list(msg);
}

void add_msg_to_list(ShowMsg* showmsg)
{
    g_msg_mutex.lock();
    g_msg_list.push_back(showmsg);
    g_msg_mutex.unlock();
}

ShowMsg* remove_msg_from_list()
{
    ShowMsg* p_msg = NULL;
    g_msg_mutex.lock();
    if(g_msg_list.size() > 0)
    {
        p_msg = (ShowMsg*)g_msg_list.front();
        g_msg_list.pop_front();
    }
    g_msg_mutex.unlock();
    return p_msg;
}

void clear_msg_list()
{
    g_msg_mutex.lock();
    std::list<ShowMsg*>::iterator it;
    for(it = g_msg_list.begin(); it != g_msg_list.end(); ++it)
    {
        ShowMsg* pmsg = *it;
        if(pmsg->data)
            delete pmsg->data;
        delete pmsg;
    }
    g_msg_list.clear();
    g_msg_mutex.unlock();
}
>>>>>>> df21d31b704e7de76870272105708fa357e85b11