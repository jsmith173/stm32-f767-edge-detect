#ifndef appH
#define appH

#include <stdint.h>

void edge_detect_init();
void edge_detect_done();
void edge_detect(uint8_t* src);

extern uint8_t* grad;

#endif

