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
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
      ar & x;
      ar & y;
  }
};

struct Mouse {
    Pos mMousePos;
    MouseBut mouseBut;
    double MidleButScrollPos = 0.0;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mMousePos;
        ar & mouseBut;
        ar & MidleButScrollPos;
    }
};

extern std::unordered_map<int, bool> keys;
extern std::list<int> changedKeys;
class UserInput {
public:
    UserInput();
    ~UserInput() {};
    int aKeys[3];
    Mouse mMouse;
    void DisPlayValues();
//    friend class boost::serialization::access;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        ar & mMouse;
//    }
};

void GetUserInput(sf::RenderWindow &renderWindow,sf::Event event,UserInput &userInput);

class RobotData {
public:
  int x = 3;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
      ar & x;
  }
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
