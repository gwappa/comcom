#ifndef COMCOM_DUMMYDRIVER_H_
#define COMCOM_DUMMYDRIVER_H_

#include <sys/types.h>

#include "iodevice.h"
#include "iobase.h"

/**
*   the driver interface that does not require arduino
*/
class DummyDriver: public DeviceDriver
{
public:
    DummyDriver();
    ~DummyDriver();

    // terminates the dummy interface
    void close();
    // write into its output (facing the frontend)
    void putChar(unsigned char* c);
private:
    iopair_t    io_;
    pid_t       child_pid_;
    bool        isopen_;
};

#endif
