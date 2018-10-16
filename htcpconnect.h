#pragma once
#include "hnetworkdef.h"
#include "hmsg.h"
class HNetManager;
class HTcpConnect :public boost::enable_shared_from_this<HTcpConnect>, boost::noncopyable
{
public:
	typedef std::vector<char> buffer_char;
public:
    HTcpConnect(io_service& io_service);
    ~HTcpConnect();
public:
    tcp::socket& sokcet();
    void start(HNetManager* manager);
	void stop();
    int getip();//返回IP
    int status(){return status_;}

public:
    void send_msg(char* p,int len);
    //void  push_msg_to_list(HMsg* msg);
    //HMsg* pop_msg_from_list();
    //HMsg* take_msg_from_list();

private:
    void handle_read_data(const err_code & err);
    void handle_read_header_data(const err_code & err);
    void handle_write(const err_code &err);
	void do_read_header();
	void do_read();

public:
    HNetManager* m_pNetManager;
    int m_n_over_time;//超时

private:
    //std::list<HMsg*> m_lp_send_list;
    boost::mutex m_send_mutex;
    HMsg m_msg;
	int status_;
	ip::tcp::socket socket_;
};

typedef boost::shared_ptr<HTcpConnect>  HTcpConnectPtr;
