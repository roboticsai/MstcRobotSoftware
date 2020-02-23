#include "socket_comm.h"
#define FRAME_WIDTH         640
#define FRAME_HEIGHT        480

int main(int argc, char *argv[])
{
  Controller controller;
  UserInput user_input;
  RobotData robot_data;

   sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Remote Computer");
   sf::Event event;
   renderWindow.setKeyRepeatEnabled(false);

   int IM_HEIGHT, IM_WIDTH, imgSize, bytes = 0;
    Mat img;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

   while (renderWindow.isOpen()){
       IM_HEIGHT = FRAME_HEIGHT;
         IM_WIDTH = FRAME_WIDTH;
         img = Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);

         imgSize = img.total()*img.elemSize();
         uchar sockData[imgSize];

         for(int i=0;i<imgSize;i+=bytes)
           if ((bytes=recv(controller.newsockfd, sockData+i, imgSize-i,0))==-1) error("recv failed");

         int ptr=0;

         for(int i=0;i<img.rows;++i)
           for(int j=0;j<img.cols;++j)
           {
             img.at<Vec3b>(i,j) = Vec3b(sockData[ptr+0],sockData[ptr+1],sockData[ptr+2]);
             ptr=ptr+3;
           }
         cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);

         image.create(img.cols, img.rows, img.ptr());

         if (!texture.loadFromImage(image))
         {
             break;
         }

         sprite.setTexture(texture);

         while (renderWindow.pollEvent(event)){
             GetUserInput(renderWindow,event,user_input);
         }
     SendUserInput(controller.newsockfd,user_input);
     ReadRobotInfo(controller.newsockfd,robot_data);
     robot_data.Display();
     renderWindow.draw(sprite);
     renderWindow.display();
   }
   return 0;
}
