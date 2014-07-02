#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "socket.h"

int main(int argc, char *argv[])
{
    
    if (argc < 3) {
       printf("ERROR, usage %s hostname port \n", argv[0]);   
       printf("$ %s localhost 123456\n",argv[0]);
       exit(0);
    }

    // Create socket
    lesocket meusocket;
    // Connect on ip:port
    meusocket.soconnect(argv[1],atoi(argv[2])); 

    // Send protocol
    char protocol[]={'a','B','c','D','e','F'};
    for (int i = 0; i < strlen(protocol); ++i)
        meusocket.sowrite(protocol[i]);      

    // Read ok_byte
    printf("Server said: %c\n",meusocket.soread());                        
    // Close the socket
    meusocket.soclose(); 


    exit(1);
}
