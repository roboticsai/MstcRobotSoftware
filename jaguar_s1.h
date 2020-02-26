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
#include <iostream>
#include <netdb.h>

#define PORT_NUM 2324

#include <iostream>
#include <unordered_map>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/serialization/vector.hpp>

using namespace boost::interprocess;
using namespace boost::archive;
using namespace std;
using namespace cv;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

class Robot {
public:
  int sockfd;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  Robot(const char *server_name) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(server_name);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    serv_addr.sin_addr.s_addr = inet_addr(server_name);
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(PORT_NUM);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
  }
  ~Robot() {
    close(sockfd);
  }
};

enum Key {
  null = -1, eforward = 0, ebackward=115, eleft=97, eright=100, inc_speed=101, dec_speed=113
};

enum MouseBut {
    None, RightBut, LeftBut, MidBut, MidButScroll
};

struct Pos {
  int x = -1;
  int y = -1;
};

struct Mouse {
    Pos mMousePos;
    MouseBut mouseBut;
    double MidleButScrollPos = 0.0;
};

class UserInput {
public:
    UserInput();
    ~UserInput() {};
    int aKeys[3];
    Mouse mMouse;
    bool mHasData = false;
    void DisPlayValues();
};

UserInput::UserInput() {
    for(auto i=0;i<3;i++)
        aKeys[i] = -1;
}

class RobotData {
public:
  int x = 3;
};

void UserInput::DisPlayValues() {
    std::cout<<"{{Key:"<<aKeys[0]<<"\t"<<aKeys[1]<<"\t"<<aKeys[2]<<"},{Mouse Pos :("<<mMouse.mMousePos.x<<","<<mMouse.mMousePos.y<<"),MouseBut Clked : "<<mMouse.mouseBut<<",Scroll pos : "<<mMouse.MidleButScrollPos<<"}}"<<std::endl;
//  else if(mKeys.size() == 1) {
//    std::cout<<"{{Key:"<<mKeys[0]<<"},{Mouse Pos :("<<mMouse.mMousePos.x<<","<<mMouse.mMousePos.y<<"),MouseBut Clked : "<<mMouse.mouseBut<<",Scroll pos : "<<mMouse.MidleButScrollPos<<"}}"<<endl;
//  }
//  else if(mKeys.size() == 2) {
//    std::cout<<"{{Key:"<<mKeys[0]<<","<<mKeys[1]<<"},{Mouse Pos :("<<mMouse.mMousePos.x<<","<<mMouse.mMousePos.y<<"),MouseBut Clked : "<<mMouse.mouseBut<<",Scroll pos : "<<mMouse.MidleButScrollPos<<"}}"<<endl;
//  }
//  else if(mKeys.size() == 3) {
//    std::cout<<"{{Key:"<<mKeys[0]<<","<<mKeys[1]<<","<<mKeys[1]<<"},{Mouse Pos :("<<mMouse.mMousePos.x<<","<<mMouse.mMousePos.y<<"),MouseBut Clked : "<<mMouse.mouseBut<<",Scroll pos : "<<mMouse.MidleButScrollPos<<"}}"<<endl;
//  }
}

void SendUserInput(int sockfd,UserInput &data) {
  auto n = write(sockfd,&data,sizeof (data));
  if (n < 0)
       error("ERROR writing to socket");
}

void ReadRobotInfo(int sockfd,RobotData &data) {
  auto n = read(sockfd,&data,sizeof (data));
  if (n < 0) error("ERROR reading from socket");
//  text_iarchive ia{string_stream};
//  ia >> data;
}

void SendRobotInfo(int sockfd,RobotData &data) {
  auto n = write(sockfd,&data,sizeof (data));
  if (n < 0)
       error("ERROR writing to socket");
}

void ReadUserInput(int sockfd,UserInput &data) {
  auto n = read(sockfd,&data,sizeof (data));
  if (n < 0) error("ERROR reading from socket");
//  text_iarchive ia{string_stream};
//  ia >> data;
}

void SendOcvImg(int sockfd,cv::Mat img) {
    int imgSize=img.total()*img.elemSize();
    auto n = send(sockfd, img.data, imgSize, 0);
    if (n < 0) error("ERROR writing img to socket");
}
