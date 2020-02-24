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

UserInput::UserInput() {
    for(int i=0;i<3;i++)
        aKeys[i] = -1;
}

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
std::unordered_map<int, bool> keys;
std::list<int> changedKeys;
void GetUserInput(sf::RenderWindow &renderWindow,sf::Event event,UserInput &userInput) {
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
  int i = 0;
  for(int i=0;i<3;i++)
      userInput.aKeys[i] = -1;
  for (auto& keyValue : keys) {
      userInput.mHasData = true;
      userInput.aKeys[i++] = keyValue.first;
  }

  sf::Vector2i localPosition = sf::Mouse::getPosition(renderWindow);
  userInput.mMouse.mouseBut = None;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      userInput.mHasData = true;
    userInput.mMouse.mouseBut = LeftBut;
  }
  else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
      userInput.mHasData = true;
    userInput.mMouse.mouseBut = RightBut;
  }
  else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
      userInput.mHasData = true;
    userInput.mMouse.mouseBut = MidBut;
  }
  else if(event.type == sf::Event::MouseWheelScrolled) {
      userInput.mHasData = true;
    userInput.mMouse.MidleButScrollPos+=event.mouseWheelScroll.delta;
  }
  if(event.type == sf::Event::MouseMoved) {
      userInput.mHasData = true;
    userInput.mMouse.mMousePos = {localPosition.x,localPosition.y};
  }
}

sf::Sprite SfImg::GetSfImg() {
             img = Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);

             imgSize = img.total()*img.elemSize();
             uchar sockData[imgSize];

             for(int i=0;i<imgSize;i+=bytes)
               if ((bytes=recv(newsockfd, sockData+i, imgSize-i,0))==-1) std::cout<<("recv failed");

             int ptr=0;

             for(int i=0;i<img.rows;++i)
               for(int j=0;j<img.cols;++j)
               {
                 img.at<Vec3b>(i,j) = Vec3b(sockData[ptr+0],sockData[ptr+1],sockData[ptr+2]);
                 ptr=ptr+3;
               }
             cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);

             image.create(img.cols, img.rows, img.ptr());

             if (!texture.loadFromImage(image))
             {
                 std::cout<<("o no");
             }

             sprite.setTexture(texture);
             return sprite;
}
