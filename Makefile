SOURCE_DIRS=C_sources
search_wildcards=$(addsuffix /*.c, $(SOURCE_DIRS))
SRCFILES=$(wildcard $(search_wildcards))
#wildcard - шаблонные символы, позволяет обозначить целую группу файлов,\
 в данном случае исходные файлы с расширением .c и .h
OBJFILES=$(patsubst %.c,%.o,$(SRCFILES))
#patsubst - функция для замены текста (превращаем список исходных файлов в список объектных файлов с такими же именами)
ASMSOURCES=$(wildcard *.asm)

CC=gcc
CFLAGS=-c -Wall -nostdlib -nostdinc -MD $(addprefix -I, $(SOURCE_DIRS))
#флаги, передаваемые Си-компилятору\
-Wall - печать всех предупреждений (warnings)
LD=ld
LDFLAGS=-v -T linker.lds
#флаги, передаваемые линкеру\
-v - показать версию компоновщика\
-T - прочитать линкер-скрипт
AS=nasm
ASFLAGS=-f bin
#флаги, передаваемые компилятору NASM\
-f bin - ассемблировать входной файл в бинарный файл формата com

ELFTARGET=my_super_file.elf 
#промежуточный ELF-файл (компиляция Си-кода)
BINTARGET=c_prog.bin
#бинарный файл, куда записывается вырезанная секция .binary из файла my_super_file.elf 
COMTARGET=kurs.com
#конечная com-программа под DOS
BINSECTION=.binary


.PHONY: all clean


all: $(BINTARGET) $(COMTARGET)

test: $(OBJFILES)
	echo $(SRCFILES)
	echo $(OBJFILES)
	
clean:
	rm ./floppy/kurs.com
	rm $(OBJFILES)

$(COMTARGET): $(ASMSOURCES)
	$(AS) $(ASFLAGS) $(ASMSOURCES) -o $(COMTARGET)

$(BINTARGET): $(ELFTARGET)
	objcopy -j $(BINSECTION) -O binary $(ELFTARGET) $(BINTARGET)

$(ELFTARGET): $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) -o $(ELFTARGET)
	
.c.o:
	$(CC) $(CFLAGS) $< -o $@

include $(wildcard *.d)

#$^ - список всех зависимостей обрабатываемого правила\
$@ - имя цели обрабатываемого правила\
$< - имя первой зависимости обрабатываемого правила