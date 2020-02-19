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
#include "common_header.h"
using namespace std;
using namespace cv;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     switch (event) {
     case EVENT_LBUTTONUP: userInput.mMouse.mouseBut = LeftBut; break;
     case EVENT_RBUTTONDOWN: userInput.mMouse.mouseBut = RightBut; break;
     case EVENT_MBUTTONDOWN: userInput.mMouse.mouseBut = MidBut; break;
     case EVENT_MOUSEMOVE: userInput.mMouse.mMousePos = {x,y}; break;
     case EVENT_MOUSEWHEEL:
         if (getMouseWheelDelta(flags)>0)
             userInput.mMouse.MidleButScrollPos -= userInput.mMouse.MidleButScrollPos;
         else
             userInput.mMouse.MidleButScrollPos += userInput.mMouse.MidleButScrollPos;
     }
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
         namedWindow("My Window", 1);
         setMouseCallback("My Window", CallBackFunc, NULL);
         auto key = waitKey(1);
         switch (key) {
             case 119: userInput.mKey = w; break;
             case 100: userInput.mKey = d; break;
             case 97:  userInput.mKey = a; break;
             case 115: userInput.mKey = s; break;
             case 114: userInput.mKey = q; break;
             case 101: userInput.mKey = e; break;
         }
         n = write(newsockfd,&userInput,sizeof (userInput));
         if (n < 0) error("ERROR writing user input to socket");
     }
     close(newsockfd);
     close(sockfd);
     return 0;
}
