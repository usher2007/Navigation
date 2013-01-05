/*
 * =====================================================================================
 *
 *       Filename:  createFile.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年01月02日 19时31分03秒
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
char buf3[] = "c";

int main(void)
{
	int fd, fd2, i = 0;
	if((fd = creat("file.hole", FILE_MODE)) < 0)
		err_sys("create error");

	if((fd2 = creat("file.nohole", FILE_MODE)) < 0)
		err_sys("create file2 error");

	while(i<16394)
	{
		if(write(fd2, buf3, 1) != 1)
			err_sys("file2 write error");
		i++;

	}

	if(write(fd, buf1, 10) != 10)
		err_sys("buf1 write error");

	if(lseek(fd, 16384, SEEK_SET) == -1)
		err_sys("lseek error");

	if(write(fd, buf2, 10) != 10)
		err_sys("buf2 write error");

	exit(0);

}
