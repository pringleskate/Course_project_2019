#ifndef _PRINT_UTILITIES_H_
#define _PRINT_UTILITIES_H_

#include "cpuid.h"
#include "keystroke.h"

void _putchar(char c);

void skip_page(void);

void clear_screen(void);

void newline(void);

int power(int x, int p);

long int strlen_m(const char * str);

void * memset_m(void * dest, int c, long int count);

int ten_convert(char * hexx);

void convert_to_ascii(char * hex_word, char * word);

void printbincharpad(char c);

#endif