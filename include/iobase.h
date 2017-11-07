#ifndef COMCOM_IOBASE_H_
#define COMCOM_IOBASE_H_

#include <termios.h>
#include <string>
#include <exception>

#define raw_output(desc) RawOutputScope _raw__##desc(desc)

// expects action to be evaluated 0, or throws runtime_error and crash
#define expect_or_die(action, msg) if((action)) terminate_by_error((msg));
/**
*   easy crash function
*/
void terminate_by_error(const std::string& title);

class ChannelClosedError: public std::runtime_error
{
public:
    explicit ChannelClosedError(int val): std::runtime_error(std::strerror(val)) {}
};

class EOFError: public std::exception
{ };

typedef struct {
    int input;
    int output;
} iopair_t;

typedef struct {
    iopair_t child;
    iopair_t parent;
} duplex_t;

/**
*   makes the output in 'raw' mode within the scope of its viability
*/
class RawOutputScope
{
public:
    RawOutputScope(int desc);
    ~RawOutputScope();
private:
    struct termios  saved_;
    struct termios  current_;
    int             desc_;
};

/**
*   create two pairs of pipes.
*   returns error code.
*   note that this does not call anything on the file descriptors (e.g. setup_console_input).
*/
int create_duplex(duplex_t* desc);

void setup_console_input(int desc);
void setup_console_output(int desc, struct termios* term); // TODO: add cc[VTIME] configuration

/**
*   sends a chunk of bytes over the descriptor.
*   note that this function does not add '\n' to the end of line.
*/
void send_message(int desc, const std::string& line);

/**
*   waits for single descriptor and returns
*/
void wait_for_input(const int& desc);

#endif
