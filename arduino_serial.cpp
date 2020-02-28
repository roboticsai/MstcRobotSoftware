#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>

enum Key {
  null = -1, eforward = 0, ebackward=115, eleft=97, eright=100, inc_speed=101, dec_speed=113
};

enum MouseBut {
    None = -1, RightBut = 0, LeftBut = 1, MidBut = 2, MidButScroll = 3
};

struct Pos {
  int x = 1;
  int y = 1;
};

struct Mouse {
    Pos mMousePos;
    MouseBut mouseBut;
    int MidleButScrollPos = 0;
};

std::string to_format(const int number) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << number;
    return ss.str();
}

class UserInput {
public:
    UserInput();
    ~UserInput() {};
    int aKeys[3];
    Mouse mMouse;
    bool mHasData = false;
    void DisPlayValues();
    std::string ToString() {
      return to_format(aKeys[0])+to_format(aKeys[1])+to_format(aKeys[2])
          +to_format(mMouse.mouseBut)+to_format(mMouse.mMousePos.x)+to_format(mMouse.mMousePos.y)+"\n";
    }
};

UserInput::UserInput() {
    for(auto i=0;i<3;i++)
        aKeys[i] = 0;
}

class SerialComm {
public:
  int fd,ret;
  char red[88];
  SerialComm() {
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    struct termios SerialPortSettings;  // Create the structure
    tcgetattr(fd, &SerialPortSettings);
    cfsetispeed(&SerialPortSettings,B2000000); // Set Read  Speed as 115200
    cfsetospeed(&SerialPortSettings,B2000000);
    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) // Set the attributes to the termios structure
      printf("Error while setting attributes \n");
      printf("\nfunction name is  %s >>\n",__func__);
      printf("reading from Serial port== %c >>\n",red);
      printf("returned fd is :%d\n",fd );
      if (fd == -1)
      {
        perror("open_port: Unable to open /dev/ttyUSB0 - ");
      }
  };
  ~SerialComm() {};
  void Write(std::string data,std::size_t del_time) {
    for(int i=0;i<data.length();i++) {
        ret=write(fd,&data[i],1);
    }
    std::this_thread::sleep_for (std::chrono::microseconds (del_time));
  }
};

int main(int argv,char *argc[])
{
  std::size_t del_time = 1000;
  if(argv==1) {
    std::cout<<"Enter delay time in micro seconds(1/1000000)sec"<<std::endl;
  }
  del_time = atoi(argc[1]);
  SerialComm serial_comm;
  UserInput user_input;
  while(true) {
    serial_comm.Write(user_input.ToString(),del_time);
  }
  return 0;
}
