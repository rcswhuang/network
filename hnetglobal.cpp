#include "hnetglobal.h"

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
