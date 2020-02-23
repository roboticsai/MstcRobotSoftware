#include "socket_comm.h"
int main(int argc, char *argv[])
{
    UserInput userInput;
    RobotData rootData;
    Robot robot(argv[1]);
    sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Remote Robot");
    UserInput user_input;
    while (true){
      SendRobotInfo(robot.sockfd,rootData);
      ReadUserInput(robot.sockfd,userInput);
    }
    return 0;
}
