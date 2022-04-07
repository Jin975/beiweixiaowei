/* 
 * [2020] Northeastern University - Network Programming
 * [2019] Signet Lab - Programming for ICT
 * All Rights Reserved.
 * 
 * Authors Filippo Campagnaro and Michele Polese 
 */

#include <udp-server.h>
#include <string.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

UdpServer::UdpServer(int port_num)
:
EthConn(port_num),
sock_fd(-1)
{
  // Exercise 4: implement the code that
  // 1- opens a UDP socket and assigns the file descriptor to sock_fd
     sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  // 2- checks if the socket was open correctly, if not, prints an error and returns
     if (sock_fd < 0) {
         std::cout << "ERROR: OPEN SOCKET" << std::endl;
         return;
    }
  // 3- binds the socket to the port specified as input and any interface,
  // and sets the SO_REUSEADDR option
  // 4- checks if this operation was successful, if not, prints an error and returns
    if(!optionsAndBind())
    {
        std::cout << "ERROR bind" << std::endl;
        close (sock_fd);
        return;
    }
}

UdpServer::~UdpServer() {
  if(sock_fd >= 0) {
    close(sock_fd);
  }
}

bool UdpServer::optionsAndBind() 
{
  int option(1);
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, 
                  (char*)&option, sizeof(option));
  struct sockaddr_in my_addr = {0}; // set all elements of the struct to 0
  my_addr.sin_family = AF_INET; // address family is AF_INET (IPV4)

  // convert port to network number format
  my_addr.sin_port = htons(port); 
  // receive data from all interfaces of the Network Interface Card (NIC)
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  // bind the socket to the port
  if (bind(sock_fd,(struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) { 
    std::cout << "ERROR: BIND SOCKET" << std::endl;
    close(sock_fd);
    return false;
  }
  return true;
}

int UdpServer::receiveFrom(std::shared_ptr<std::array<char,MTU>> buf, 
  sockaddr_in& srcaddr, socklen_t addrlen)  
{
  int rcv_size = recvfrom(sock_fd,buf->data(), buf->max_size(), 0,
    (struct sockaddr *)&srcaddr, &addrlen);

  if(rcv_size < 0) {
    std::cout << "ERROR: RECV" << std::endl;
    close(sock_fd);
    sock_fd = -1;
  }
  return rcv_size;
}


int UdpServer::transmitTo(std::shared_ptr<std::array<char,MTU>> buf, 
  size_t size_to_tx, sockaddr_in& srcaddr, socklen_t addrlen) 
{
  // Exercise 5: implement this method, which has to perform the following operations:
  // 1- send a UDP packet using sock_fd
    int sent_data = sendto(sock_fd, buf->data(), size_to_tx, 0, (struct sockaddr*)&srcaddr, addrlen);
  // 2- check if the send operation was successful, if not, closes the socket and returns a negative number
    if (sent_data < 0) { std::cout << "ERROR:SEND" << std::endl; close(sock_fd); return -1; }
    return sent_data;
}

void UdpServer::print(std::ostream& out) const
{
    std::cout << "Port:" << port << std::endl;
  // Exercise 1: implement this method to print some relevant info on a UdpServer
}
