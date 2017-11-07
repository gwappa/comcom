
#include "arduinodriver.h"

#include <unistd.h>
#include <fcntl.h>
#include <cstring>

ArduinoDriver::ArduinoDriver(std::string path): DeviceDriver()
{
    memset(&tio,0,sizeof(tio)); // TODO: error?
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    reader_desc = open(path.c_str(), O_RDWR | O_NONBLOCK); // TODO: error?
    isopen = true;
    cfsetospeed(&tio,B115200);            // 115200 baud TODO: error?
    cfsetispeed(&tio,B115200);            // 115200 baud TODO: error?

    tcsetattr(reader_desc,TCSANOW,&tio);     // TODO: error?
}

ArduinoDriver::~ArduinoDriver(){
    if(isopen){
        close();
    }
}

void ArduinoDriver::close(){
    ::close(reader_desc);
    isopen = false;
}

void ArduinoDriver::putChar(unsigned char* c){
    ::write(reader_desc,c,1); // TODO: error?
}
