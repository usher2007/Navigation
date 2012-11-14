#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Connector.h"
#include "ace/Synch.h"
#include "ace/SOCK_Connector.h"
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

typedef ACE_Connector<My_Svc_Handler, ACE_SOCK_CONNECTOR> MyConnector;

int main(int argc, char *argv[])
{
	ACE_INET_Addr addr(1358, "192.168.0.56");
	My_Svc_Handler *handler = new My_Svc_Handler;

	MyConnector connector;

	if(connector.connect(handler, addr) == -1)
	{
		return -1;
	}

	while(1)
		ACE_Reactor::instance()->handle_events();

	return 0;
};