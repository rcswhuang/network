#include "hnetmanager.h"
#include "htcpconnect.h"
#include "hprotocol.h"
#include "hnetthread.h"

//extern void add_data_to_recv_list(RecvData* recv_data);
extern void add_data_for_show(unsigned short type, char*data,int len,std::string info);
extern SndData* remove_data_from_send_list();
extern void clear_send_list();

bool is_valid_ip(const char *ip)
{
    /* 定义正则表达式 */
    cregex reg_ip = cregex::compile("(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])");
    return 	regex_match(ip, reg_ip);
}

HNetManager::HNetManager(HNetThread* thread):m_pNetThread(thread)
{
    m_send_mode = MODE_MASTER_RESERVER;
    m_tcpServerA = NULL;
    m_tcpServerB = NULL;
    work.reset(new io_service::work(ios));
    m_nThreads = 3;
}

HNetManager::~HNetManager()
{
    ios.stop();
    work.reset();
    clear_send_list();

    if (m_p_timer)
    {
        m_p_timer->cancel();
    }

    //释放线程池
    std::vector<boost::shared_ptr<boost::thread>>::const_iterator it;
    for ( it = m_listThread.cbegin(); it != m_listThread.cend(); ++ it)
    {
        (*it)->join();
    }

    //释放单线程
//	if (m_pNetManagerThread)
//	{
//		m_pNetManagerThread->interrupt();
//	}

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

}

void HNetManager::config()
{
    m_NetConfig.str_ip_master = "198.120.0.110";
    m_NetConfig.str_ip_reserver = "";
    m_NetConfig.port = 9092;
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
        m_tcpServerA = new HTcpServer(ios,m_NetConfig.str_ip_master,m_NetConfig.port);
        m_tcpServerA->start(this);
        b_availA = true;
    }

    if(is_valid_ip(m_NetConfig.str_ip_reserver.data()))
    {
        m_tcpServerB = new HTcpServer(ios,m_NetConfig.str_ip_reserver,m_NetConfig.port);
        m_tcpServerB->start(this);
        b_avaliB = true;
    }

    if(b_availA || b_avaliB)
    {
        m_p_timer = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(ios)) ;
       start_timer();

       //启动线程池
       for (int i = 0; i != m_nThreads; ++i)
       {
           err_code err;
           boost::shared_ptr<boost::thread> pTh(new boost::thread(boost::bind(&boost::asio::io_service::run, boost::ref(ios), err)));
           m_listThread.push_back(pTh);
       }

       //单线程启动
//      m_pNetManagerThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, boost::ref(ios), err)));
        return true;
    }
    return false;
}


void HNetManager::handle_send(char* pData,int nLength)
{
    //双主模式
    if(MODE_DOUBLE_MASTER == m_send_mode)
    {

    }
    else if(MODE_MASTER_RESERVER == m_send_mode) //主备模式
    {
        /*
        std::string info = "linkA";
        if(m_tcpServerA->m_connects->status() == CONNECTED)
        {
            m_tcpServerA->m_connects->send_msg(pData,nLength);
            //add msg
            //process msg
        }
        else if(m_tcpServerB->m_connects->status() == CONNECTED)
        {
            info = "linkB";
            m_tcpServerB->m_connects->send_msg(pData,nLength);
        }*/
    }
}

void HNetManager::handle_send(SndData*& sendData)
{
    if (m_tcpServerA)
    {
        HTcpConnectPtr m_connect = m_tcpServerA->findConnect(sendData->ipport);
        if (m_connect && m_connect->status() == CONNECTED)
        {
            m_connect->send_msg(sendData->data, sendData->len);
        }
    }
    else if (m_tcpServerB)
    {
        HTcpConnectPtr m_connect = m_tcpServerB->findConnect(sendData->ipport);
        if (m_connect && m_connect->status() == CONNECTED)
        {
            m_connect->send_msg(sendData->data, sendData->len);
        }
    }
}

void HNetManager::handle_recv(RecvData*& recvData)
{
    if(NULL == recvData) return;
    //添加到列表
    //add_data_to_recv_list(recvData);

    //可以直接发送给协议层处理
    m_pNetThread->m_pProtocol->handleReceive(recvData);
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

    SndData* p_send_data =  remove_data_from_send_list();
    while(p_send_data)
    {
        if(p_send_data->data && p_send_data->len > 0)
        {
            handle_send(p_send_data->data,p_send_data->len);
        }
        if(p_send_data->data)
            delete[] p_send_data->data;
        delete p_send_data;
        p_send_data = remove_data_from_send_list();
    }
}
void HNetManager::handle_timer(const boost::system::error_code& err)//时间处理函数
{
    if(!err)
    {
        proc_timer();
        start_timer();
    }
}


