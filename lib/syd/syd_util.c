#include "asuro.h"

#include "syd.h"
#include "syd_util.h"


// TODO: reintegrate ser_printf? 

void send(uchar id, uchar* data, char type, uchar size) {
    char HEADER[] = {'\n', 's', 'e', 'n', 'd', id, type, size };
    SerWrite(HEADER, 8);
    SerWrite(data, ((type & (0xff-SIGNED)) * size));
}
