// This example demostrates the main loop

#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <iostream>
#include <list>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
double mouseWheelPos = 0.0;
int main(int argc, char ** argv) {
  sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "SFML Demo");

  sf::Event event;
  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;

  // If true, you will continue to receive keyboard events when a key is held down
  // If false, it will only fire one event per press until released
  renderWindow.setKeyRepeatEnabled(true);

  std::unordered_map<int, bool> keys;
  std::list<int> changedKeys;

  cv::VideoCapture cap;
  if(!cap.open(-1))
      return 0;
   // Read image from file
  cv::Mat img;
  cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT,460);
  while (renderWindow.isOpen()){
    changedKeys.clear();
    cap >> img;
    cv::cvtColor(img,img,cv::COLOR_BGR2RGBA);
    image.create(img.cols, img.rows, img.ptr());

    if (!texture.loadFromImage(image))
    {
        break;
    }

    sprite.setTexture(texture);

    while (renderWindow.pollEvent(event)){
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
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        std::cout<<"Left mouse wheel clicked at pos ="<<event.mouseMove.x<<"\t"<<event.mouseMove.y<<std::endl;
      }
      else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        std::cout<<"Right mouse wheel clicked at pos ="<<event.mouseMove.x<<"\t"<<event.mouseMove.y<<std::endl;
      }
      else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        std::cout<<"Middle mouse wheel clicked at pos ="<<event.mouseMove.x<<"\t"<<event.mouseMove.y<<std::endl;
      }
      else if(event.type == sf::Event::MouseWheelScrolled) {
        mouseWheelPos+=event.mouseWheelScroll.delta;
        std::cout << "wheel movement: " << mouseWheelPos << std::endl;
        std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
        std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
      }
      if(event.type == sf::Event::MouseMoved) {
        std::cout<<"mouse pos="<<event.mouseMove.x<<"\t"<<event.mouseMove.y<<endl;
      }
    }


    // To get the actual value as a string, you need to use Thor or write your own version
    for (auto& keyValue : keys)
      std::cout<<"Key=" << keyValue.first << " ";

    if (!changedKeys.empty()){
      std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Keys changed" << std::endl;
    }
    renderWindow.clear();
    renderWindow.draw(sprite);
    renderWindow.display();
  }

}
