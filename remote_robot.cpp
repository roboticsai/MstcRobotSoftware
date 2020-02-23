#include "robot.h"
int main(int argc, char *argv[])
{
    Robot robot(argv[1]);
    UserInput user_input;
    RobotData robot_data; robot_data.x = 3;

    cv::VideoCapture cap;
     if(!cap.open(-1))
         return 0;
    cv::Mat img;
    while (true){
      cap >> img;
      if ( img.empty() )
      {
           cout << "Error loading the image" << endl;
           return -1;
      }

      auto imgSize=img.total()*img.elemSize();

      auto n = send(robot.sockfd, img.data, imgSize, 0);
      if (n < 0) error("ERROR writing img to socket");

      ReadUserInput(robot.sockfd,user_input);
      SendRobotInfo(robot.sockfd,robot_data);
      user_input.DisPlayValues();
    }
    return 0;
}
