/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

struct MousePos {
    int x = -1;
    int y = -1;
} mousePos;

void GetMousePos(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          mousePos.x = x;
          mousePos.y = y;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
         mousePos.x = x;
         mousePos.y = y;     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
         mousePos.x = x;
         mousePos.y = y;     }
     else if ( event == EVENT_MOUSEMOVE )
     {
         mousePos.x = x;
         mousePos.y = y;     }
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");

     printf("server: got connection from %s port %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

     while(true) {
         n = read(newsockfd,&mousePos,sizeof (mousePos));
         if (n < 0) error("ERROR reading from socket");
         cout<<mousePos.x<<"\t"<<mousePos.y<<endl;
         n = write(newsockfd,&mousePos,sizeof (mousePos));
         if (n < 0) error("ERROR writing to socket");
     }
     close(newsockfd);
     close(sockfd);
     return 0;
}
