#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "asuro.h"

#include "syd.h"
#include "syd_util.h"

#include "version.h"

#define SIZE_STATUS 80

void ser_printf(const char *fmt, ...) {
    va_list args;
    uchar STATUS[SIZE_STATUS];
    va_start(args, fmt);
    uchar s = (uchar) vsnprintf(STATUS, SIZE_STATUS, fmt, args);
    va_end(args);
    SerWrite(STATUS, s);
}

void syd_message(char* name) {
    int battery = Battery();
    ser_printf("\nSTART\nProject: %s\n", name);
    ser_printf("Battery Power: %d\n", battery);

}


void syd_send_data(char *name, char type, uchar data_size, uchar unit_size, uchar* data) {
    ser_printf("\nSEND:%s:%d%c:%d\n", name, data_size, type, data_size*unit_size);
    SerWrite(data, data_size*unit_size);
}


void send(uchar id, uchar* data, char type, uchar size) {
    char HEADER[] = {'\n', 's', 'e', 'n', 'd', id, type, size };
    SerWrite(HEADER, 8);
    SerWrite(data, ((type & (0xff-SIGNED)) * size));
}
