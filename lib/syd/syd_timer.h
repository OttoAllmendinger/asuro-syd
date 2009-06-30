#pragma once

void syd_timer_init();
void syd_timer_mainloop();
void syd_timer_add_callback(void (*fct)(void), int ms, int counter);
void syd_timer_dump();
