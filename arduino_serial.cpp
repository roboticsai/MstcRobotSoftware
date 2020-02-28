#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <iostream>

enum Key {
  null = -1, eforward = 0, ebackward=115, eleft=97, eright=100, inc_speed=101, dec_speed=113
};

enum MouseBut {
    None, RightBut, LeftBut, MidBut, MidButScroll
};

struct Pos {
  int x = -1;
  int y = -1;
};

struct Mouse {
    Pos mMousePos;
    MouseBut mouseBut;
    int MidleButScrollPos = 0;
};

class UserInput {
public:
    UserInput();
    ~UserInput() {};
    int aKeys[3];
    Mouse mMouse;
    bool mHasData = false;
    void DisPlayValues();
    std::string ToString() {
      return std::to_string(aKeys[0])+std::to_string(aKeys[1])+std::to_string(aKeys[2])
          +std::to_string(mMouse.mouseBut)+std::to_string(mMouse.mMousePos.x)+std::to_string(mMouse.mMousePos.y)+"\n";
    }
};

UserInput::UserInput() {
    for(auto i=0;i<3;i++)
        aKeys[i] = -1;
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
  void Write(const char *data,std::size_t size) {
    int i,ret,flag;
    ret=i=flag=0;
    printf("\nfunction name is  %s >>\n",__func__);
    ret=write(fd,data,size);
  }
};

int main()
{
  SerialComm serial_comm;
  UserInput user_input;
  std::string data = user_input.ToString();
  while(true) {
    std::cout<<"data="<<data<<std::endl;
    std::cout<<"size of data="<<sizeof (data)<<std::endl;
    serial_comm.Write(data.c_str(),sizeof (data));
    sleep(1);
  }
  return 0;
}
