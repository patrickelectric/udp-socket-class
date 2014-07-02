#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

int main(int argc, char *argv[])
{

    if (argc < 2) 
    {
        printf("ERROR, no port provided\n");
        printf("$ %s 123456\n",argv[0]);
        exit(1);
    }

    // Create class
    lesocket meuscoket;     

    // Create server
    meuscoket.secreate(atoi(argv[1]));      
    // Accept connections
    meuscoket.seaccept();                   

    sleep(1);

    // Print all avaiables bytes
    printf("Client said: ");
    while(meuscoket.seavaiable()>0) printf("%c",meuscoket.seread()); printf("\n");

    // Send ok_byte
    char byte='P';
    meuscoket.sewrite(byte);  
    // Close socket
    meuscoket.seclose();      

    exit(1);
}
