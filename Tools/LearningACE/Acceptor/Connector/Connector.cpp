#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Connector.h"
#include "ace/Synch.h"
#include "ace/SOCK_Connector.h"
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

		ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
		return 0;
	}

	int handle_output(ACE_HANDLE)
	{
		strcpy(data, "12345678901");
		peer().send_n(data, 12);
		peer().send_n(data, 12);
		ACE_OS::printf("Send: %s\n", data);
		return 0;
	}
private:
	char *data;
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