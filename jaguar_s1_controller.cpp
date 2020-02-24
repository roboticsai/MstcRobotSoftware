#include "socket_comm.h"

int main()
{
  Controller controller;
  UserInput user_input;
  RobotData robot_data;
  SfImg sfImg(controller.newsockfd);

   sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Remote Computer");
   sf::Event event;
   renderWindow.setKeyRepeatEnabled(false);

   while (renderWindow.isOpen()){
     auto sprite = sfImg.GetSfImg();
     while (renderWindow.pollEvent(event)){
         GetUserInput(renderWindow,event,user_input);
     }
     SendUserInput(controller.newsockfd,user_input);
     ReadRobotInfo(controller.newsockfd,robot_data);
     robot_data.Display();
     renderWindow.draw(sprite);
     renderWindow.display();
   }
   return 0;
}
