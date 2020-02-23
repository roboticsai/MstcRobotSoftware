#include "robot.h"
int main(int argc, char *argv[])
{
    Robot robot(argv[1]);
    UserInput user_input;
    while (true){
      ReadUserInput(robot.sockfd,user_input);
      user_input.DisPlayValues();
    }
    return 0;
}
