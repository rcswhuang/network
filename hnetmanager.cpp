#include "hnetmanager.h"
#include "htcpconnect.h"
#include "hprotocol.h"
#include <QTimerEvent>

extern void add_data_to_recv_list(RecvData* recv_data);
extern RecvData* remove_data_from_recv_list();
extern void clear_recv_list();
extern void add_msg_for_show(unsigned short type, RecvData* data,std::string info);

bool is_valid_ip(const char *ip)
{
    /* 定义正则表达式 */
    cregex reg_ip = cregex::compile("(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])");
    return 	regex_match(ip, reg_ip);
}

HNetManager::HNetManager()
{
    m_send_mode = MODE_MASTER_RESERVER;
    work.reset(new io_service::work(ios));
}

HNetManager::~HNetManager()
{
    stop();
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
    clear_recv_list();
}

void HNetManager::config()
{
    m_NetConfig.str_ip_master = "";
    m_NetConfig.str_ip_reserver = "";
    m_NetConfig.port = 1234;
    //读取IP信息
}

bool HNetManager::start()
{
    //ios.run必须要先执行才能完成后续一系列的动作
    config();
    bool b_availA,b_avaliB;
    b_availA = b_avaliB = false;
    if(is_valid_ip(m_NetConfig.str_ip_master.data()))
    {
        m_tcpServerA = new HTcpServer(ios,"",1234);
        m_tcpServerA->start(this);
        b_availA = true;
    }

    if(is_valid_ip(m_NetConfig.str_ip_reserver.data()))
    {
        m_tcpServerB = new HTcpServer(ios,"",1234);
        m_tcpServerB->start(this);
        b_avaliB = true;
    }

    if(b_availA || b_avaliB)
    {
        m_p_timer = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(ios)) ;
        start_timer();
        return true;
    }
    return false;
}

void HNetManager::run()
{
    ios.run();
}

void HNetManager::stop()
{
    ios.stop();
}

//谁发出 谁删除
void HNetManager::handle_send(RecvData *senddata)
{
    if(!senddata)
        return;
    //先修改发送时间
/*    std::string time = to_simple_string(second_clock::local_time());
    senddata->time = time;
    //双主模式
    if(MODE_DOUBLE_MASTER == m_send_mode)
    {
        int ip = senddata->ip;
        HConnectPtr conn = conn_map[ip];
        if(conn)
        {
            conn->send_msg(senddata->data,senddata->len);
            std::string info = "linkA";
            if(conn->m_p_tcp_server == m_tcpServerB)
                info = "linkB";
            add_msg_for_show(MSG_LINK_SEND,senddata,info);
        }
    }
    else if(MODE_MASTER_RESERVER == m_send_mode) //主备模式
    {
        std::string info = "linkA";
        if(m_tcpServerA->m_p_client_connect->status() == CONNECTED)
        {
            m_tcpServerA->m_p_client_connect->send_msg(senddata->data,senddata->len);
        }
        else if(m_tcpServerB->m_p_client_connect->status() == CONNECTED)
        {
            info = "linkB";
            m_tcpServerB->m_p_client_connect->send_msg(senddata->data,senddata->len);
        }
        add_msg_for_show(MSG_LINK_SEND,senddata,info);
    }*/
}

void HNetManager::handle_send(char* pData,int nLength)
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

void HNetManager::handle_recv(char *pData, int nLength,int ip,std::string& time)
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

void HNetManager::proc_recv_data()
{
    RecvData* p_recv_data = remove_data_from_recv_list();
    while(p_recv_data)
    {
        if(p_recv_data->data && p_recv_data->len > 0)
        {
            //调用规约接口处理
            //HProtocol::m_pInstance->handleReceive(p_recv_data);
            //调用显示链路层接口处理
            HTcpConnectPtr conn = conn_map[p_recv_data->ip];
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

void HNetManager::timerEvent(QTimerEvent *event)
{
    proc_recv_data();
}

void HNetManager::start_timer()
{
    if(m_p_timer)
    {
        m_p_timer->expires_from_now(boost::posix_time::seconds(2));
        m_p_timer->async_wait(boost::bind(&HNetManager::handle_timer,this,asio::placeholders::error));
    }
}

void HNetManager::proc_timer()//处理定时函数
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
void HNetManager::handle_timer(const boost::system::error_code& err)//时间处理函数
{
    if(!err)
    {
        proc_timer();
        start_timer();
    }
}


