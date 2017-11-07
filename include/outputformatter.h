#ifndef COMCOM_OFORMATTER_H_
#define COMCOM_OFORMATTER_H_

#include <cstdint>
#include <unistd.h>
#include "iodevice.h"

class OutputFormatter: public Writable
{
public:
    OutputFormatter(int out_fd): _out_desc(out_fd), _count(0) {}
    ~OutputFormatter() {};

    void        putChar(unsigned char* c) { ::write(_out_desc,c,1); _count++; } // TODO: error?
    uint64_t    count() { return _count; }
private:
    int         _out_desc;
    uint64_t    _count;
};

#endif
