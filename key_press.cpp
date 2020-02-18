#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argv, char* args[])
{
     while(true) {
         // Wait until user press some key
        auto key = waitKey(1);
        switch (key) {
        case 119: cout<<"move forward"<<endl; break;
        case 100: cout<<"move right"<<endl; break;
        case 97:cout<<"move left"<<endl; break;
        case 115:cout<<"move backward"<<endl; break;
        case 114:cout<<"decrease speed"<<endl; break;
        case 101: cout<<"increase speed"<<endl; break;
        }
     }

     return 0;

}

