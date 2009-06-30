#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "asuro.h"

#include "syd_util.h"

#include "version.h"

#define SIZE_STATUS 80

char* build() {
    return BUILD;
}

void ser_printf(const char *fmt, ...) {
    va_list args;
    unsigned char STATUS[SIZE_STATUS];
    va_start(args, fmt);
    unsigned char s = (unsigned char) vsnprintf(STATUS, SIZE_STATUS, fmt, args);
    va_end(args);
    SerWrite(STATUS, s);
}

void send_data(char *name, char type, unsigned char data_size, unsigned char unit_size, void *data) {
    ser_printf("\nSEND:%s:%d%c:%d\n", name, data_size, type, data_size*unit_size);
    SerWrite((unsigned char*)data, data_size*unit_size);
}

void init_util() {
    ser_printf("\nINIT\nbuild: %s\n", build());
}

// void send_data(char *name, char type, uchar unit_size, char data) { }
