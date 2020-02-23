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

#define PORT_NUM 2323

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
std::stringstream string_stream;

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

class UserInput {
public:
    UserInput();
    ~UserInput() {};
    std::vector<int> mKeys;
    Mouse mMouse;
    std::unordered_map<int, bool> keys;
    std::list<int> changedKeys;
    void DisPlayValues();
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mKeys;
        ar & mMouse;
    }
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
    bzero((char *) &serv_addr, sizeof(serv_addr));
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

void SendUserInput(int sockfd,UserInput &data);

void ReadRobotInfo(int sockfd,RobotData &data);

void SendRobotInfo(int sockfd,RobotData &data);

void ReadUserInput(int sockfd,UserInput &data);
