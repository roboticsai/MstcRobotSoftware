#include "socket_comm.h"
int main(int argc, char *argv[])
{
  Controller controller;
  RobotData robotPos;
  UserInput mousePos;

   sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Remote Computer");
   sf::Event event;
   renderWindow.setKeyRepeatEnabled(false);
   UserInput user_input;
   sf::Sprite sprite;
   while (renderWindow.isOpen()){
     ReadRobotInfo(controller.newsockfd,robotPos);
     SendUserInput(controller.newsockfd,mousePos);
     while (renderWindow.pollEvent(event)){
       GetUserInput(renderWindow,event,user_input);
     }
     user_input.DisPlayValues();
     renderWindow.clear();
     renderWindow.display();
   }
   return 0;
}
