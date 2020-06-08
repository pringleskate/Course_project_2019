#include "cpuid.h"
#include "keystroke.h"
#include "rdmsr.h"


__attribute__((section(".entry")))
int main(int argc, char *argv[])
{
	catch_enter();
    get_CPUID(0);
    get_CPUID(1);
	get_MSR(EFER_REG);

  	return 0;
}
