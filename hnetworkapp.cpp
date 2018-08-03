#include "hnetworkapp.h"
#include "htcpclient.h"
#include "hprotocol.h"
#include <QTimerEvent>

boost::mutex g_recv_mutex;
std::list<RecvData*> g_recv_list;
void add_data_to_recv_list(RecvData* recv_data);
RecvData* remove_data_from_recv_list();
void clear_recv_list();
extern QMutex g_msg_mutex;
extern void add_msg_for_show(unsigned short type, RecvData* data,std::string info);


HNetworkApp* HNetworkApp::pInstance = NULL;
HNetworkApp* HNetworkApp::instance()
{
    if(pInstance == NULL)
    {
        pInstance = new HNetworkApp;
    }

    return pInstance;
}

void HNetworkApp::exitinstance()
{
    if(pInstance)
    {
        delete pInstance;
        pInstance = NULL;
    }
}

HNetworkApp::HNetworkApp()
{
    m_send_mode = MODE_MASTER_RESERVER;
    work.reset(new io_service::work(ios));
}

HNetworkApp::~HNetworkApp()
{
    if(m_tcpServerA)
    {
        delete m_tcpServerA;
        m_tcpServerA = NULL;
    }

    if(m_tcpServerB)
    {
        delete m_tcpServerB;
        m_tcpServerB = NULL;
    }
    stop();
}

void HNetworkApp::readOption()
{

    //读取IP信息
}

void HNetworkApp::start()
{
    //ios.run必须要先执行才能完成后续一系列的动作
    readOption();
    bool b_availA,b_avaliB;
    b_availA = b_avaliB = false;
    if(b_availA || b_avaliB)
    {
        //startTimer(1000);
        m_p_timer = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(ios)) ;
        start_timer();
        run();
    }	
}

void HNetworkApp::run()
{
    ios.run();
}

void HNetworkApp::stop()
{
    ios.stop();
}

//谁发出 谁删除
void HNetworkApp::handle_send(RecvData *senddata)
{
    if(!senddata)
        return;
    //先修改发送时间
    std::string time = to_simple_string(second_clock::local_time());
    senddata->time = time;
    int ip = senddata->ip;
    //双主模式
    if(MODE_DOUBLE_MASTER == m_send_mode)
    {
        /*int ip = senddata->ip;
        HTcpClientPtr conn = conn_map[ip];
        if(conn)
        {
            conn->send_msg(senddata->data,senddata->len);
            std::string info = "linkA";
            if(conn->m_p_tcp_server == m_tcpServerB)
                info = "linkB";
            add_msg_for_show(MSG_LINK_SEND,senddata,info);
        }*/
    }
    else if(MODE_MASTER_RESERVER == m_send_mode) //主备模式
    {
        std::string info = "linkA";
<<<<<<< HEAD:hnetworkapp.cpp
        if(m_tcpServerA->find_tcp_client_ptr(ip)->status() == CONNECTED)
=======
        //要判断是否为空
        if(m_tcp_server_a->m_p_client_connect->status() == CONNECTED)
>>>>>>> df21d31b704e7de76870272105708fa357e85b11:HNetApp.cpp
        {
            m_tcpServerA->find_tcp_client_ptr(ip)->send_msg(senddata->data,senddata->len);
        }
        else if(m_tcpServerB->find_tcp_client_ptr(ip)->status() == CONNECTED)
        {
            info = "linkB";
            m_tcpServerB->find_tcp_client_ptr(ip)->send_msg(senddata->data,senddata->len);
        }
        add_msg_for_show(MSG_LINK_SEND,senddata,info);
    }
}

void HNetworkApp::handle_send(char* pData,int nLength)
{
    //如果A网通 A网发 否则B网通 B网发
    //主备模式发送
  /*  if(m_tcpServerA->m_p_client_connect->status() == CONNECTED)
    {
        m_tcpServerA->m_p_client_connect->send_msg(pData,nLength);
    }
    else if(m_tcpServerB->m_p_client_connect->status() == CONNECTED)
    {
        m_tcpServerB->m_p_client_connect->send_msg(pData,nLength);
    }*/
}

void HNetworkApp::handle_recv(char *pData, int nLength,int ip,std::string& time)
{
    if(!pData || 0 == nLength) return;
    RecvData* recv_data = new RecvData;
    if(!recv_data) return;
    recv_data->data = new char[nLength];
    memset(recv_data->data,0,sizeof(nLength));
    memcpy(recv_data->data,pData,nLength);
    recv_data->len = nLength;
    recv_data->ip = ip;
    recv_data->time = time;
    //假设存在两个连接同时发数据过来的情况
    add_data_to_recv_list(recv_data);
}

void HNetworkApp::proc_recv_data()
{
    RecvData* p_recv_data = remove_data_from_recv_list();
    while(p_recv_data)
    {
        if(p_recv_data->data && p_recv_data->len > 0)
        {
            //调用规约接口处理
            //HProtocol::m_pInstance->handleReceive(p_recv_data);
            //调用显示链路层接口处理
            HTcpClientPtr conn = conn_map[p_recv_data->ip];
            std::string info = "who Recv";
            if(conn)
            {
                /*if(conn->m_p_tcp_server == m_tcpServerA)
                    info = "linkA";
                else if(conn->m_p_tcp_server == m_tcpServerB)
                    info = "linkB";*/
            }
            add_msg_for_show(MSG_LINK_RECV,p_recv_data,info);
        }
        if(p_recv_data->data)
            delete p_recv_data->data;
        delete p_recv_data;
        p_recv_data = remove_data_from_recv_list();
    }
}

void HNetworkApp::timerEvent(QTimerEvent *event)
{
    proc_recv_data();
}

void HNetworkApp::start_timer()
{
    if(m_p_timer)
    {
        m_p_timer->expires_from_now(boost::posix_time::seconds(2));
        m_p_timer->async_wait(boost::bind(&HNetworkApp::handle_timer,this,asio::placeholders::error));
    }
}

void HNetworkApp::proc_timer()//处理定时函数
{
    //处理心跳报文时间 发送心跳报文
    //proc_recv_data();
    /*
     * 超时部分处理 热备用，就算备网不用也要有心跳。除此之外其他情况都可以不判。
    */
    /*
    if(0) //此处暂时不判
    {
        if(m_tcpServerA && m_tcpServerA->m_p_client_connect)
        {
            if(m_tcpServerA->m_p_client_connect->m_n_over_time++ > 10) //可以写成从配置里面读
            {
                //超时
                std::ostringstream os;
                os.str().reserve(128);
                os << "网络超时，关闭链接";
                m_tcpServerA->m_p_client_connect->stop();
            }
        }

        if(m_tcpServerB && m_tcpServerB->m_p_client_connect)
        {
            if(m_tcpServerB->m_p_client_connect->m_n_over_time++ > 10) //可以写成从配置里面读
            {
                //超时
                std::ostringstream os;
                os.str().reserve(128);
                os << "网络超时，关闭链接";
                m_tcpServerB->m_p_client_connect->stop();
            }
        }
    }*/
}
void HNetworkApp::handle_timer(const boost::system::error_code& err)//时间处理函数
{
    if(!err)
    {
        proc_timer();
        start_timer();
    }
}


/**************************************全局函数******************************************/
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
            delete pdata->data;
        delete pdata;
    }
    g_recv_list.clear();
    g_recv_mutex.unlock();
}


