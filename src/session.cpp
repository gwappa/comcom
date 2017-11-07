#include "session.h"
#include "iobase.h"
#include "outputformatter.h"

#include <iostream>

#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>
#include <sys/select.h>


inline bool pass_char(unsigned char* charbuf, int input, Writable* output)
{
    if ((read(input,charbuf,1)>0) && (charbuf>0)) {
        output->putChar(charbuf);
        return true;
    } else {
        return false;
    }
}

int run_session(RWPort* device)
{
    raw_output(STDOUT_FILENO);
    OutputFormatter out(STDOUT_FILENO);
    setup_console_input(STDIN_FILENO);

    int devin_fd = device->inputDescriptor();
    int stdin_fd  = STDIN_FILENO;

    // initialize and set FD_SET
    fd_set fds, _tempfds; // _tempfds to be used during each loop
    FD_ZERO(&fds);
    FD_SET(stdin_fd, &fds);
    FD_SET(devin_fd, &fds);
    int max_fd = (stdin_fd > devin_fd)? stdin_fd : devin_fd;

    unsigned char c='I';
    bool done_writing = false;
    uint64_t nwritten = 0;
    while ((!done_writing) || (out.count() <= nwritten))
    {
        // initialize _tempfds
        memcpy(&_tempfds, &fds, sizeof(fd_set)); // TODO: error?

        select(max_fd+1, &_tempfds, NULL, NULL, NULL);

        if (FD_ISSET(devin_fd, &_tempfds)){
            if(!pass_char(&c, devin_fd, &out)){ // serial -> STDOUT
                std::cerr << "***session: device seems to be closed.\r\n";
                break;
            }
        }
        if (FD_ISSET(stdin_fd, &_tempfds) && !done_writing){
            if( pass_char(&c, stdin_fd, device)) { // STDIN -> serial
                if( c == 'Q' ){
                    done_writing = true;
                } else {
                    nwritten++;
                }
            } else {
                std::cerr << "***session: console seems to be closed.\r\n";
                break;
            }
        }
    }
    std::cout << std::endl;
    return 0;
}
