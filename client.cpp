#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
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
    exit(0);
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


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
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
         mousePos.y = y;
     }
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
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
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    int camera_ind = 0;
    if (argc > 2){
        camera_ind = atoi(argv[3]);
        cout<<"Camera index ="<<"\t"<<camera_ind<<endl;
    }
    else{
        cout<<"Please give camera index as parameter!"<<endl;
        return 1;
    }
    cv::VideoCapture cap;
    if(!cap.open(camera_ind))
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

        int height = img.rows;
        int width = img.cols;

        Mat cropped = Mat(img, Rect(width/2 - width/7,
                                           height/2 - height/9,
                                           2*width/7, 2*height/7));
        img = cropped;

        IM_HEIGHT = FRAME_HEIGHT;
        IM_WIDTH = FRAME_WIDTH;

        resize(img, img, Size( IM_WIDTH , IM_HEIGHT ));

        imgSize=img.total()*img.elemSize();

        n = send(sockfd, img.data, imgSize, 0);
        if (n < 0) error("ERROR writing to socket");

        //Create a window
        namedWindow("My Window", 1);

        //set the callback function for any mouse event
        setMouseCallback("My Window", CallBackFunc, NULL);

        //show the image
        imshow("My Window", img);
        waitKey(1);

        // Send data here
        n = send(sockfd, img.data, imgSize, 0);
        if (n < 0)
             error("ERROR writing to socket");
        n = write(sockfd,&mousePos,sizeof (mousePos));
        if (n < 0)
             error("ERROR writing to socket");
        n = read(sockfd,&mousePos,sizeof (mousePos));
        if (n < 0)
             error("ERROR reading from socket");
        cout<<mousePos.x<<"\t"<<mousePos.y<<endl;
    }
    close(sockfd);
    return 0;
}
