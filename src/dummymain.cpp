#include <iostream>
#include <unistd.h>

#include "iobase.h"

int main()
{
    int input   = STDIN_FILENO;
    int output  = STDOUT_FILENO;

    raw_output(output);
    setup_console_input(input);

    sleep(2);
    send_message(output, "--Arduino ready.\n");

    int offset = 0;
    const char flag_laser = 0x02;
    const char flag_sync  = 0x01;

    const char laser_on   = 'A';
    const char laser_off  = 'B';
    const char sync_on    = '1';
    const char sync_off   = '2';
    const char flush      = 'F';

    const char quit       = 'Q'; // auxiliary

    char state = 0x00; // the current state
    char msg = 0x00; // the state to be sent to the output

    char buf;
    int status;
    bool more = true;

    while(more){
        wait_for_input(input);

        switch((status = ::read(input,&buf,1))) {
            case 0:
                // EOF??
                more = false;
                continue;
            case 1:
                // OK
                break;
            case -1:
            default:
                std::cerr << "***error " << status << std::endl;
                more = false;
                continue;
        }

        switch(buf){
        case flush:
            if( offset > 0 ){
                for(; offset<4; offset++){
                    msg = (msg << 2) | state;
                }
                ::write(output, &msg, 1);
                offset = 0;
            }
            continue;
        case laser_on:
            state |= flag_laser;
            break;
        case laser_off:
            state &= ~flag_laser;
            break;
        case sync_on:
            state |= flag_sync;
            break;
        case sync_off:
            state &= ~flag_sync;
            break;
        case quit:
            more = false;
            state &= 0x00;
            break;
        default:
            // do nothing
            break;
        }

        msg = (msg << 2) | state;
        if( ++offset == 4 ){
            ::write(output, &msg, 1);
            offset = 0;
        }
    }
    return 0;
}
