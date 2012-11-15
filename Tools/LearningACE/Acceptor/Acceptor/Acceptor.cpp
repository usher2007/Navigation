#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include <iostream>

#define  DATA_SIZE 12
class My_Svc_Handler: public ACE_Svc_Handler <ACE_SOCK_STREAM,ACE_NULL_SYNCH>
{
public:
	My_Svc_Handler()
	{
		data = new char[DATA_SIZE];
	}
	
	int open(void *)
	{
		std::cout<<"Connection established"<<std::endl;

		ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
		return 0;
	}

	int handle_input(ACE_HANDLE)
	{
		peer().recv_n(data, DATA_SIZE);
		ACE_OS::printf("<< %s\n", data);
		return 0;
	}
private:
	char *data;
};

typedef ACE_Acceptor<My_Svc_Handler, ACE_SOCK_ACCEPTOR> MyAcceptor;

int main(int argc, char* argv[])
{
	ACE_INET_Addr addr(1358);
	MyAcceptor acceptor(addr, ACE_Reactor::instance());

	while(true)
	{
		ACE_Reactor::instance()->handle_events();
	}
	return 0;
}