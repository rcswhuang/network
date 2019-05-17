#include "hnetworkdef.h"
#include <QtGlobal>
#include <QMutex>
/////////////////////////////////////////////接受共享队列///////////////////////////////////////
boost::mutex g_recv_mutex;
std::list<RecvData*> g_recv_list;
void add_data_to_recv_list(RecvData* recv_data);
RecvData* remove_data_from_recv_list();
void clear_recv_list();

/////////////////////////////////////////////消息共享队列//////////////////////////////////////
boost::mutex g_msg_mutex;
std::list<ShowMsg*> g_msg_list;
void add_msg_to_list(ShowMsg* showmsg);
ShowMsg* remove_msg_from_list();
void clear_msg_list();

void add_data_for_show(unsigned short type, char*data,int len,std::string info);
void add_msg_for_show(unsigned short type,std::string msg);
///////////////////////////////////////////////发送共享队列////////////////////////////////////////////////
boost::mutex g_snd_mutex;
std::list<SndData*> g_send_list;
void add_data_to_send_list(SndData* sndData);
SndData* remove_data_from_send_list();
void clear_send_list();

//////////////////////////////////////显示消息报文队列 (链路、规约 <-> 主界面)//////////////////////////////////////////////////////////
void add_data_for_show(unsigned short type, char*sdata,int len,std::string info)
{
    if(!sdata) return;
    ShowMsg* msg = new ShowMsg;
    if(NULL == msg) return;
    msg->data = new char[len];
    if(NULL == msg->data)
    {
        delete msg;
        return;
    }
    msg->type = type;
    msg->len = len;
    memcpy(msg->data,sdata,len);
    //msg->ip = rdata->ip;
    //msg->time = rdata->time;
    msg->info = info;
    add_msg_to_list(msg);
}

//直接发送显示消息(不含报文)
void add_msg_for_show(unsigned short type,std::string show)
{
    int len = show.length();
    if(len <= 0) return;
    ShowMsg* msg = new ShowMsg;
    msg->data = 0;
    msg->len = 0;
    msg->type = type;
    msg->info = show;
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
            delete[] pmsg->data;
        delete pmsg;
    }
    g_msg_list.clear();
    g_msg_mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************接受报文共享队列 (链路 <-> 规约)******************************************/
void add_data_to_recv_list(RecvData* recv_data)
{
    g_recv_mutex.lock();
    g_recv_list.push_back(recv_data);
    g_recv_mutex.unlock();
}

RecvData* remove_data_from_recv_list()
{
    RecvData* p_recv_data = NULL;
    g_recv_mutex.lock();
    if(g_recv_list.size() > 0)
    {
        p_recv_data = (RecvData*)g_recv_list.front();
        g_recv_list.pop_front();
    }
    g_recv_mutex.unlock();
    return p_recv_data;
}

void clear_recv_list()
{
    g_recv_mutex.lock();
    std::list<RecvData*>::iterator it;
    for(it = g_recv_list.begin(); it != g_recv_list.end(); ++it)
    {
        RecvData* pdata = *it;
        if(pdata->data)
            delete[] pdata->data;
        delete pdata;
    }
    g_recv_list.clear();
    g_recv_mutex.unlock();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void add_data_to_send_list(SndData* sndData)
{
    g_snd_mutex.lock();
    g_send_list.push_back(sndData);
    g_snd_mutex.unlock();
}

SndData* remove_data_from_send_list()
{
    SndData* p_send_data = NULL;
    g_snd_mutex.lock();
    if(g_send_list.size() > 0)
    {
        p_send_data = (SndData*)g_send_list.front();
        g_send_list.pop_front();
    }
    g_snd_mutex.unlock();
    return p_send_data;
}

void clear_send_list()
{
    g_snd_mutex.lock();
    std::list<SndData*>::iterator it;
    for(it = g_send_list.begin(); it != g_send_list.end(); ++it)
    {
        SndData* pdata = *it;
        if(pdata->data)
            delete[] pdata->data;
        delete pdata;
    }
    g_send_list.clear();
    g_snd_mutex.unlock();
}
