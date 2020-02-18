#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if ( event == EVENT_MOUSEMOVE )
     {
          cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
     }
}

int main(int argv, char* args[])
{
    int camera_ind = 0;
    if (argv > 1){
        camera_ind = atoi(args[1]);
    }
    else{
        cout<<"Please give camera index as parameter!"<<endl;
        return 1;
    }
    cv::VideoCapture cap;
    if(!cap.open(camera_ind))
        return 0;
     // Read image from file
     Mat img;

     while(true) {
         cap >> img;
         //if fail to read the image
         if ( img.empty() )
         {
              cout << "Error loading the image" << endl;
              return -1;
         }

         //Create a window
         namedWindow("My Window", 1);

         //set the callback function for any mouse event
         setMouseCallback("My Window", CallBackFunc, NULL);

         //show the image
         imshow("My Window", img);
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

