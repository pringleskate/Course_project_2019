#ifndef _CPUID_H_
#define _CPUID_H_

#include "printf.h"
#include "cpuid.h"

/*Структура для хранения содержимого регистров.*/
struct Registers {
	int REG_EAX; 
	int REG_EBX; 
	int REG_ECX; 
	int REG_EDX
 };

struct Registers rdmsr(int ecx);

void get_MSR(int ecx);

#endif