#include "rdmsr.h"

extern int REG_with_features; //вспомогательная переменная для вызова функции check_bit_register()
extern int MSR_enable_flag;

/*Ассемблерная функция rdmsr, которая заполняет содержимое структуры с регистрами.*/
struct Registers rdmsr(int ecx){
    struct Registers regs; 
    asm("rdmsr"
       :"=a"(regs.REG_EAX),
        "=d"(regs.REG_EDX)
       :"c"(ecx)
       );
    return regs;
}

/*Функция для считывания содержимого моделезависимых регистров процессора.*/
void get_MSR(int ecx)
{
    if (MSR_enable_flag) {
        printf_("\n\n\nMSR REGISTERS\n\n");

        struct Registers regs = rdmsr(ecx);
        if (ecx == 0xC0000080){
            REG_with_features = regs.REG_EAX;
            printf_("Extended Feature Enable Register (EFER MSR) %X%X: \n", regs.REG_EDX, regs.REG_EAX);
            printf_("   SCE   = SysCall/sysret is Enabled        "); printf_("%d\n", check_bit_register(0));
            printf_("   LME   = Long-Mode is Enabled             "); printf_("%d\n", check_bit_register(8));
            printf_("   LMA   = Long-Mode is Active              "); printf_("%d\n", check_bit_register(10));
            printf_("   NXE   = Non-eXecutable pages Enabled     "); printf_("%d\n", check_bit_register(11));
            printf_("   SVME  = Secure Virtual Machine Enabled   "); printf_("%d\n", check_bit_register(12));
            printf_("   LMSLE = Long Mode Segment Limit Enable   "); printf_("%d\n", check_bit_register(13));
            printf_("   FFXSR = fast FXSAVE/FXSTOR               "); printf_("%d\n", check_bit_register(14));
            printf_("   TCE   = Translation Cache Extension      "); printf_("%d\n", check_bit_register(15));
        }
    }
}

