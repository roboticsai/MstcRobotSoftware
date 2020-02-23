// This example demostrates the main loop
#include "socket_comm.h"
int main(int argc, char *argv[]) {
  sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "SFML Demo");

  sf::Event event;
  renderWindow.setKeyRepeatEnabled(false);
  UserInput user_input;
  sf::Sprite sprite;
  while (renderWindow.isOpen()){
    while (renderWindow.pollEvent(event)){
        GetUserInput(renderWindow,event,user_input);
    }
    user_input.DisPlayValues();
    renderWindow.clear();
    renderWindow.display();
  }
  return 0;
}
