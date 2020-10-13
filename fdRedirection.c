#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*
 * Rediret stdout to file and then 
 * restore stdout to terminal after finished.
 * */

int main(int argc, char** argv)
{
	int outfd = dup(1);
	int fd;

	fd = open("log.txt", O_CREAT|O_RDWR|O_APPEND);

	if(fd < 0 )
	{
		perror("open()");
		exit(1);
	}


	if(dup2(fd,1) < 0)
	{
		perror("dup2()");
		exit(2);
	}
	close(fd);
	write(1, "hellodd\n",8);
	printf("test123454\n");
	printf("the saved stdout fd is:=%d\n",outfd);
	fflush(NULL);
	close(1);

	// restore stdout to terminal
	if(dup2(outfd, 1)< 0)
	{
		perror("stdout dup2()");
		exit(3);
	}
	write(1, "I am back\n",10);

	puts("Have restroed to terminal");
	printf("stdout....\n");

	exit(0);
}
