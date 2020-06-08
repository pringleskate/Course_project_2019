#ifndef _KEYSTROKE_H_
#define _KEYSTROKE_H_ 

#define ENTER_RELEASED 0x9C

unsigned char inb_port(unsigned int port);

void outb_port(unsigned int port, char val);

void catch_enter(void);

#endif