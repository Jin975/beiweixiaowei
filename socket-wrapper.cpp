/*
 * [2020] Northeastern University - Network Programming
 * All Rights Reserved.
 * 
 * Author Michele Polese and Pragadeesh Kumar Nithyakumar 
*/

#include "socket-wrapper.h"

std::ostream& operator<<(std::ostream& out, const SocketWrapper& wrapper)
{
	wrapper.print(out);
	return out;
}
