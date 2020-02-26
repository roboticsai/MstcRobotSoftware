/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
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

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/serialization/vector.hpp>

#define FRAME_WIDTH         640
#define FRAME_HEIGHT        480
#define PORT_NUM 2324

using namespace boost::interprocess;
using namespace boost::archive;
using namespace std;
using namespace cv;

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

extern std::unordered_map<int, bool> keys;
extern std::list<int> changedKeys;
class UserInput {
public:
    UserInput();
    ~UserInput() {};
    int aKeys[3];
    Mouse mMouse;
    bool mHasData = false;
    void DisPlayValues();
};

void GetUserInput(sf::RenderWindow &renderWindow,sf::Event event,UserInput &userInput);

class RobotData {
public:
  int x = 3;
  void Display() {
    std::cout<<x<<std::endl;
  }
};

class SfImg {
public:
    SfImg(int _newsockfd) {
        newsockfd = _newsockfd;
    }
    ~SfImg() {}
    int imgSize, bytes = 0, newsockfd;
     Mat img;
     sf::Image image;
     sf::Texture texture;
     sf::Sprite sprite;
     sf::Sprite GetSfImg();
};

void error(const char *msg);

class Controller {
public:

  int sockfd, newsockfd;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
  void error(const char *msg)
  {
      perror(msg);
      exit(1);
  }
  Controller();
  ~Controller();
 };


void SendUserInput(int sockfd,UserInput &data);

void ReadRobotInfo(int sockfd,RobotData &data);

void SendRobotInfo(int sockfd,RobotData &data);

void ReadUserInput(int sockfd,UserInput &data);

sf::Sprite GetSfImg(int newsockfd);
