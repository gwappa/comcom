
#include "iodevice.h"

int Readable::inputDescriptor(){ return reader_desc; }

void Writable::newline(){
    unsigned char nl[] = {'\r', '\n'};
    this->putChar(nl);
    this->putChar(nl+1);
}
