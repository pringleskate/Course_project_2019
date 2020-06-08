#ifndef _CPUID_H_
#define _CPUID_H_

#define TSC_REG 0x10 //Time-Stamp counter 
#define APIC_BASE_REG 0x1B
#define EFER_REG 0xC0000080 //Extended Feature Enable Register 
							//SCE = SysCall/sysret is Enabled (1=yes, 0=no) - bit 0
							//LME = Long-Mode is Enabled (1=yes, 0=no) - bit 8
							//LMA = Long-Mode is Active (1=yes, 0=no) - bit 10
							//NXE = Non-eXecutable pages Enabled (1=yes, 0=no - bit 11 

#include "printf.h"

/*Структура для хранения содержимого регистров.*/
struct Registers {
	int REG_EAX; 
	int REG_EBX; 
	int REG_ECX; 
	int REG_EDX
};

struct Registers cpuid(int eax);

void get_CPUID(int eax);

void get_manufacturer_string(void);

void get_CPU_info(void);

void get_CPU_features_Intel(void);

void get_CPU_features_AMD(void);

void convert_vendor_ID(int number_word, char * word);

int check_bit_register(int flag);

#endif