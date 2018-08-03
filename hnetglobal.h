#include "networkapi.h"
#include <QMutex>

QMutex g_msg_mutex;
std::list<ShowMsg*> g_msg_list;
void add_msg_to_list(ShowMsg* showmsg);
ShowMsg* remove_msg_from_list();
void clear_msg_list();

void add_msg_for_show(unsigned short type, RecvData* data,std::string info);
void add_msg_for_show(unsigned short type,std::string msg);

