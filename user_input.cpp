// This example demostrates the main loop

#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <iostream>
#include <list>
int main(int argc, char ** argv) {
  sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "SFML Demo");

  sf::Event event;

  // If true, you will continue to receive keyboard events when a key is held down
  // If false, it will only fire one event per press until released
  renderWindow.setKeyRepeatEnabled(false);

  std::unordered_map<int, bool> keys;
  std::list<int> changedKeys;
  int count = 0;
  while (renderWindow.isOpen()){
    changedKeys.clear();

    while (renderWindow.pollEvent(event)){
      std::cout<<"in-------------"<<count++<<std::endl;

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
      sf::Vector2i localPosition = sf::Mouse::getPosition(renderWindow);
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        std::cout<<"Left mouse wheel clicked at pos ="<<localPosition.x<<"\t"<<localPosition.y<<std::endl;
      }
      else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        std::cout<<"Right mouse wheel clicked at pos ="<<localPosition.x<<"\t"<<localPosition.y<<std::endl;
      }
      else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        std::cout<<"Middle mouse wheel clicked at pos ="<<localPosition.x<<"\t"<<localPosition.y<<std::endl;
      }
      else if(event.type == sf::Event::MouseWheelScrolled) {
        std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
        std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
      }
      if(event.type == sf::Event::MouseMoved) {
        std::cout<<"mouse pos="<<localPosition.x<<"\t"<<localPosition.y<<std::endl;
      }
    }

    if(!keys.empty()) {
      std::cout << "Currently pressed keys: ";
      // To get the actual value as a string, you need to use Thor or write your own version
      for (auto& keyValue : keys)
        std::cout << keyValue.first << " ";
      std::cout << std::endl;
    }
    if (!changedKeys.empty()){
      std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Keys changed" << std::endl;
    }
    renderWindow.clear();
    renderWindow.display();
  }

}
