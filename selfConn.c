#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>

#define TRY_CNT 60000

void exec_cmd(char *cmd) {
        printf("\n**********start exec cmd: %s **********\n\n", cmd);
        system(cmd);
        printf("\n\n**********finish exec cmd: %s **********\n", cmd);
}

int main(int argc, char *argv[])
{

        int cnt ;
        int sock;
        int ret;

        struct sockaddr_in seraddr, cliaddr;
        socklen_t addrlen;

        char cmd[1024];
        int port;

        if(argc < 2)
        {
                printf("need port arg. usage:\nselfconntest port\n");
                return 1;
        }

        snprintf(cmd, sizeof(cmd), "netstat -npt | grep %s", argv[1]);
        port = atoi(argv[1]);

        if(port < 1000 || port > 65535)
        {
                printf("port should be 1000 - 65535\n");
                return 1;
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if( sock < 0 )
        {
                printf("socket() error. errno = %d, errstr = %s\n",
                                errno, strerror(errno));
                return -1;
        }

        // init seraddr

        memset(&seraddr, 0, sizeof(seraddr));
        seraddr.sin_family = AF_INET;
        seraddr.sin_port = ntohs(port);

        if( inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr.s_addr) < 0 )
        {
                printf("inet_pton() error");
                return -2;

        }

        printf("remote addr:\t ip = %X, port = %u\n",
                        ntohl(seraddr.sin_addr.s_addr), ntohs(seraddr.sin_port));

        for(cnt = 0; cnt < TRY_CNT; cnt++)
        {
                ret = connect(sock, (const struct sockaddr *)&seraddr, sizeof(seraddr));
                if( ret == 0 )
                {
                        printf("try %d times, connect succ.\n", cnt + 1);
                        // get local addr info
                        addrlen = sizeof(cliaddr);
                        ret = getsockname(sock, (struct sockaddr *)&cliaddr, &addrlen);
                        if( ret < 0 )
                        {
                                printf("getsockname () error. errno = %d, errstr = %s\n",
                                                errno, strerror(errno));
                        } else
                        {
                                printf("local addrr:\t ip = %X, port = %u\n",
                                                ntohl(cliaddr.sin_addr.s_addr), ntohs(cliaddr.sin_port));
                                exec_cmd(cmd);
                        }
                        // pause();
                        sleep(1);
                        break;

                }

                // only print error message of first time and last time

                if( cnt == 0 || cnt == TRY_CNT - 1) {
                        printf("connect () error. errno = %d, errstr = %s\n",
                                        errno, strerror(errno));

                }

        }

        printf("cnt = %d\n", cnt+1);
        close(sock);
        sleep(2);
        exec_cmd(cmd);
        return 0;

}
