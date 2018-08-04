#ifndef HCLIENT_H
#define HCLIENT_H
#include "networkapi.h"
#include "hnetworkapp.h"
#include "hmsg.h"
using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::posix_time;
class HTcpClient;
typedef boost::shared_ptr<HTcpClient> HTcpClientPtr;
class HTcpClient :public boost::enable_shared_from_this<HTcpClient>, boost::noncopyable
{
    HTcpClient(io_service &ios, ip::address& local_addr, ip::address& remote_addr, unsigned short port, HNetworkApp* app);

	void start();
	void stop();

public:
	void restart(const boost::system::error_code& err);
	bool is_connected(); 
	void on_connect(const boost::system::error_code& err);
	void do_read();
    void on_read(const boost::system::error_code& err);
    void send_msg(char* msg,int length);
    void do_write(const std::string msg);
	void on_write(const boost::system::error_code& err, size_t bytes);

    //外部调用start返回指针 暂时用不到
    static HTcpClientPtr start(io_service &ios, const ip::address& local_addr, ip::address& remote_addr, unsigned short port);
private:
    HMsg m_msg;
	bool b_is_connected;
	ip::tcp::socket sock_;
	ip::tcp::endpoint local_ep;
	ip::tcp::endpoint remote_ep;
	enum {max_msg = 1024};
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	deadline_timer connect_timer;
    HNetworkApp* m_pNetworkApp;
};
/*
class AsyncTCPClient : public boost::noncopyable
{
public:
	AsyncTCPClient()
	{
		//work_ptr.reset(new asio::io_service::work(ios));
	}
	
public:
	void read_option();
	void create_session();
public:
	void create_threads(unsigned short thread_pool_size);
	void run();
	void stop();

private:
	ip::address local_addr;
	ip::address remote_addr;
	unsigned short port_num;
	enum { defalut_thread_pool_size = 3 };
	typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;
	typedef boost::shared_ptr<boost::thread> thread_ptr;
	typedef boost::shared_ptr<io_service> io_service_ptr;
	io_service ios;
	//std::map<int, std::shared_ptr<Session>> m_active_sessions;
	//std::mutex m_active_sessions_guard;
	//std::unique_ptr<boost::asio::io_service::work> m_work;
	//std::unique_ptr<std::thread> m_thread;
	std::vector<thread_ptr> threads_pool;
};

*/
#endif // HCLIENT_H
