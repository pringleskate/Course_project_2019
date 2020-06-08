#!/bin/bash

if (find main.o) && (find ./floppy/*.com); then 
	make clean all
else 
	make 
fi &&
file_name=$(find . -maxdepth 1 -name "*.com") &&
mv ./"$file_name" ./floppy/ &&

#choose one of the options for starting QEMU with exact CPU:
#-cpu Nehalem ------ Intel Core i7 9xx (Nehalem Class Core i7, 2008)
#-cpu Opteron_G3 --- AMD Opteron 23xx (Gen 3 Class Opteron, 2009)

qemu-system-x86_64 -cpu Nehalem -hda dos.qcow -m 256 -fda fat:floppy:ro:floppy -monitor stdio
#qemu-system-x86_64 -cpu Opteron_G3 -hda dos.qcow -m 256 -fda fat:floppy:ro:floppy -monitor stdio