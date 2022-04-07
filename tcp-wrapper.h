


#ifndef H_TCP_WRAPPER
#define H_TCP_WRAPPER

#include <string>

constexpr size_t C_MTU = 512; /** <Chat MTU */

/**
 * Class TcpWrapper
 */
class TcpWrapper
{
public:
  /**
   * Class constructor.
   * @param port_num integer with the port number
   */
  TcpWrapper(int port_num);

  /**
   * Class destructor.
   */
  ~TcpWrapper();

  /**
   * Accept a new connection and store the IP.
   * @return false if accept fails
   */
  bool acceptClient();

  /**
   * Receives data from remote host.
   * @param buf buffer where the received data is stored
   * @return number of bytes received (-1 if error occurred)
   */
  int recvData(char* buf);

  /**
   * Send data to remote host.
   * @param buf pointer to the buffer where the data to be sent is stored
   * @param size_to_tx size to transmit
   * @return number of bytes sent (-1 if error occurred)
   */
  int sendData(const char* buf, size_t size_to_tx);

   int getPort() const;
  std::string getIp() const;
   bool getExit() const;
   int get1_sock_fd()const;
  int getSock_fd() const;
  // Exercise 5: declare and implement get methods for the 5 variables
  // Remember to enforce const correctness

private:
  int m_port; /**< listen socket port number */
  std::string m_ip; /**< address of who is connected */
  bool m_exit; /**< true if run should end */
  int m_l_sock_fd; /**< listener socket file descriptor */
  int m_sock_fd; /**< data socket file descriptor */
 
  /**
   * Set the socket options and bind.
   * @return false as soon as any of the operations it performs fails
   */
  bool optionsAndBind();

  /**
   * Function executed in a thread dispatched by the constructor 
   * to start the server listener
   */
  void run();

};

#endif /* H_TCP_WRAPPER */
