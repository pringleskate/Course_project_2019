use16
org 100h

;разрешение вентиля линии А20 (обязательно для программ защищенного режима) - обеспечивает доступ к HMA (верхней области памяти)
	in	al,92h  
	or	al,2   
	out	92h,al 
  
;подготавливаем псевдодескриптор для загрузки в GDT    
	xor	eax,eax 
	mov	ax,ds	
	shl	eax,4	
	mov	ebp,eax 
	mov	word [gdt_data+2],ax	
	rol	eax,16	
	mov	[gdt_data+4],al 
	
	mov	ax,ds	
	shl	eax,4
	add eax,long_start
	
	mov ax,cs
	shl eax,4
    mov [prog_base],eax

	xor	eax,eax 
	mov	ax,cs	

	shl	eax,4	
	mov	word [gdt_code+2],ax
	rol	eax,16	
	mov	[gdt_code+4],al 

    add ebp,gdt_null
	mov	dword	[pdescr+2],ebp	
	mov	word	[pdescr],gdt_size-1
	
	lgdt [pdescr] 

	cli		
	mov	al,80h
	mov	eax,cr0 
	or	eax,1	
	mov	cr0,eax
	jmp	16:continue

continue:
use32
   	mov	eax, 8	; load 4 GB data descriptor
	mov	ds, ax
	mov ss, ax	; to all data segment registers
				;Обнуляем память 
	mov ax, 32
	mov es, ax
	mov	eax, cr4
	or eax, 1 << 5 

	mov	cr4, eax 
	mov ecx, 1000h*50
	mov edi, 70000h
	mov eax, 0
	rep	stosd
	
;Заполняем (PML4)
	mov dword [es:700000h],701007h
;Заполняем (PDPE)
	mov dword [es:701000h],702007h
	mov dword [es:701008h],703007h
	mov dword [es:701010h],704007h
	mov dword [es:701018h],705007h

;Заполняем таблицы страниц
	mov ecx, 2048
	mov edi, 702000h
	mov eax, 87h
	
cycle2:

	mov dword [es:edi],eax
	add edi, 8h
	add eax, 200000h
	dec ecx
	cmp ecx, 0
	jne cycle2

	mov	eax, 700000h
	mov	cr3, eax 		; load page-map level-4 base

	mov	ecx, 0C0000080h		; EFER MSR
	rdmsr
	or eax, 1 << 8

	wrmsr

	mov	eax, cr0
	or eax, 1 << 31

	mov	cr0, eax 		; enable paging

	mov eax, [prog_base]
	add eax, long_start
	push word 24
	push eax
	
	mov ax, 8
	mov ds, ax
	mov ax, 32
	mov es, ax
	mov esi, relocation_start
	mov edi, 100000h
	mov ecx, relocation_end - relocation_start
	rep movsb
	mov ax, 8
	mov ds, ax
	
	jmp	24:(100000h + long_start - relocation_start)

use64
relocation_start:
bin_start:
	incbin "c_prog.bin"
long_start:
	mov ax, 32
	mov ds, ax
	mov ss, ax
	mov es, ax

	;for testing PS/2
	xor rax, rax
	in al, 60h
	
	
	xor rax, rax
	xor rbx, rbx
	xor rcx, rcx
	xor rdx, rdx
	xor rdi, rdi
	xor rsi, rsi

	mov rsp, 0x200000
	mov rbp, rsp
	
	mov rax, 100000h
	call rax
	
	jmp $
relocation_end:
	


;таблица глобальных десрипторов
align	8

gdt_null	db	00h,00h,00h,00h,00h,00h,00h,00h
gdt_data	db	0FFh,0FFh,0,0,0,92h,0CFh,0
gdt_code	db	0FFh,0FFh,0,0,0,9Ah,0CFh,0
gdt_long    db 	0FFh,0FFh,0,0,0,9Ah,0AFh,0
gdt_data2   db	0FFh,0FFh,0,0,0,92h,0CFh,0

gdt_size db $-gdt_null

pdescr	dq		0	;псевдодескриптор для команды LGDT
prog_base dd	0
