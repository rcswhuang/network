#include "htcpconnect.h"
#include "htcpserver.h"
#include "hnetmanager.h"
extern void add_msg_for_show(unsigned short type,std::string msg);

HTcpConnect::HTcpConnect(io_service& ios):
socket_(ios)
{
    status_ = INITCONNECT;
    m_n_over_time = 0;
}

HTcpConnect::~HTcpConnect()
{
    stop();
    while(!m_lp_send_list.empty())
    {
        HMsg* front = take_msg_from_list();
        if(!front)
            delete front;
    }
}

ip::tcp::socket& HTcpConnect::sokcet()
{
    return socket_;
}

void HTcpConnect::start(HNetManager* manager)
{
    m_pNetManager = manager;
    status_ = CONNECTED;
    socket_.set_option(boost::asio::ip::tcp::acceptor::linger(true, 0));
    socket_.set_option(boost::asio::socket_base::keep_alive(true));
    do_read_header();
}

void HTcpConnect::stop()
{
    socket_.close();
    status_ = CLOSED;
    std::ostringstream os;
    os.str().reserve(128);
    os << "TcpClinet: " << socket_.remote_endpoint()<<"is stop!";
    add_msg_for_show(MSG_ERROR,os.str());
}

int HTcpConnect::getip()
{
    return socket_.remote_endpoint().address().to_v4().to_ulong();
}

void HTcpConnect::do_read_header()
{
    m_msg.clear();
    async_read(socket_, asio::buffer(m_msg.data(),m_msg.header_length), boost::bind(&HTcpConnect::handle_read_header_data,this,asio::placeholders::error));
}

void HTcpConnect::handle_read_header_data(const err_code & err)
{
    if (!err && m_msg.analy_header())
    {
        do_read();
    }
    else
    {
        stop();//表示链接中断等异常情况
        std::ostringstream os;
        os.str().reserve(128);
        os << socket_.remote_endpoint().address().to_v4().to_string() << ":Connection is interrupted!";
        add_msg_for_show(MSG_ERROR,os.str());
    }
}

void HTcpConnect::do_read()
{
    async_read(socket_,asio::buffer(m_msg.data(),m_msg.data_length()),boost::bind(&HTcpConnect::handle_read_data,this,asio::placeholders::error));
}

void HTcpConnect::handle_read_data(const err_code & err)
{
    if (!err)
    {
        m_pNetManager->handle_recv(m_msg.msg(),(int)m_msg.msg_length());//链路层报文:需要互斥定时来进行处理
        m_n_over_time = 0;
        do_read_header();
    }
    else
    {
       stop();
       std::ostringstream os;
       os.str().reserve(128);
       os << socket_.remote_endpoint().address().to_v4().to_string() << ":Connection is interrupted!";
       add_msg_for_show(MSG_ERROR,os.str());
    }
}

void HTcpConnect::send_msg(char* p,int len)
{
    if(!p) return;
    HMsg *msg = new HMsg;
    memcpy(msg->data(),p,len);
    msg->set_data_length(len-HEAD_SIZE);
    boost::asio::async_write(socket_,boost::asio::buffer(p_msg->msg(),p_msg->msg_length()),boost::bind(&HTcpConnect::handle_write,this,asio::placeholders::error));
}

void HTcpConnect::push_msg_to_list(HMsg* msg)
{
    m_send_mutex.lock();
    m_lp_send_list.push_back(msg);
    m_send_mutex.unlock();
}

HMsg* HTcpConnect::pop_msg_from_list()
{
    HMsg* msg = NULL;
    m_send_mutex.lock();
    if(!m_lp_send_list.empty())
        msg = m_lp_send_list.front();
    m_send_mutex.unlock();
    return msg;
}

HMsg* HTcpConnect::take_msg_from_list()
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

void HTcpConnect::handle_write(const err_code &err)
{
    if(!err)
    {

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

