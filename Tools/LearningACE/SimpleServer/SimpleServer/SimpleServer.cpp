#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Thread.h"
#include "ace/Get_Opt.h"
#include <iostream>

class My_Svc_Handler;
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;

typedef ACE_Acceptor<My_Svc_Handler, ACE_SOCK_ACCEPTOR> Acceptor;

typedef ACE_Connector<My_Svc_Handler, ACE_SOCK_CONNECTOR> Connector;

#define  DATA_SIZE 12
class My_Svc_Handler: public ACE_Svc_Handler <ACE_SOCK_STREAM,ACE_NULL_SYNCH>
{
public:
	static ACE_SOCK_Stream* Peer;
	ACE_thread_t t_id;

	My_Svc_Handler()
	{
		data = new char[DATA_SIZE];
	}

	int open(void *)
	{
		std::cout<<"Acceptor: received new connection"<<std::endl;
		Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);

		My_Svc_Handler::Peer = &peer();
		ACE_Thread::spawn((ACE_THR_FUNC)send_data, 0, THR_NEW_LWP, &t_id);
		return 0;
	}

	static void* send_data(void *)
	{
		while(1)
		{
			std::cout<<">>Hello World"<<std::endl;
			Peer->send_n("Hello World", sizeof("Hello World"));
			
			ACE_OS::sleep(1);
		}
		return 0;
	}

	int handle_input(ACE_HANDLE)
	{
		if(Peer->recv_n(data, DATA_SIZE) == 0)
		{
			std::cout<<"Peer probably aborted connection";
			ACE_Thread::cancel(t_id);
			return -1;
		}
		ACE_OS::printf("<< %s\n", data);
		return 0;
	}
private:
	char *data;
};

ACE_SOCK_Stream * My_Svc_Handler::Peer = 0;
#define  PORT_NO 1358
void main_accept()
{
	ACE_INET_Addr addr(PORT_NO);
	Acceptor myacceptor(addr, Reactor::instance());
	while(1)
		Reactor::instance()->handle_events();

	return;
}

void main_connect()
{
	ACE_INET_Addr addr(PORT_NO, "127.0.0.1");
	Connector myconnector;
	My_Svc_Handler *my_svc_handler = new My_Svc_Handler;
	myconnector.connect(my_svc_handler, addr);
	while(1)
		Reactor::instance()->handle_events();

	return;
}

int main(int argc, char* argv[])
{
	static const ACE_TCHAR options[] = ACE_TEXT (":i:");
	ACE_Get_Opt cmd_opts(argc, argv, options);
	int option;
	ACE_TCHAR optValue[1024];
	while((option = cmd_opts())!=EOF)
	{
		switch(option)
		{
		case 'i':
			ACE_OS_String::strncpy(optValue, cmd_opts.opt_arg(), 1024);
			break;
		default:
			break;
		}
	}

	if(ACE_OS_String::strcmp(optValue, "S") == 0)
	{
		main_accept();
	}
	else if(ACE_OS_String::strcmp(optValue, "C") == 0)
	{
		main_connect();
	}
	return 0;
};