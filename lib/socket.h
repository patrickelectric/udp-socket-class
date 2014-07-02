/**
  ******************************************************************************
  * @file    socket.h
  * @author  Patrick José Pereira
  * @version V1.3
  * @date    2-Jul-2014
  * @brief   A library to make comunication with socket
  *
  ******************************************************************************/
#ifndef lesocket_h
#define lesocket_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <netdb.h> 

class lesocket
{
  private:
  	//client variables
  	int n;
  	int sockfd;
  	int newsockfd;

  	//client functions
    void error(const char* msg);

  public:
  	//client 
  	int  soavaiable();
	  char soread();
	  void sowrite(char byte);
    void soclose();
    void soconnect(char* host, int port);

    //server
    void secreate(int port);
    void seaccept();
    int  seavaiable();
    char seread();
    void sewrite(char byte);
    void seclose();

};

#endif