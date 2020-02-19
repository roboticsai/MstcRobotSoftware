#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argv, char* args[])
{

     while(true) {
         namedWindow("My Window", 1);
         // Wait until user press some key
        auto key = waitKey(1);
        if(key!=255) cout<<key<<"------------------------------------"<<endl;
     }

     return 0;

}

