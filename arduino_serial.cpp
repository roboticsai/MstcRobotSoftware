#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <iostream>
void reading(int fd)
{
  char red[4];
  int i,ret,flag;
  ret=i=flag=0;
  printf("\nfunction name is  %s >>\n",__func__);
  while(1)
  {
    ret=read(fd,&red,4);
    //printf("\nReading data.... >>>>>%s<< \n",red);
    std::cout<<"data="<<red<<std::endl;
    sleep(1);
  }
}
int open_port(void)
{
  int fd,ret;
  char red[88];
  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
struct termios SerialPortSettings;  // Create the structure
tcgetattr(fd, &SerialPortSettings);
cfsetispeed(&SerialPortSettings,B9600); // Set Read  Speed as 115200
cfsetospeed(&SerialPortSettings,B9600);
if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) // Set the attributes to the termios structure
  printf("Error while setting attributes \n");
  printf("\nfunction name is  %s >>\n",__func__);
  printf("reading from Serial port== %c >>\n",red);
  printf("returned fd is :%d\n",fd );
  if (fd == -1)
  {
    perror("open_port: Unable to open /dev/ttyUSB0 - ");
  }
    return (fd);
}
int main()
{
  int k = open_port();
  reading(k);
  return 0;
}
