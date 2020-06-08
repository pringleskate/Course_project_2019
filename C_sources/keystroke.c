#include "keystroke.h"

/*Считывание байта информации из порта.*/
unsigned char inb_port(unsigned int port) {
   unsigned char ret;
   asm ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}

/*Кладем байт информации в порт.*/
void outb_port(unsigned int port, char val) {
    asm ( "outb %%al, %%dx" : : "a"(val), "d"(port));
}

/*Отлавливаем нажатие клавиши Enter.*/
void catch_enter(void) {
    unsigned int status_register_port = 0x64;
    unsigned int data_port = 0x60;
    
    while (1) {
        unsigned char status_return_value = inb_port(status_register_port);
        int status_value = (int) status_return_value;

        if ((status_value & 1) != 0) {
            unsigned char data_port_value = inb_port(data_port);
            int data_value = (int) data_port_value;
            if (data_value == ENTER_RELEASED){
                break;
            }

        }
    }
}
