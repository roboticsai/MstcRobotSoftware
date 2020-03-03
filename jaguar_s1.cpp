#include "jaguar_s1.h"
int main(int argv, char *argc[])
{
  if(argv<3) {
    std::cout<<"Enter port number and hostname"<<std::endl;
    return 0;
  }
    Robot robot(atoi(argc[1]),argc[2]);
    UserInput user_input;
    RobotData robot_data; robot_data.x = 3;
    SerialComm serial_comm;

    cv::VideoCapture cap;
     if(!cap.open(-1))
         return 0;
    cv::Mat img;
    while (true){
      cap >> img;

      SendOcvImg(robot.sockfd,img);

      ReadUserInput(robot.sockfd,user_input);
      SendRobotInfo(robot.sockfd,robot_data);
      //user_input.DisPlayValues();
      serial_comm.Write(user_input.ToString(),10000);
    }
    return 0;
}
