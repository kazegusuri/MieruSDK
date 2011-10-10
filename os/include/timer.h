
#pragma once

void disable_global_interupt();
void enable_global_interupt();
void init_timer();
extern "C" void timer_exception();
