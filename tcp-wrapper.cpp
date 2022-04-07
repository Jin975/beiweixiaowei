

#include <tcp-wrapper.h>
#include <string.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

TcpWrapper::TcpWrapper(int port_num) :
// Exercise 1: implement the constructor of TcpWrapper, which
// does the following:
   
// - uses a member initialization list to initialize the member variables,
//   with m_port initialized to port_num, and the other member variables
//   to appropriate default values
// - in the body of the constructor, open a TCP socket and assign its file
//   descriptor to m_l_sock_fd
// - check if the socket was opened successfully, if not, close and return
// - call the function optionsAndBind, check if successful, if not, close and return
// - call the function run()
    m_port{port_num},
    m_ip{},
    m_exit{false},
    m_l_sock_fd{},
    m_sock_fd{}
{


    
 m_l_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
if (m_l_sock_fd < 0) 
{

    std::cout << "ERROR:OPEN SOCKET" << std::endl;
    close(m_l_sock_fd);
    return ;
}
if (!optionsAndBind())
{
    std::cout << "ERROR:failed to call the function optionAndBind" << std::endl;
    close(m_l_sock_fd);
    return ;
}
run();
}
TcpWrapper::~TcpWrapper() {
// Exercise 2: implement the destructor of TcpWrapper, which
// does the following:
// - sets m_exit to true
// - closes the m_l_sock_fd socket, and assigns it the value -1. This has
//   to happen only if the socket is still open (i.e., the file descriptor
//   is larger than or equal to 0), otherwise you will get an error
// - closes the m_sock_fd socket, if open, and assigns it the value -1.
   
    m_exit=true;
        if (m_l_sock_fd < 0) {
            std::cout << "ERROR:OPEN SOCKET" << std::endl;
            close(m_l_sock_fd);
            m_l_sock_fd = -1;
        }
        
        if (m_sock_fd >= 0) {
            close(m_sock_fd);
            m_sock_fd =-1;

        }
}


void 
TcpWrapper::run() {
  // create a char buffer called buf, of size C_MTU
  char buf[C_MTU];

  // wait for a client to connect
 
  if(acceptClient()) 
  {   
      while (!m_exit)
      {
          int data_size = recvData(buf);
          if (data_size < 0)
          {
              std::cout << "Number of recvdata can't be negative" << std::endl;
              std::cout << " You fail" << std::endl;
              m_exit = true;
              break;


          }
          else if (data_size == 0)
          {
              std::cout << "Tcp connection is closed" << std::endl;
             
              m_exit = true;
              break;


          }

          std::cout << "Received  " << buf << std::endl;
          memset(buf, 0, sizeof(buf));
          char packet[] = "Packet ACK\n";
          int sent_size = sendData(packet, sizeof(packet));

          if ((sent_size) < 0)
              {
              std::cout << "send fail" << std::endl;
              m_exit = true;
              break;
        }
      }
      }

    // Exercise 4: implement the following, inside a while loop that stops if m_exit is true:
    // - call recvData, passing buf, to receive data, and save the
    //   return value of recvData in a data_size variable
    // - check if the data_size is smaller than 0, if yes, set m_exit to true and break the loop
    // - check if the data_size is equal to 0, if yes, set m_exit to true and break the loop
    // - print the proper message in each of the cases above
    // - print the buf to the terminal
    // - reset the buffer for a new read using memset
    // - send a packet back which contains the string "Packet ACK\n". Check if the transmission
    //   is successful, if not, set m_exit to true and break the loop. Use the method sendData
  }



bool 
TcpWrapper::optionsAndBind() {
  // set the options for the socket
  int option(1);
  setsockopt(m_l_sock_fd, SOL_SOCKET, SO_REUSEADDR, 
                  (char*)&option, sizeof(option));
  struct sockaddr_in my_addr = {0}; // set all elements of the struct to 0
  my_addr.sin_family = AF_INET; // address family is AF_INET (IPV4)

  // convert m_port to network number format
  my_addr.sin_port = htons(m_port); 
  // accept connection from all interfaces of the Network Interface Card (NIC)
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  // bind the socket to the m_port
  if (bind(m_l_sock_fd,(struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) { 
    std::cout << "ERROR: BIND SOCKET" << std::endl;
    if(m_l_sock_fd>=0) {
      close(m_l_sock_fd);
    }
    return false;
  }

  // accept at most 1 client at a time, and set the socket in a listening state
  if (listen(m_l_sock_fd, 1) < 0) { 
    std::cout << "ERROR: LISTEN SOCKET" << std::endl;
    
    return false;
  }
  return true;
}


bool 
TcpWrapper::acceptClient() {
  // Exercise 3: implement the acceptClient() method, which does the following:
  // - prepares the struct client_addr to store the client address
  // - calls the accept API on the socket m_l_sock_fd, and saves the file descriptor
  //q   of the new socket in m_sock_fd
  // - if the accept was not successful, close the sockets (if they are open!) and return false
  // - otherwise, print the client ip, and save it in m_ip as a string (you can use inet_ntoa or
  //   inet_pton), and return true
    struct sockaddr_in client_addr;
    socklen_t addr_l = sizeof(client_addr);
   m_sock_fd = accept(m_l_sock_fd, (struct sockaddr*)&client_addr,&addr_l);
    if (m_sock_fd < 0) {
        std::cout << "ERROR: ACCEPT CONNECTION" << std::endl;
        close(m_sock_fd);
        close(m_l_sock_fd);
        return -4;
    }
        std::cout << "New connection from" << inet_ntoa(client_addr.sin_addr) << std::endl;
        std::string m_ip = inet_ntoa(client_addr.sin_addr);
        return 1;
    
}


int 
TcpWrapper::recvData(char* buf)  
{
  if(m_sock_fd < 0) {
    return -1;
  }
  // call the recv API
  int rcv_size = ::recv(m_sock_fd, buf, C_MTU, 0);
  // check if the recv size is ok or not
  if(rcv_size < 0) {
    std::cout << "ERROR: RECV" << std::endl;

    // here we close just m_sock_fd because, in principle,
    // the m_l_sock_fd can be used for other clients (not in this case, though)
    // first, check if the socket is open
    if(m_sock_fd >= 0) { // then close it
      close(m_sock_fd);
      m_sock_fd = -1;
    }
  }
  return rcv_size;
}


int TcpWrapper::sendData(const char* buf, size_t size_to_tx) 
{
  if(m_sock_fd<0) {
    return -1;
  }
  int sent_size = ::send(m_sock_fd,buf,size_to_tx,0); // send the data through sckfd
  if(sent_size < 0) { // the send returns a size of -1 in case of errors
    std::cout << "ERROR: SEND" << std::endl;
    if(m_sock_fd >= 0) {
      close(m_sock_fd);
      m_sock_fd = -1;
    } // if error close the socket and exit
    return -4;
  }
  return sent_size;
}
