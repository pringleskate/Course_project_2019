#include "cpuid.h" 


int REG_with_features = 0;        //вспомогательная переменная для вызова функции check_bit_register()
int MSR_enable_flag = 0;          //если выставлен, то MSR-регистры доступны, для чтения в частности
static int Vendor = 0;            //флаг для идентификации производителя  

/*Ассемблерная функция cpuid, которая заполняет содержимое структуры с регистрами.*/
struct Registers cpuid(int eax) {
    struct Registers regs; 
    asm("cpuid"
        :"=a"(regs.REG_EAX),
        "=b"(regs.REG_EBX),
        "=d"(regs.REG_EDX),
        "=c"(regs.REG_ECX)
        :"a"(eax)
        );
    return regs;
}

/*Функция для считывания характеристик процессора.*/
void get_CPUID(int eax) {
    if (eax == 0){
        /*Если выполнить инструкцию CPUID c входным параметром EAX=0, то в выходные параметры:
        EAX - максимальное допустимое значение, с которым можно выполнять CPUID
        EBX:EDX:ECX - 12-байтный идентификатор производителя*/      
        get_manufacturer_string();
    }
    else if (eax == 1) {
        switch (Vendor) {
            case 1:
                get_CPU_info();
                get_CPU_features_Intel();
                break;
            case 2:
                get_CPU_info();
                get_CPU_features_AMD();
                break;
            case 0:
                printf_("Unknown Vendor ID\n");
                break;
        }  
    }
    else 
        printf_("Unknown CPUID argument\n");
}

/*Вывод идентификатора производителя процессора (Vendor ID) */
void get_manufacturer_string(void) {
    struct Registers regs = cpuid(0);
    char name_ecx[4] = {0};
    char name_edx[4] = {0};
    char name_ebx[4] = {0};

    convert_vendor_ID(regs.REG_EBX, name_ebx); 
    convert_vendor_ID(regs.REG_EDX, name_edx); 
    convert_vendor_ID(regs.REG_ECX, name_ecx); 

    printf_("CPUID\n\n");
    printf_("Vendor id: %s%s%s\n", name_ebx, name_edx, name_ecx);
    if (regs.REG_EBX == 0x756E6547) //Intel
        Vendor = 1;
    else if (regs.REG_EBX == 0x68747541) //AMD
        Vendor = 2;
    //Переменная Vendor остается равной 0, если производитель не определен
}


/*Вывод базовой информации о процессоре, 
  такой как идентификаторы семейства, модели и типа процессора (для Intel).*/
void get_CPU_info(void) {
    
    struct Registers regs = cpuid(1);

    printf_("Extended family ID:  %X\n", (regs.REG_EAX & 0xF00000) >> 20); //EAX bits 27-20
    printf_("Extended model ID: %X\n", (regs.REG_EAX & 0xF0000) >> 16); //EAX bits 19-16

    printf_("Family ID: %X\n", (regs.REG_EAX & 0xF00) >> 8); //EAX bits 11-8
    printf_("Model ID: %X\n", (regs.REG_EAX & 0xF0) >> 4); //EAX bits 7-4
    printf_("Stepping ID: %X\n", regs.REG_EAX & 0xF); //EAX bits 3-0

    if (Vendor == 1) {
        printf_("Procesor Type: "); //EAX bits 13-12
        switch ( (regs.REG_EAX & 0x3000) >> 12 ) {
            case 0:
                printf_("OEM\n");
                break;

            case 1:
                printf_("Overdrive\n");
                break;

            case 2:
                printf_("Dual\n");
                break;
        }
    }
    else 
        printf_("Processor Type is not available for reading\n");
    
}

