ENTRY(main);

base_address = 0x100000;
SECTIONS
{
	. = base_address;
	.binary : { *(.entry)   }
	.binary : { *(.text)   }
	.binary : { *(.data)   }
	.binary : { *(.rodata) }
	.binary : { *(.bss)    }

	/DISCARD/ : { *(.comment) }
}

