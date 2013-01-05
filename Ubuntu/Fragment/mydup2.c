/*
 * =====================================================================================
 *
 *       Filename:  mydup2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年01月06日 02时10分26秒
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


int mydup2(const int fildes1, const int fildes2);
int main(int argc, char* argv[])
{
	printf("%d\n", mydup2(0, 1));
}

int mydup2(const int fildes1, const int fildes2)
{
	int tmp;

	if(fildes1 == fildes2)
		return fildes2;
	tmp = dup(fildes1);
	if(tmp > fildes2)
	{
		close(fildes2);
		tmp = dup(fildes1);
	}
	printf("try: %d\n", tmp);
	if(tmp != fildes2)
		tmp = mydup2(fildes1, fildes2);
	return tmp;
}
