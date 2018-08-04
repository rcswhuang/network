#include "hclient.h"

HTcpClient::HTcpClient(io_service &ios, ip::address& local_addr, ip::address& remote_addr, unsigned short port, HNetworkApp* app)
:sock_(ios), connect_timer(ios, boost::posix_time::seconds(10)),m_pNetworkApp(app)
{
	b_is_connected = false;
	local_ep.address(local_addr);
	local_ep.port(port);
	sock_.bind(local_ep);

	remote_ep.address(remote_addr);
	remote_ep.port(port);
	//启动定时器
	//connect_timer.expires_at(boost::posix_time::seconds(1));
	connect_timer.async_wait(&restart);
}

void HTcpClient::start()
{
    sock_.async_connect(remote_ep, boost::bind(on_connect, this,asio::placeholders::error));
}

void HTcpClient::restart(const boost::system::error_code& err)
{
	if (!is_connected())
	{
		connect_timer.expires_at(connect_timer.expires_at() + boost::posix_time::seconds(10));
		connect_timer.async_wait(&restart);
		start();
	}
}
bool HTcpClient::is_connected()
{
	return b_is_connected;
}

void HTcpClient::on_connect(const boost::system::error_code& err)
{
	if (!err)
	{
		b_is_connected = true;
		do_read();
	}
	else
	{
		b_is_connected = false;//未连接上
		//stop();
	}
}

void HTcpClient::do_read()
{
    async_read(sock_, asio::buffer(m_msg.data(),m_msg.data_length()), boost::bind(on_read,this,asio::placeholders::error));
	//sock_.async_read_some();
}
void HTcpClient::do_write(const std::string msg)
{
	//if (!started()) return;
	std::copy(msg.begin(), msg.end(), write_buffer_);
    sock_.async_write_some(buffer(write_buffer_), boost::bind(on_write,this,asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
}


void HTcpClient::on_read(const boost::system::error_code& err)
{
	if(err) stop();
    std::string time = to_simple_string(second_clock::local_time());
    int ip = remote_ep.address().to_v4().to_ulong();
    m_pNetworkApp->handle_recv(m_msg.msg(),(int)m_msg.msg_length(),ip,time);
    do_read();
}

void HTcpClient::on_write(const boost::system::error_code& err, size_t bytes)
{
	if (err) do_read();
}

void HTcpClient::send_msg(char* msg,int length)
{

}

/*
HTcpClientPtr HTcpClient::start(io_service &ios, const ip::address& local_addr, ip::address& remote_addr, unsigned short port, Callback callback)
{
    HTcpClientPtr new_(new HTcpClient(ios, local_addr, remote_addr, port, callback));
	new_->start();
	return new_;
}*/



/////////////////////////////////////////////////////客户端类////////////////////////////////////////////
 /*
void AsyncTCPClient::create_threads(unsigned short thread_pool_size)
{
	if (thread_pool_size > defalut_thread_pool_size)
		thread_pool_size = defalut_thread_pool_size;

	for (unsigned int i = 0; i < thread_pool_size; i++)
	{
		boost::shared_ptr<boost::thread> th(new boost::thread(boost::bind(&io_service::run, ios)));
		threads_pool.push_back(th);
	}
}

void AsyncTCPClient::run()
{
	create_threads(3);
	for (std::size_t i = 0; i < threads_pool.size(); i++)
		threads_pool[i]->join();
}

void AsyncTCPClient::stop()
{
	ios.stop();
}*/
