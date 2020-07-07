#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define	BUF_LEN		1024

int main()
{

	// get current work directory.
	char buf[BUF_LEN] = {'\0'};
	char res_path[BUF_LEN] = {'\0'};

    char * pcwd = getcwd(buf, BUF_LEN);

	fprintf(stdout,"CurentWorkDirectory:%s\n", buf);


	// resolute relative path to absolute path
	//
	fprintf(stdout, "relative path is:../../working/\n");
	strcpy(buf, "/home/YunzhiLi/client_lme/");
	char *ret = realpath(buf, res_path);
    if( ret == NULL)
	{
		perror("realpath()");
		exit(1);
	}
	fprintf(stdout, "ResolvedPath:%s\n",res_path);
	exit(0);
}