/*Вывод информации об особенностях конкретного процессора производства Intel.*/
void get_CPU_features_Intel(void) {
    struct Registers regs = cpuid(1);

    printf_("\nCPU features:\n");

    REG_with_features = regs.REG_EDX;
    printf_(" FPU                                             : "); printf_("%d\n", check_bit_register(0));
    printf_(" VMode Extensions                                : "); printf_("%d\n", check_bit_register(1));
    printf_(" Debugging Extensions                            : "); printf_("%d\n", check_bit_register(2));
    printf_(" Pages Size Extensions                           : "); printf_("%d\n", check_bit_register(3));
    printf_(" Time Stamp Counter                              : "); printf_("%d\n", check_bit_register(4));
    printf_(" Model Specific Registers                        : "); printf_("%d\n", check_bit_register(5));
    if (check_bit_register(5))
        MSR_enable_flag = 1;
    printf_(" Physical Address Extension                      : "); printf_("%d\n", check_bit_register(6));
    printf_(" Machine-Check Exception                         : "); printf_("%d\n", check_bit_register(7));
    printf_(" CMPXCHG8B Instruction                           : "); printf_("%d\n", check_bit_register(8));
    printf_(" On-chip APIC Hardware                           : "); printf_("%d\n", check_bit_register(9));
    //bit 10 - reserved
    printf_(" Fast System Call                                : "); printf_("%d\n", check_bit_register(11));
    printf_(" Memory Type Range Registers (MTRR)              : "); printf_("%d\n", check_bit_register(12));
    printf_(" Page Global Enable                              : "); printf_("%d\n", check_bit_register(13));
    printf_(" Machine-Check Architecture                      : "); printf_("%d\n", check_bit_register(14));
    printf_(" Conditional Move Instruction                    : "); printf_("%d\n", check_bit_register(15));
    printf_(" Page Attributes Table                           : "); printf_("%d\n", check_bit_register(16));
    printf_(" 36-bit Page Size Extension                      : "); printf_("%d\n", check_bit_register(17));
    printf_(" Processor serial number is present and enabled  : "); printf_("%d\n", check_bit_register(18));
    printf_(" CLFLUSH Instruction                             : "); printf_("%d\n", check_bit_register(19));
    //bit 20 - reserved
    printf_(" Debug Store                                     : "); printf_("%d\n", check_bit_register(21));
    printf_(" ACPI                                            : "); printf_("%d\n", check_bit_register(22));
    printf_(" MMX Technology                                  : "); printf_("%d\n", check_bit_register(23));
    printf_(" FXSAVE, FXRSTOR Instructions                    : "); printf_("%d\n", check_bit_register(24));
    printf_(" Streaming SIMD Extensions                       : "); printf_("%d\n", check_bit_register(25));
    printf_(" Streaming SIMD Extensions 2                     : "); printf_("%d\n", check_bit_register(26));
    printf_(" Self-Snoop                                      : "); printf_("%d\n", check_bit_register(27));
    printf_(" Multi-Threading                                 : "); printf_("%d\n", check_bit_register(28));
    printf_(" Thermal Monitor                                 : "); printf_("%d\n", check_bit_register(29));
    //bit 30 - reserved
    printf_(" Pending Break Enable                            : "); printf_("%d\n", check_bit_register(31));

    REG_with_features = regs.REG_ECX;
    printf_(" Streaming SIMD Extensions 3                     : "); printf_("%d\n", check_bit_register(0));
    printf_(" PCLMULDQ Instruction                            : "); printf_("%d\n", check_bit_register(1));
    printf_(" 64-bit Debug Store                              : "); printf_("%d\n", check_bit_register(2));
    printf_(" MONITOR/MWAIT                                   : "); printf_("%d\n", check_bit_register(3));
    printf_(" CPL Qualified Debug Store                       : "); printf_("%d\n", check_bit_register(4));
    printf_(" Virtual Machine Extensions                      : "); printf_("%d\n", check_bit_register(5));
    printf_(" Safer Mode Extensions                           : "); printf_("%d\n", check_bit_register(6));
    printf_(" Enhanced Intel SpeedStepTechnology              : "); printf_("%d\n", check_bit_register(7));
    printf_(" Thermal Monitor 2                               : "); printf_("%d\n", check_bit_register(8));
    printf_(" Supplemental Streaming SIMD Extensions 3        : "); printf_("%d\n", check_bit_register(9));
    printf_(" LI Context ID                                   : "); printf_("%d\n", check_bit_register(10));
    //bit 11 - reserved
    printf_(" Fused Multiply Add                              : "); printf_("%d\n", check_bit_register(12));
    printf_(" CMPXCHG16B                                      : "); printf_("%d\n", check_bit_register(13));
    printf_(" xTPR Update Control                             : "); printf_("%d\n", check_bit_register(14));
    printf_(" Perfmon and Debug Capability                    : "); printf_("%d\n", check_bit_register(15));
    //bit 16 - reserved
    printf_(" Process Context Identifiers                     : "); printf_("%d\n", check_bit_register(17));
    printf_(" Direct Cache Access                             : "); printf_("%d\n", check_bit_register(18));
    printf_(" Streaming SIMD Extensions 4.1                   : "); printf_("%d\n", check_bit_register(19));
    printf_(" Streaming SIMD Extensions 4.2                   : "); printf_("%d\n", check_bit_register(20));
    printf_(" Extended xAPIC Support                          : "); printf_("%d\n", check_bit_register(21));
    printf_(" MOVBE Instruction                               : "); printf_("%d\n", check_bit_register(22));
    printf_(" POPCNT Instruction                              : "); printf_("%d\n", check_bit_register(23));
    printf_(" Time Stamp Counter Deadline                     : "); printf_("%d\n", check_bit_register(24));
    printf_(" AES Instruction Extensions                      : "); printf_("%d\n", check_bit_register(25));
    printf_(" XSAVE/XSTOR States                              : "); printf_("%d\n", check_bit_register(26));
    printf_(" OS-Enabled Extended State Management            : "); printf_("%d\n", check_bit_register(27));
    printf_(" Advanced Vector Extensions                      : "); printf_("%d\n", check_bit_register(28));
    printf_(" 16-bit floating-point conversion instructions   : "); printf_("%d\n", check_bit_register(29));
    printf_(" RDRAND instruction supported                    : "); printf_("%d\n", check_bit_register(30));
    //bit 31 - not used
}

