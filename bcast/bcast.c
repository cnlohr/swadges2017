#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>


#define HELLO_PORT 7878
#define HELLO_GROUP "10.201.233.92"
//#define HELLO_GROUP "10.201.255.255"
//#define HELLO_GROUP "225.0.0.37"

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int fd;
//    char *message = "EHELLO";
//	char *message = "CL\t12\tffffffffffffff";
	char message[100] = "ct	";

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }


	int broadcastEnable=1;
	int ret=setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));


    /* set up destination address */
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    addr.sin_port=htons(HELLO_PORT);

	int frame = 0;
    while (1)
    {
		memset( message+3, (frame++)%10, 12 );
//		memset( message+3, 0xff, 12 );

        if (sendto(fd, message, 15, 0,(struct sockaddr *) &addr, sizeof(addr)) < 0)
        {
            perror("sendto");
            exit(1);
        }
        usleep(16000);
    }
}

