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
#define FRAME_WIDTH         640
#define FRAME_HEIGHT        480
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

     int IM_HEIGHT, IM_WIDTH, imgSize, bytes = 0;
     Mat img;
     while(true) {
         IM_HEIGHT = FRAME_HEIGHT;
         IM_WIDTH = FRAME_WIDTH;
         img = Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);

         imgSize = img.total()*img.elemSize();
         uchar sockData[imgSize];

         for(int i=0;i<imgSize;i+=bytes)
           if ((bytes=recv(newsockfd, sockData+i, imgSize-i,0))==-1) error("recv failed");

         int ptr=0;

         for(int i=0;i<img.rows;++i)
           for(int j=0;j<img.cols;++j)
           {
             img.at<Vec3b>(i,j) = Vec3b(sockData[ptr+0],sockData[ptr+1],sockData[ptr+2]);
             ptr=ptr+3;
           }

         cout<<"img="<<img.at<Vec3b>(55,55)<<endl;

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
