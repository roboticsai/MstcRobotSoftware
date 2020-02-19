#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <iostream>
using namespace std;

enum Key {
  null, w, s, a, d, e, q, wa, wd, sa, sd
};

enum MouseBut {
    None, RightBut, LeftBut, MidBut
};

struct MousePos {
    int x = -1;
    int y = -1;
};

struct Mouse {
    MousePos mMousePos;
    MouseBut mouseBut;
    double MidleButScrollPos = 0.0;
} mouse;

class UserInput {
public:
    UserInput() {};
    ~UserInput() {};
    Key mKey;
    Mouse mMouse;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mKey;
        ar & mMouse;
    }
} userInput;

class RobotData {
public:

};

std::ostream& operator<<(std::ostream &os, const UserInput &userInput) {
    os<<"{{Key:"<<userInput.mKey<<"},{Mouse Pos :("<<userInput.mMouse.mMousePos.x<<","<<userInput.mMouse.mMousePos.y<<"),MouseBut Clked : "<<userInput.mMouse.mouseBut<<",Scroll pos : "<<userInput.mMouse.MidleButScrollPos<<"}}"<<endl;
    return  os;
}
