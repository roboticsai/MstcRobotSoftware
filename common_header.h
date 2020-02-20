#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <iostream>
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <iostream>
#include <list>
using namespace std;

enum Key {
  null = -1, eforward = 0, ebackward=115, eleft=97, eright=100, inc_speed=101, dec_speed=113
};

enum MouseBut {
    None, RightBut, LeftBut, MidBut, MidButScroll
};

struct Mouse {
    sf::Vector2i mMousePos;
    MouseBut mouseBut;
    double MidleButScrollPos = 0.0;
} mouse;

class UserInput {
public:
    UserInput();
    ~UserInput() {};
    void GetUserInput(sf::RenderWindow &renderWindow,sf::Event event);
    std::vector<int> mKeys;
    Mouse mMouse;
    std::unordered_map<int, bool> keys;
    std::list<int> changedKeys;
    void DisPlayValues();
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mKeys;
        ar & mMouse;
    }
};

class RobotData {
public:

};

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

UserInput::UserInput() {
  std::cout<<"construtor called"<<std::endl;
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
