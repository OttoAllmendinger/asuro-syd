#pragma once

#include "syd.h"

#define SIGNED 0x80

#define S1 (SIGNED | 1)
#define S2 (SIGNED | 2)
#define S4 (SIGNED | 4)

#define U1 1
#define U2 2
#define U4 4

void ser_printf(const char *fmt, ...);
void syd_message(char* name);
void send(uchar id, uchar* data, char type, uchar size);
