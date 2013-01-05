/*
 * =====================================================================================
 *
 *       Filename:  MyTerm.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年12月29日 12时39分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "apue.h"
#include <sys/wait.h>
#include "myerr.h"

static void sig_int(int);

int main(void)
{
	char buf[MAXLINE];
	pid_t pid;
	int status;

	if(signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal error");

	printf("%% ");
	while(fgets(buf, MAXLINE, stdin) != NULL)
	{
		if(buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;

		if((pid = fork()) < 0)
		{
			err_sys("fork error");
		}
		else if(pid == 0)
		{
			execlp(buf, buf, (char *)0);
			err_ret("couldn't execute: %s", buf);
			exit(127);
		}

		if ((pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error");

		printf("%% ");
	}

	exit(0);
}

void sig_int(int signo)
{
	printf("interrupt\n%% ");
}
