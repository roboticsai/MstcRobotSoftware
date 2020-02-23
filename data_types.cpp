#include "common_header.h"
UserInput::UserInput() {
  std::cout<<"construtor called"<<std::endl;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
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

void UserInput::GetUserInput(sf::RenderWindow &renderWindow,sf::Event event) {
  if (event.type == sf::Event::EventType::Closed)
    renderWindow.close();

  if (event.type == sf::Event::EventType::KeyPressed){
    if (keys.count(event.key.code) == 0){
      keys[event.key.code] = true;
      changedKeys.push_back(event.key.code);
    }
  }
  if (event.type == sf::Event::EventType::KeyReleased){
    if (keys.count(event.key.code) == 1){
      keys.erase(event.key.code);
      changedKeys.push_back(event.key.code);
    }
  }
  mKeys.clear();
  for (auto& keyValue : keys)
    mKeys.push_back(keyValue.first);

  sf::Vector2i localPosition = sf::Mouse::getPosition(renderWindow);
  mMouse.mouseBut = None;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    mMouse.mouseBut = LeftBut;
  }
  else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
    mMouse.mouseBut = RightBut;
  }
  else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
    mMouse.mouseBut = MidBut;
  }
  else if(event.type == sf::Event::MouseWheelScrolled) {
    mMouse.MidleButScrollPos+=event.mouseWheelScroll.delta;
  }
  if(event.type == sf::Event::MouseMoved) {
    mMouse.mMousePos = localPosition;
  }
}


Robot::Robot(char *server_name) {
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

Robot::~Robot() {
  close(sockfd);
}

void Robot::WriteSocket(void *data) {
  n = write(sockfd,&data,sizeof (data));
    if (n < 0)
         error("ERROR writing to socket");
}
void Robot::ReadSocket(void *data) {
  n = read(sockfd,&data,sizeof (data));
  if (n < 0)
       error("ERROR reading from socket");
}
