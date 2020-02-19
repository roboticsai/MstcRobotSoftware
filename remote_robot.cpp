#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "common_header.h"
#include <iostream>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define FRAME_WIDTH         640
#define FRAME_HEIGHT        480

using namespace std;
using namespace cv;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    cv::VideoCapture cap;
    if(!cap.open(-1))
        return 0;
     // Read image from file
    Mat img;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,460);
    int IM_HEIGHT, IM_WIDTH, imgSize;

    while(true) {
        cap >> img;
        //if fail to read the image
        if ( img.empty() )
        {
             cout << "Error loading the image" << endl;
             return -1;
        }

        imgSize=img.total()*img.elemSize();

        n = send(sockfd, img.data, imgSize, 0);
        if (n < 0) error("ERROR writing img to socket");

        n = read(sockfd,&userInput,sizeof (userInput));
        if (n < 0)
             error("ERROR user input from socket");
        cout<<"Keypress="<< userInput<<endl;
    }
    close(sockfd);
    return 0;
}
