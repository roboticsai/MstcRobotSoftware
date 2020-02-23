#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <iostream>
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <iostream>
#include <list>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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
#include <unordered_map>
#include <list>
#define PORT_NUM 2323

using namespace std;
using namespace cv;

enum Key {
  null = -1, eforward = 0, ebackward=115, eleft=97, eright=100, inc_speed=101, dec_speed=113
};

enum MouseBut {
    None, RightBut, LeftBut, MidBut, MidButScroll
};

struct Mouse {
    sf::Vector2i mMousePos;
    MouseBut mouseBut;
    double MidleButScrollPos = 0.0;
};

class UserInput {
public:
    UserInput();
    ~UserInput() {};
    void GetUserInput(sf::RenderWindow &renderWindow,sf::Event event);
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

class Controller {
public:
  int sockfd, newsockfd;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  Controller();
  ~Controller();
  void WriteSocket(void *data);
  void ReadSocket(void *data);
 };

class Robot {
public:
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];
  Robot(char *server_name);
  ~Robot();
  void WriteSocket(void *data);
  void ReadSocket(void *data);

};

class RobotData {
public:

};
