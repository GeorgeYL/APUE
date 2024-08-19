
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <cstring>
#include <string>
static int ver = 0;
const std::string fileName="./rotateRedirect.log";
int oldStdOut=-1;
void print(int& fd, std::string str)
{
        fflush(NULL);
        struct stat buf;
        int ret = fstat(fd, &buf);
        if(ret < 0)
        {
                perror("fstat()");
                exit(2);
        }
        std::cout << "size="<<buf.st_size << " Bytes.blocks="<< buf.st_blocks<<",blkSize="<< buf.st_blksize << std::endl;;
        if(buf.st_size > 50)
        {
                char buf[128];
                memset(buf,'\0',128);
                sprintf(buf,"%s.%d",fileName.c_str(),++ver);
                std::string newName(buf);
                fflush(NULL);
        FILE *fs = fdopen(fd, "w+");
                if(fs == NULL )
                {
                        perror("fdopen()");
                }
                dup2(oldStdOut,1);
                dup2(oldStdOut,2);

                std::cout << "im back.." << std::endl;
                close(fd);
                fprintf(fs, "test");
                if(fs) fclose(fs);
                int ret = rename(fileName.c_str(), newName.c_str());
                if(ret < 0)
                {
                        perror("rename():");
                        exit(1);
                }
                fd = open(fileName.c_str(), O_RDWR|O_TRUNC|O_CREAT, 0644);
                if(fd < 0)
                {
                        perror("open() again:");
                        exit(0);
                }
                int fdret = dup2(fd, 2);
                fdret = dup2(fd, 1);
                std::cout << "good " << std::endl;

        }
        write(fd, str.c_str(), str.size());

}
int main()
{

        int fd;
    FILE *fs;

    fd = open(fileName.c_str(), O_RDWR|O_TRUNC|O_CREAT, 0644);
        if(fd < 0)
    {
           perror("open()");
           exit(1);
    }
    fs = fdopen(fd, "w+");
    if(fs == NULL)
    {
           perror("fopen()");
           exit(1);
    }
    fprintf(fs,"to REDIRECT 2->1 \n");
        fflush(NULL);
        print(fd, "again");
        oldStdOut = dup(1);
        int fdret = dup2(fd, 2);
    if(fdret < 0)
    {
                perror("dup2 2->1.");
                exit(1);
        }
        std::cerr<< "Redirected 2->1; same as std::err to std::out ["<<fdret <<std::endl;;

    fprintf(fs,"to REDIRECT fd->1 RET\n");
    fdret = dup2(fd, 1);
    if(fdret < 0)
    {
                perror("dup2 fd->1.");
                exit(1);
        }
        std::cout << "Redirected std::out to logfile ["
                << fdret << "]" << std::endl;
        std::cout << "File stream also opened for logfile ["
                << fd << "]" << std::endl;
    printf("fd is:%d\n", fd);
        //close(fd);

        fprintf(stdout, "stdout print test\n");
        fprintf(stderr, "stderr print test\n");

        std::cout << "hell" << std::endl;

    print(fd,"Hello");  /*
        pid_t pid = fork();
        if(pid < 0)
        {
                perror("fork()");
                exit(1);
        }
        else if(pid == 0)
        {
                //close(1);
                //close(2);
                std::cout << "child" << std::endl;
                int fd = open("/dev/console", O_RDWR, 0644);
                if(fd < 0)
                {
                        perror("open()");
                        exit(1);
                }
                execlp("/usr/bin/top","top", NULL);

                perror("execlp()");
                exit(1);
        }
        else
        */{}


        std::cout << "main is wating" << std::endl;
        fflush(NULL);

        std::cout << "main is exiting" << std::endl;
        close(fd);

        exit(0);
}



