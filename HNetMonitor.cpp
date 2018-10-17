#if defined (_MSC_VER) && (_MSC_VER >=1600)
#pragma execution_character_set("utf-8")
#endif
#include "hnetmonitor.h"
#include "ui_netmonitor.h"
#include "hmyhighligher.h"
#include "hnetthread.h"
#include "hprotocol.h"
#include <QTimer>
extern ShowMsg* remove_msg_from_list();
extern void clear_msg_list();
/*********************************************************************************************************/
HNetMonitor::HNetMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::netMonitor)
{
    ui->setupUi(this);
    m_pNetThread = NULL;
    m_pProtocol = NULL;
    m_nShowMsgCount = 0;
    h = new HMyHighligher(ui->msgTextEdit->document());
    init();
    setWindowTitle("网络报文监视界面");
}

HNetMonitor::~HNetMonitor()
{
    delete ui;
    if(m_pNetThread)
        delete m_pNetThread;
    if(m_pProtocol)
        delete m_pProtocol;
    clear_msg_list();
}

void HNetMonitor::init()
{
    QTimer *refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(procShowMsgList()));
    refreshTimer->start(1000);
    m_pNetThread = new HNetThread;
    m_pNetThread->start();
    m_pProtocol = new HProtocol;
    m_pProtocol->start();

}

//定时处理函数 显示或者加入日志
void HNetMonitor::procShowMsgList()
{
    ShowMsg *msg = remove_msg_from_list();
    while(msg)
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
        strMsg += " \n";
        ui->linkTextEdit->insertPlainText(strMsg);
    }
    else if(MSG_INFORMATION == msg->type )
    {
        strType = QString("information: "); //主要是接收连接等信息提示
        QString strMsg = msg->info.c_str();
        strMsg += " \n";
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
            strType = QString(msg->info.c_str()) + QString(":<< "); //接收报文:
        }
        else if(MSG_LINK_SEND == msg->type)
        {
            whichTextEdit = 1;
            strType = QString(msg->info.c_str()) + QString(":>> "); //发送报文:
        }

        QString strShowMsg,strMsg;
        for(int i = 0; i < msg->len;i++)
        {
            QString strTemp = QString("%1 ").arg((uchar)msg->data[i],2,16,QLatin1Char('0'));
            strMsg += strTemp;
        }
        strMsg += "\n";
        if(0 == whichTextEdit)
        {
            strShowMsg = strType + strMsg;
            strShowMsg += " \n";
            ui->msgTextEdit->insertPlainText(strShowMsg);//接收报文(心跳):<< 85 00 00 00
        }
        else if(1 == whichTextEdit)//链路层的报文详细 但不设置颜色等信息
        {
            //2018-11-11 00:12:21 from:198.120.0.101 to linkA:<< 85 00 00 00
            //2018-11-11 00:12:22 from:linkA to 198.120.0.101:<< 85 00 00 00
            strShowMsg = strType + strMsg;
            strShowMsg += " \n";
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
