#include "udp-wrapper.h"
#include <cstddef>
#include <string.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include"socket-wrapper.h"

UdpWrapper::UdpWrapper(int port_num)
	:
	m_port{ port_num },
	m_last_recv_ip{0},
	m_last_addr_len{ 0 },
	m_exit{ false },
	m_sock_fd{ -1 }

{
	//create socket 
	m_sock_fd= socket(AF_INET, SOCK_DGRAM, 0);
//check if the socket was successful
	if (m_sock_fd < 0) 
{
	std::cout << "Error:open socket"; return ;
}
	//bind
if (!optionsAndBind()) 
{
	std::cout << "ERROR: BIND" << std::endl;
	close(m_sock_fd);
	return;
}
}
UdpWrapper::~UdpWrapper() 
{
	quit();
}

void UdpWrapper::quit() 
{
	m_exit = true;
	//close the socket
	if (m_sock_fd > 0)
	{
		close(m_sock_fd);
		m_sock_fd = -1;
	}
}
bool
UdpWrapper::optionsAndBind() 
{//set options for the socket
	int option(1);
	setsockopt(m_sock_fd, SOL_SOCKET, SO_REUSEADDR,
		(char*)&option, sizeof(option));
	struct sockaddr_in my_addr = { 0 };
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(m_port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//bind the socket to the m_port
	if (bind(m_sock_fd, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {

		std::cout << "bind socket address failed";
		if (m_sock_fd > 0)
		{
			close(m_sock_fd);
		}
		return false;
	}

	return true;
}
	void
		UdpWrapper::runServer() 
	{//create a char buffer
		char buf[C_MTU];
		//start a while loop to receive data and send application ACKs to this client
		while (!m_exit)
		{ int recv_data = recvData(buf);
			
		if (recv_data < 0) {
				std::cout << "ERROR UDP RECV" << std::endl;
				m_exit = true;
				break;
			}
			std::cout << "RECV DATA:" << buf << std::endl;
			int send_data = sendData("ACK", 3);

			if (send_data < 0)
			{
				std::cout << "ERROR: UDP SEND" << std::endl;
				m_exit = true;
				break;
			}

			memset(buf, 0, C_MTU);
		}
	}




	int UdpWrapper::recvData(char* buf)
	{
		if (m_sock_fd <0 || m_exit) 
		{
		return -1;
		}
		m_last_addr_len = sizeof(m_last_recv_ip);

		int size = recvfrom(m_sock_fd, buf, C_MTU, 0, (sockaddr*)&m_last_recv_ip, &m_last_addr_len);

		m_last_recv_ip.sin_addr.s_addr = m_sock_fd;
		if (size < 0) {
			std::cout << "ERROR RECV" << std::endl;
				close(m_sock_fd);
				m_sock_fd = -1;
				return -3;
			
		}
		return size;
	}




	int
		UdpWrapper::sendData(const char* buf, size_t size_to_tx)
	{

		if (m_sock_fd < 0||m_exit) 
		{
				return -1;}
		int d_bytes = sendto(m_sock_fd, buf, size_to_tx, 0, (struct sockaddr*)&m_last_recv_ip, m_last_addr_len); // send
		if (d_bytes<0) 
		{
			std::cout << "ERROR SENDING" << std::endl;
			close(m_sock_fd);
			return -4;
		}
		return d_bytes;
	}



	void
		UdpWrapper::print(std::ostream& out)const
	{
		out << "UDP socket on port" << m_port<<std::endl;
	}

	std::string UdpWrapper::getLastIp() const
	{
		return std::to_string(m_last_recv_ip.sin_addr.s_addr);
	}
