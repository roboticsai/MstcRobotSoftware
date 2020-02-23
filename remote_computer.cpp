#include "socket_comm.h"
int main(int argc, char *argv[])
{
  Controller controller;
  UserInput user_input;

   sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Remote Computer");
   sf::Event event;
   renderWindow.setKeyRepeatEnabled(false);
   sf::Sprite sprite;
   while (renderWindow.isOpen()){
     while (renderWindow.pollEvent(event)){
       GetUserInput(renderWindow,event,user_input);
     }
     user_input.DisPlayValues();
     SendUserInput(controller.newsockfd,user_input);
     renderWindow.clear();
     renderWindow.display();
   }
   return 0;
}
