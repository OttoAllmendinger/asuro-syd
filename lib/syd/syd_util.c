#ifdef SERPRINT
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#endif

#include "asuro.h"

#include "syd.h"
#include "syd_util.h"


// TODO: reintegrate ser_printf? 

#ifdef SERPRINT
void ser_printf(const char *fmt, ...) {
    va_list args;
    uchar STATUS[SIZE_STATUS];
    va_start(args, fmt);
    uchar s = (uchar) vsnprintf(STATUS, SIZE_STATUS, fmt, args);
    va_end(args);
    SerWrite(STATUS, s);
}
#else
void ser_printf(const char *fmt, ...) {
    // do nuthing
};
#endif

void send(uchar id, uchar* data, char type, uchar size) {
    char HEADER[] = {'\n', 's', 'e', 'n', 'd', id, type, size };
    SerWrite(HEADER, 8);
    SerWrite(data, ((type & (0xff-SIGNED)) * size));
}
