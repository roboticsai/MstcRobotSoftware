#include "jaguar_s1.h"
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

      SendOcvImg(robot.sockfd,img);

      ReadUserInput(robot.sockfd,user_input);
      SendRobotInfo(robot.sockfd,robot_data);
      user_input.DisPlayValues();
    }
    return 0;
}
