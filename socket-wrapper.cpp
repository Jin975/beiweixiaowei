
#include "socket-wrapper.h"

std::ostream& operator<<(std::ostream& out, const SocketWrapper& wrapper)
{
	wrapper.print(out);
	return out;
}
