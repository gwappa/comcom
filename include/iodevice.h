#ifndef COMCOM_IOADAPTER_H_
#define COMCOM_IOADAPTER_H_

class Readable
{
public:
    Readable(int desc=-1): reader_desc(desc) {}
    // returns raw file descriptor for input
    int inputDescriptor();

protected:
    int reader_desc;
};

class Writable
{
public:
    virtual void putChar(unsigned char* c)=0;
    void newline();
};

class RWPort: public Readable, public Writable
{
public:
    RWPort(int desc=-1): Readable(desc), Writable() {}
    virtual ~RWPort() {}
};

/**
*   abstract marker class for driver implementation
*/
class DeviceDriver: public RWPort
{
public:
    explicit DeviceDriver(int desc=-1): RWPort(desc) { }
    virtual ~DeviceDriver() {}
    // terminates the interface
    virtual void close()=0;
};

#endif
