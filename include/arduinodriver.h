#ifndef COMCOM_ARDUINODRIVER_H_
#define COMCOM_ARDUINODRIVER_H_

#include <termios.h>
#include <string>
#include "iodevice.h"

class ArduinoDriver: public DeviceDriver
{
public:
    // initializes with a serial port
    ArduinoDriver(std::string path);
    // closes the serial port if still open
    ~ArduinoDriver();
    // closes the serial port
    void close();

    void putChar(unsigned char* c);

private:
    struct termios tio;
    bool isopen;
};

#endif
