#include "iobase.h"
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sstream>


void terminate_by_error(const std::string& title)
{
    std::stringstream ss;
    ss << title << ": " << std::strerror(errno);
    throw std::runtime_error(ss.str());
}

RawOutputScope::RawOutputScope(int desc): desc_(desc)
{
    tcgetattr(desc_, &saved_);
    setup_console_output(desc_, &current_);
}

RawOutputScope::~RawOutputScope()
{
    tcsetattr(desc_,TCSANOW,&saved_);
}

int create_duplex(duplex_t* desc)
{
    int childread_parentwrite[2];
    int parentread_childwrite[2];
    int ret;

    if((ret = pipe(childread_parentwrite)) != 0){
        return ret;
    }
    if((ret = pipe(parentread_childwrite)) != 0){
        close(childread_parentwrite[0]);
        close(childread_parentwrite[1]);
        return ret;
    }
    desc->child.input   = childread_parentwrite[0];
    desc->child.output  = parentread_childwrite[1];
    desc->parent.input  = parentread_childwrite[0];
    desc->parent.output = childread_parentwrite[1];
    return 0;
}

void setup_console_input(int desc)
{
    fcntl(desc, F_SETFL, O_NONBLOCK);       // make the reads non-blocking TODO: error?
}

void setup_console_output(int desc, struct termios* term)
{
    memset(term,0,sizeof(struct termios));
    term->c_iflag=0;
    term->c_oflag=0;
    term->c_cflag=0;
    term->c_lflag&=~ICANON;
    term->c_cc[VMIN]=1;
    term->c_cc[VTIME]=0;
    tcsetattr(desc,TCSANOW,term); // TODO: error?
    tcsetattr(desc,TCSAFLUSH,term); // TODO: error?
}

void send_message(int desc, const std::string& line)
{
    int len = line.length();
    const char *bytes = line.c_str();
    int offset = 0;
    int ret;
    while(offset<len){
        ret = write(desc, bytes+offset, (len-offset));
        if( ret == -1 ){
            if( errno != EAGAIN ){
                throw ChannelClosedError(errno);
            }
        } else {
            offset += ret;
        }
    }
}

void wait_for_input(const int& desc)
{
    fd_set fds, _tempfds;
    FD_ZERO(&fds);
    FD_SET(desc, &fds);

    while(true){
        memcpy(&_tempfds, &fds, sizeof(fd_set)); // TODO: error?
        select(desc+1, &_tempfds, NULL, NULL, NULL);
        if( FD_ISSET(desc, &_tempfds) ){
            break;
        }
    }
}
