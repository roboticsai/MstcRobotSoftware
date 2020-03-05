
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iomanip>
#include <iostream>
#include <cstdlib>


////////////////////////////////////////////////////////////
// Function prototypes
// (I'm too lazy to put them into separate headers...)
////////////////////////////////////////////////////////////
void doClient(unsigned short port,char *server_name);
void doServer(unsigned short port);


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main(int argv,char *argc[])
{
    // Choose a random port for opening sockets (ports < 1024 are reserved)

    if(argv==1) {
        std::cout<<"Enter server name"<<std::endl;
        return 0;
    }
    doClient(2435,argc[1]);
    std::cout<<"end--------------"<<std::endl;

    return EXIT_SUCCESS;
}
