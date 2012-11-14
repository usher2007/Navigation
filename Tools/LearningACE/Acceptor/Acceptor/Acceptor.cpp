#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include <iostream>

class My_Svc_Handler: public ACE_Svc_Handler <ACE_SOCK_STREAM,ACE_NULL_SYNCH>
{
public:
	int open(void *)
	{
		std::cout<<"Connection established"<<std::endl;
		return 0;
	}
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