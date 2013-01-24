/*
 * =====================================================================================
 *
 *       Filename:  AppendReadWrite.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年01月06日 03时02分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "apue.h"
#include "myerr.h"
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";
char buf3[] = "XXXXXXXXXX";

int main(void)
{
	int fd, i = 0;
	char buf[1024];

	if((fd = open("appendfile.txt", O_RDWR | O_APPEND)) < 0)
		err_sys("create error");
	while(i<10)
	{
		if(write(fd, buf1, 10) != 10)
			err_sys("write error");
		i++;
	}

	if(lseek(fd, 50, SEEK_SET) == -1)
		err_sys("lseek error");
	if(read(fd, buf, 10) < 0)
		err_sys("read error");
	buf[10] = '\0';
	printf("%s\n", buf);

	if(lseek(fd, 50, SEEK_SET) == -1)
		err_sys("lseek error");
	if(write(fd, buf3, 10) != 10)
		err_sys("write error");

	close(fd);
	return 0;
}
