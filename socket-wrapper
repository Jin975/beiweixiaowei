/*
 * [2020] Northeastern University - Network Programming
 * All Rights Reserved.
 * 
 * Author Michele Polese and Pragadeesh Kumar Nithyakumar 
*/


#ifndef H_SOCKET_SERVER
#define H_SOCKET_SERVER

#include <string>
#include <iostream>

constexpr size_t C_MTU = 512; /** <Chat MTU */

/**
 * Class TcpWrapper
 */
class SocketWrapper
{
public:
  /**
   * Class destructor.
   */
  virtual ~SocketWrapper() {};

  /**
   * Receives data from remote host.
   * @param buf buffer where the received data is stored
   * @return number of bytes received (-1 if error occurred)
   */
  virtual int recvData(char* buf) = 0;

  /**
   * Send data to remote host.
   * @param buf pointer to the buffer where the data to be sent is stored
   * @param size_to_tx size to transmit
   * @return number of bytes sent (-1 if error occurred)
   */
  virtual int sendData(const char* buf, size_t size_to_tx) = 0;

  /**
   * Run the server
   */
  virtual void runServer() = 0;

protected:
  /**
   * Print something about the socket
   * @param out an ostream to print to
   * @return an ostream
   */
  virtual void print(std::ostream& out) const = 0;

  friend std::ostream& operator<<(std::ostream& out, const SocketWrapper& wrapper);
};

std::ostream& operator<<(const std::ostream& out, const SocketWrapper& wrapper);

#endif /* H_SOCKET_SERVER */
