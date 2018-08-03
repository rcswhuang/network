#include "hclient.h"
/*
HSession::HSession(io_service &ios, ip::address& local_addr, ip::address& remote_addr, unsigned short port, Callback callback)
:sock_(ios), m_callback(callback), connect_timer(ios, boost::posix_time::seconds(10))
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

void HSession::start()
{
	sock_.async_connect(remote_ep, boost::bind(on_connect, _1));
}

void HSession::restart(const boost::system::error_code& err)
{
	if (!is_connected())
	{
		connect_timer.expires_at(connect_timer.expires_at() + boost::posix_time::seconds(10));
		connect_timer.async_wait(&restart);
		start();
	}
}
bool HSession::is_connected()
{
	return b_is_connected;
}

//整个过程保证ios时刻都有异步任务在执行
void HSession::on_connect(const boost::system::error_code& err)
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

void HSession::do_read()
{
	async_read(sock_, buffer(read_buffer_), boost::bind(on_read, _1, _2));
	//sock_.async_read_some();
}
void HSession::do_write(const std::string msg)
{
	//if (!started()) return;
	std::copy(msg.begin(), msg.end(), write_buffer_);
	sock_.async_write_some(buffer(write_buffer_), boost::bind(on_write, _1, _2));
}


void HSession::on_read(const boost::system::error_code& err, size_t bytes)
{
	if(err) stop();
	std::string msg(read_buffer_, bytes);
	m_callback(MSG_R_TYPE, msg, msg.size());
}

void HSession::on_write(const boost::system::error_code& err, size_t bytes)
{
	if (err) do_read();
}


ptr_session HSession::start(io_service &ios, const ip::address& local_addr, ip::address& remote_addr, unsigned short port, Callback callback)
{
	ptr_session new_(new HSession(ios, local_addr, remote_addr, port, callback));
	new_->start();
	return new_;
}



/////////////////////////////////////////////////////客户端类////////////////////////////////////////////
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