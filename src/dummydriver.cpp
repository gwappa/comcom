#include "dummydriver.h"

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <iostream>

/**
*   the private subprocess function for the dummy interface.
*   @param info     I/O file handles
*/
void run_dummy_device(iopair_t* io);


DummyDriver::DummyDriver(): DeviceDriver(), isopen_(false)
{
    duplex_t duplex;

    expect_or_die(create_duplex(&duplex), "DummyDriver: failed to create pipes for the device subprocess");

    if((child_pid_ = fork()) == 0){
        ::close(duplex.parent.input);
        ::close(duplex.parent.output);
        run_dummy_device(&(duplex.child));
    } else if (child_pid_ > 0) {
        isopen_ = true;
        ::close(duplex.child.input);
        ::close(duplex.child.output);
        io_.input = duplex.parent.input;
        io_.output = duplex.parent.output;
        struct termios term;
        setup_console_input(io_.input);
        setup_console_output(io_.output, &term);
        reader_desc = io_.input;
    } else {
        terminate_by_error("DummyDriver: could not start the device subprocess.");
    }
}

DummyDriver::~DummyDriver(){
    close();
}

void DummyDriver::close(){
    if(isopen_){
        ::close(io_.input);
        ::close(io_.output);
        if (kill(child_pid_, SIGKILL)) {
            switch(errno){
                case ESRCH:
                    break;
                default:
                    terminate_by_error("DummyDriver: failed to join the device subprocess");
            }
        }
        isopen_ = false;
    }
}

void DummyDriver::putChar(unsigned char* c){
    ::write(io_.output,c,1); // TODO: error?
}

void run_dummy_device(iopair_t* io)
{
    int input = io->input;
    int output = io->output;

    ::close(STDIN_FILENO);
    dup(input);
    ::close(input);

    ::close(STDOUT_FILENO);
    dup(output);
    ::close(output);

    char* argv[] = { 0 };
    char* env[]  = { 0 };
    execve("./dummydevice", argv, env);
    terminate_by_error("failed to launch dummy device.");
}
