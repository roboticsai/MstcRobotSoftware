#include "socket_comm.h"
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

Controller::Controller() {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
     error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT_NUM);
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
}

Controller::~Controller() {
  std::cout<<"Destructor called"<<std::endl;
  close(newsockfd);
  close(sockfd);
};

void SendUserInput(int sockfd,UserInput &data) {
  text_oarchive oa{string_stream};
  oa << data;
  auto n = write(sockfd,&oa,sizeof (oa));
  if (n < 0)
       error("ERROR writing to socket");
}

void ReadRobotInfo(int sockfd,RobotData &data) {
  auto n = read(sockfd,&string_stream,sizeof (string_stream));
  if (n < 0) error("ERROR reading from socket");
//  text_iarchive ia{string_stream};
//  ia >> data;
}

void SendRobotInfo(int sockfd,RobotData &data) {
  text_oarchive oa{string_stream};
  oa << data;
  auto n = write(sockfd,&oa,sizeof (oa));
  if (n < 0)
       error("ERROR writing to socket");
}

void ReadUserInput(int sockfd,UserInput &data) {
  auto n = read(sockfd,&string_stream,sizeof (string_stream));
  if (n < 0) error("ERROR reading from socket");
//  text_iarchive ia{string_stream};
//  ia >> data;
}

UserInput::UserInput() {
  std::cout<<"construtor called"<<std::endl;
}

void UserInput::DisPlayValues() {
  if(mKeys.empty())
    std::cout<<"{{Key:"<<null<<"},{Mouse Pos :("<<mMouse.mMousePos.x<<","<<mMouse.mMousePos.y<<"),MouseBut Clked : "<<mMouse.mouseBut<<",Scroll pos : "<<mMouse.MidleButScrollPos<<"}}"<<std::endl;
  else if(mKeys.size() == 1) {
    std::cout<<"{{Key:"<<mKeys[0]<<"},{Mouse Pos :("<<mMouse.mMousePos.x<<","<<mMouse.mMousePos.y<<"),MouseBut Clked : "<<mMouse.mouseBut<<",Scroll pos : "<<mMouse.MidleButScrollPos<<"}}"<<endl;
  }
  else if(mKeys.size() == 2) {
    std::cout<<"{{Key:"<<mKeys[0]<<","<<mKeys[1]<<"},{Mouse Pos :("<<mMouse.mMousePos.x<<","<<mMouse.mMousePos.y<<"),MouseBut Clked : "<<mMouse.mouseBut<<",Scroll pos : "<<mMouse.MidleButScrollPos<<"}}"<<endl;
  }
  else if(mKeys.size() == 3) {
    std::cout<<"{{Key:"<<mKeys[0]<<","<<mKeys[1]<<","<<mKeys[1]<<"},{Mouse Pos :("<<mMouse.mMousePos.x<<","<<mMouse.mMousePos.y<<"),MouseBut Clked : "<<mMouse.mouseBut<<",Scroll pos : "<<mMouse.MidleButScrollPos<<"}}"<<endl;
  }
}

void GetUserInput(sf::RenderWindow &renderWindow,sf::Event event,UserInput &userInput) {
  if (event.type == sf::Event::EventType::Closed)
    renderWindow.close();

  if (event.type == sf::Event::EventType::KeyPressed){
    if (userInput.keys.count(event.key.code) == 0){
      userInput.keys[event.key.code] = true;
      userInput.changedKeys.push_back(event.key.code);
    }
  }
  if (event.type == sf::Event::EventType::KeyReleased){
    if (userInput.keys.count(event.key.code) == 1){
      userInput.keys.erase(event.key.code);
      userInput.changedKeys.push_back(event.key.code);
    }
  }
  userInput.mKeys.clear();
  for (auto& keyValue : userInput.keys)
    userInput.mKeys.push_back(keyValue.first);

  sf::Vector2i localPosition = sf::Mouse::getPosition(renderWindow);
  userInput.mMouse.mouseBut = None;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    userInput.mMouse.mouseBut = LeftBut;
  }
  else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
    userInput.mMouse.mouseBut = RightBut;
  }
  else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
    userInput.mMouse.mouseBut = MidBut;
  }
  else if(event.type == sf::Event::MouseWheelScrolled) {
    userInput.mMouse.MidleButScrollPos+=event.mouseWheelScroll.delta;
  }
  if(event.type == sf::Event::MouseMoved) {
    userInput.mMouse.mMousePos = {localPosition.x,localPosition.y};
  }
}
