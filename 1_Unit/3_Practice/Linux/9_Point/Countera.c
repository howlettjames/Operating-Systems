segment .text
global _start

_start:
	mov ecx, 0

	increase: 
		mov eax, ecx
		add eax, 48
		push eax
		mov eax, esp
		call strprintLF

		pop eax
		inc ecx
		cmp ecx, 10
		jne increase

		call exit
	

;String print with line feed function
strprintLF:
    call    strprint
 
    push    eax         ; push eax onto the stack to preserve it while we use the eax register in this function
    mov     eax, 0Ah    ; move 0Ah into eax - 0Ah is the ascii character for a linefeed
    push    eax         ; push the linefeed onto the stack so we can get the address
    mov     eax, esp    ; move the address of the current stack pointer into eax for sprint
    call    strprint    ; call our sprint function
    pop     eax         ; remove our linefeed character from the stack
    pop     eax         ; restore the original value of eax before our function was called
ret     	


;String printing function
strprint:
    push    edx
    push    ecx
    push    ebx
    push    eax
    call    strlen
 
    mov     edx, eax
    pop     eax
 
    mov     ecx, eax
    mov     ebx, 1
    mov     eax, 4
    int     0x80
 
    pop     ebx
    pop     ecx
    pop     edx
ret

;String length calculation function
strlen:
    push    ebx
    mov     ebx, eax
 
	nextchar:
	    cmp     byte [eax], 0
	    jz      finished
	    inc     eax
	    jmp     nextchar
	 
	finished:
	    sub     eax, ebx
	    pop     ebx
ret

exit:
	mov eax, 1						;sys_exit
	int 0x80						;Llamada al sistema
	ret
