/*
*   comocom -- main.cpp
*   @author K Sehara
*/
#include <iostream>
#include "session.h"
#include "dummydriver.h"
#include "arduinodriver.h"

int main(int argc, char** argv)
{
    if( argc < 2 ){
        std::cerr << "--------" << std::endl;
        std::cerr << "for connecting to Arduino, please start with:" << std::endl;
        std::cerr << "  " << argv[0] << " /dev/tty**" << std::endl;
        std::cerr << "--------" << std::endl;
        std::cerr << "***dummy device is used during this session." << std::endl;
        // device = new DummyDriver();
        DummyDriver device;
        return run_session(&device);
    } else {
        ArduinoDriver device(argv[1]);
        return run_session(&device);
    }
}
