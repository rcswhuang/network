#include "hconnect.h"
#include "htcpserver.h"
#include "hnetworkapp.h"
extern QMutex g_msg_mutex;
extern std::list<ShowMsg*> g_msg_list;
extern void add_msg_for_show(unsigned short type, RecvData* data,std::string info);
extern void add_msg_for_show(unsigned short type,std::string msg);

HConnect::HConnect(io_service& ios,HNetworkApp* app):
socket_(ios),m_pNetWorkApp(app)
{
    status_ = INITCONNECT;
    m_n_over_time = 0;
}

HConnect::~HConnect()
{
    stop();
    while(!m_lp_send_list.empty())
    {
        HMsg* front = take_msg_from_list();
        if(!front)
            delete front;
    }
}

ip::tcp::socket& HConnect::sokcet()
{
    return socket_;
}

void HConnect::start()
{
    status_ = CONNECTED;
    socket_.set_option(boost::asio::ip::tcp::acceptor::linger(true, 0));
    socket_.set_option(boost::asio::socket_base::keep_alive(true));
    do_read_header();
}

void HConnect::stop()
{
    std::ostringstream os;
    os.str().reserve(128);
    os << "Connection stop	" << socket_.remote_endpoint()
            << "	=\\=>	" << socket_.local_endpoint();
    //LOG(TRACE) << "--HConnect-- " << os.str() << "\n";

    socket_.close();
    status_ = CLOSED;
}

int HConnect::getip()
{
    return socket_.remote_endpoint().address().to_v4().to_ulong();
}


void HConnect::do_read_header()
{
    m_msg.clear();
    async_read(socket_, asio::buffer(m_msg.data(),m_msg.header_length), boost::bind(&HConnect::handle_read_header_data,this,asio::placeholders::error));
}

void HConnect::handle_read_header_data(const err_code & err)
{
    if (!err && m_msg.analy_header())
    {
        do_read();
    }
    else
    {
        stop();//表示链接中断等异常情况
        std::ostringstream os; //信息可以加入时间
        os.str().reserve(128);
        os << socket_.remote_endpoint().address().to_v4().to_string() << "Connection is interrupted!";
        add_msg_for_show(MSG_ERROR,os.str());
    }
}

void HConnect::do_read()
{
    async_read(socket_,asio::buffer(m_msg.data(),m_msg.data_length()),boost::bind(&HConnect::handle_read_data,this,asio::placeholders::error));
}

void HConnect::handle_read_data(const err_code & err)
{
    if (!err)
    {
        std::string time = to_simple_string(second_clock::local_time());
        m_pNetWorkApp->handle_recv(m_msg.msg(),(int)m_msg.msg_length(),getip(),time);//链路层报文:需要互斥定时来进行处理
        m_n_over_time = 0;
        do_read_header();
    }
    else
    {
       stop();
       std::ostringstream os;
       os.str().reserve(128);
       os << socket_.remote_endpoint().address().to_v4().to_string() << "Connection is interrupted!";
       add_msg_for_show(MSG_ERROR,os.str());
    }
}

void HConnect::send_msg(char* p,int len)
{
    if(!p) return;
    HMsg *msg = new HMsg;
    memcpy(msg->data(),p,len);
    msg->set_data_length(len-HEAD_SIZE);

    push_msg_to_list(msg);

    if(!m_lp_send_list.empty())
    {
        HMsg* p_msg = pop_msg_from_list();
        boost::asio::async_write(socket_,boost::asio::buffer(p_msg->msg(),p_msg->msg_length()),boost::bind(&HConnect::handle_write,this,asio::placeholders::error));
    }
}

void  HConnect::push_msg_to_list(HMsg* msg)
{
    m_send_mutex.lock();
    m_lp_send_list.push_back(msg);
    m_send_mutex.unlock();
}

HMsg* HConnect::pop_msg_from_list()
{
    HMsg* msg = NULL;
    m_send_mutex.lock();
    if(!m_lp_send_list.empty())
        msg = m_lp_send_list.front();
    m_send_mutex.unlock();
    return msg;
}

HMsg* HConnect::take_msg_from_list()
{
    HMsg* msg = NULL;
    m_send_mutex.lock();
    if(!m_lp_send_list.empty())
    {
        msg = m_lp_send_list.front();
        m_lp_send_list.pop_front();
    }
    m_send_mutex.unlock();
    return msg;
}

void HConnect::handle_write(const err_code &err)
{
    if(!err)
    {
        if(!m_lp_send_list.empty())
        {
            HMsg* front = take_msg_from_list();
            if(!front)
                delete front;

            HMsg* p_msg = pop_msg_from_list();
            boost::asio::async_write(socket_,boost::asio::buffer(p_msg->msg(),p_msg->msg_length()),boost::bind(&HConnect::handle_write,this,asio::placeholders::error));
        }
    }
    else
    {
        stop();
        std::ostringstream os;
        os.str().reserve(128);
        os << socket_.remote_endpoint().address().to_v4().to_string() << "Connection is interrupted!";
        add_msg_for_show(MSG_ERROR,os.str());
    }
}

