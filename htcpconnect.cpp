#include "htcpconnect.h"
#include "htcpserver.h"
#include "hnetmanager.h"
extern void add_msg_for_show(unsigned short type,std::string msg);
extern void add_data_for_show(unsigned short type, char*data,int len,std::string info);
HTcpConnect::HTcpConnect(io_service& ios):
socket_(ios)
{
    status_ = INITCONNECT;
    m_n_over_time = 0;
}

HTcpConnect::~HTcpConnect()
{
    //stop();
    /*
    while(!m_lp_send_list.empty())
    {
        HMsg* front = take_msg_from_list();
        if(!front)
            delete front;
    }*/
}

tcp::socket& HTcpConnect::sokcet()
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
    status_ = CLOSED;
    try
    {
        socket_.shutdown(tcp::socket::shutdown_both);//必须要先关闭所有socket_
        socket_.close();

        uint32_t ip = socket_.remote_endpoint().address().to_v4().to_ulong();
        uint16_t port = (uint16_t)socket_.remote_endpoint().port();
        uint64_t ipport = ((uint64_t)ip << 16) | (uint64_t)(port);

        m_pTcpServer->removeConnect(ipport);

    }catch (std::exception& e)
    {
    }
}

int HTcpConnect::getip()
{
    return socket_.remote_endpoint().address().to_v4().to_ulong();
}

void HTcpConnect::do_read_header()
{
    m_msg.clear();
    async_read(socket_, asio::buffer(m_msg.msg(),4), boost::bind(&HTcpConnect::handle_read_header_data,this,asio::placeholders::error));
}

void HTcpConnect::handle_read_header_data(const err_code & err)
{
    if (!err && m_msg.analy_header())
    {
        do_read();
    }
    else
    {
        //stop();//表示链接中断等异常情况
        //std::ostringstream os;
        //os.str().reserve(128);
        //os << socket_.remote_endpoint().address().to_v4().to_string() << ":Connection is interrupted!";
        //add_msg_for_show(MSG_ERROR,os.str());
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
        add_data_for_show(MSG_LINK_RECV, m_msg.msg(),(int)m_msg.msg_length(),"link");
        //m_pNetManager->handle_recv(m_msg.msg(),(int)m_msg.msg_length());//链路层报文:需要互斥定时来进行处理
        uint32_t ip = socket_.remote_endpoint().address().to_v4().to_ulong();
        uint16_t port = (uint16_t)socket_.remote_endpoint().port();
        uint64_t ipport = ((uint64_t)ip << 16) | (uint64_t)(port);

        handle_recv(m_msg.msg(),(int)m_msg.msg_length(),ipport);//链路层报文:需要互斥定时来进行处理
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

void HTcpConnect::handle_recv(char* pData, int nLength, uint64_t ipport)
{
    if (!pData || 0 == nLength) return;
    RecvData* recv_data = new RecvData;
    if (!recv_data) return;
    recv_data->data = new char[nLength];
    memset(recv_data->data, 0, sizeof(nLength));
    memcpy(recv_data->data, pData, nLength);
    recv_data->len = nLength;
    recv_data->ipport = ipport;
    m_pNetManager->handle_recv(recv_data);
}

void HTcpConnect::send_msg(char* p,int len)
{
    if(!p) return;
    HMsg *msg = new HMsg;
    memcpy(msg->data(),p,len);
    msg->set_data_length(len-HEAD_SIZE);
    add_data_for_show(MSG_LINK_SEND,p,len,"link");
    boost::asio::async_write(socket_,boost::asio::buffer(msg->msg(),msg->msg_length()),boost::bind(&HTcpConnect::handle_write,this,asio::placeholders::error));
}

void HTcpConnect::handle_write(const err_code &err)
{
    if(!err)
    {
        return;
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

//当多个消息发送的时候需要同步，本应用不需要
/*
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
}*/