/*Вывод информации об особенностях конкретного процессора производства AMD.*/
void get_CPU_features_AMD(void) {
    struct Registers regs = cpuid(1);

    printf_("\nCPU features:\n");

    REG_with_features = regs.REG_EDX;
    printf_(" FPU                                             : "); printf_("%d\n", check_bit_register(0));
    printf_(" VMode Extensions                                : "); printf_("%d\n", check_bit_register(1));
    printf_(" Debugging Extensions                            : "); printf_("%d\n", check_bit_register(2));
    printf_(" Pages Size Extensions                           : "); printf_("%d\n", check_bit_register(3));
    printf_(" Time Stamp Counter                              : "); printf_("%d\n", check_bit_register(4));
    printf_(" Model Specific Registers                        : "); printf_("%d\n", check_bit_register(5));
    if (check_bit_register(5))
        MSR_enable_flag = 1;
    printf_(" Physical Address Extension                      : "); printf_("%d\n", check_bit_register(6));
    printf_(" Machine-Check Exception                         : "); printf_("%d\n", check_bit_register(7));
    printf_(" CMPXCHG8B Instruction                           : "); printf_("%d\n", check_bit_register(8));
    printf_(" APIC                                            : "); printf_("%d\n", check_bit_register(9));
    //bit 10 - reserved
    printf_(" SYSENTER and SYSEXIT instructions               : "); printf_("%d\n", check_bit_register(11));
    printf_(" Memory Type Range Registers (MTRR)              : "); printf_("%d\n", check_bit_register(12));
    printf_(" Page Global Extension                           : "); printf_("%d\n", check_bit_register(13));
    printf_(" Machine-Check Architecture                      : "); printf_("%d\n", check_bit_register(14));
    printf_(" Conditional Move Instruction                    : "); printf_("%d\n", check_bit_register(15));
    printf_(" Page Attributes Table                           : "); printf_("%d\n", check_bit_register(16));
    printf_(" 36-bit Page Size Extension                      : "); printf_("%d\n", check_bit_register(17));
    //bit 18 - reserved
    printf_(" CLFLUSH Instruction                             : "); printf_("%d\n", check_bit_register(19));
    //bit 20:22 - reserved
    printf_(" MMX Instruction                                 : "); printf_("%d\n", check_bit_register(23));
    printf_(" FXSAVE, FXRSTOR Instructions                    : "); printf_("%d\n", check_bit_register(24));
    printf_(" Streaming SIMD Extensions                       : "); printf_("%d\n", check_bit_register(25));
    printf_(" Streaming SIMD Extensions 2                     : "); printf_("%d\n", check_bit_register(26));
    //bit 27 - reserved
    printf_(" Hyper-threading Technology                      : "); printf_("%d\n", check_bit_register(28));
    //bit 29:31 - reserved

    REG_with_features = regs.REG_ECX;
    printf_(" Streaming SIMD Extensions 3                     : "); printf_("%d\n", check_bit_register(0));
    printf_(" PCLMULDQ Instruction                            : "); printf_("%d\n", check_bit_register(1));
    //bit 2 - reserved
    printf_(" MONITOR/MWAIT                                   : "); printf_("%d\n", check_bit_register(3));
    //bit 4:8 - reserved
    printf_(" Supplemental Streaming SIMD Extensions 3        : "); printf_("%d\n", check_bit_register(9));
    //bit 10:11 - reserved
    printf_(" Fused Multiply Add                              : "); printf_("%d\n", check_bit_register(12));
    printf_(" CMPXCHG16B                                      : "); printf_("%d\n", check_bit_register(13));
    //bit 14:18 - reserved
    printf_(" Streaming SIMD Extensions 4.1                   : "); printf_("%d\n", check_bit_register(19));
    printf_(" Streaming SIMD Extensions 4.2                   : "); printf_("%d\n", check_bit_register(20));
    //bit 21:22 - reserved
    printf_(" POPCNT Instruction                              : "); printf_("%d\n", check_bit_register(23));
    //bit 24 - reserved
    printf_(" AES Instruction Extensions                      : "); printf_("%d\n", check_bit_register(25));
    printf_(" XSAVE/XSTOR States                              : "); printf_("%d\n", check_bit_register(26));
    printf_(" XSAVE (and related) instructions are enabled    : "); printf_("%d\n", check_bit_register(27));
    printf_(" AVX instruction support                         : "); printf_("%d\n", check_bit_register(28));
    printf_(" F16C half-precision convert instruction support : "); printf_("%d\n", check_bit_register(29));
    //bit 30 - reserved
    //bit 31 - reserved for use by hypervisor to indicate guest status*/
}

/*Конвертация строки-идентификатора производителя в ASCII-код*/
void convert_vendor_ID(int number_word, char * word) {
    char hex_word[8] = {0};
    memset_m(hex_word, 0, 9);
    sprintf_(hex_word, "%X", number_word);
    convert_to_ascii(hex_word, word);
}


/*Функция проверяет установлен ли бит номер flag в REG_EDX или REG_ECX 
  для дальнейшего использования в выводе об особенностях процессора.*/
int check_bit_register(int flag) {
    int mask=1;
    mask <<= flag;

    if ( (REG_with_features & mask) != 0) 
        return 1;
    else 
        return 0;
}