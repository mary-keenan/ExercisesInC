/* Code from Head First C.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
    int d_sock, bytes_received;
    char buf[255], rec[256];
    struct sockaddr_in si;

    /* connect to server */
    d_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&si, 0, sizeof(si));
    si.sin_family = PF_INET;
    si.sin_addr.s_addr = inet_addr("127.0.0.1");
    si.sin_port = htons(30000);
    connect(d_sock, (struct sockaddr *) &si, sizeof(si));

    /* display advice on stdout */
    bytes_received = recv(d_sock, rec, 255, 0);
    rec[bytes_received] = '\0';
    printf("%s\n", rec);

    /* print reply */
    printf("Get off my back, Mom!\n");
    
    close(d_sock);
    return 0;
}
