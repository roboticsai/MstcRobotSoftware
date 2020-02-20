// This example demostrates the main loop

#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <iostream>
#include <list>
#include "common_header.h"

int main(int argc, char ** argv) {
  sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "SFML Demo");

  sf::Event event;

  // If true, you will continue to receive keyboard events when a key is held down
  // If false, it will only fire one event per press until released
  renderWindow.setKeyRepeatEnabled(false);
  UserInput user_input;
  while (renderWindow.isOpen()){
    while (renderWindow.pollEvent(event)){
      user_input.GetUserInput(renderWindow,event);
      user_input.DisPlayValues();
    }
    renderWindow.clear();
    renderWindow.display();
  }

}
