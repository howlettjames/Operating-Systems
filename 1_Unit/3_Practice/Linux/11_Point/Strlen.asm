segment .bss
string resb 101

segment .data
msg1 db 'Enter the string: ',0
msg2 db 'The length is: ',0

segment .txt
global _start

_start:
	mov eax, msg1					
	call strprint					;print msg

	mov eax, string					;string is the buffer
	mov ebx, 100d					;100 bytes to read
	call strread					;read the bytes into string

	mov eax, msg2
	call strprint

	mov eax, string					;save the address of string into eax
	call strlen						;calculate the length of string, eax will contain the length

	dec eax							;To not count the '\n'
	call iprintLF	

	call exit

;------------------------------------------
; void iprint(Integer number)
; Integer printing function (itoa)
iprint:
    push    eax             ; preserve eax on the stack to be restored after function runs
    push    ecx             ; preserve ecx on the stack to be restored after function runs
    push    edx             ; preserve edx on the stack to be restored after function runs
    push    esi             ; preserve esi on the stack to be restored after function runs
    mov     ecx, 0          ; counter of how many bytes we need to print in the end
 
divideLoop:
    inc     ecx             ; count each byte to print - number of characters
    mov     edx, 0          ; empty edx
    mov     esi, 10         ; mov 10 into esi
    idiv    esi             ; divide eax by esi
    add     edx, 48         ; convert edx to it's ascii representation - edx holds the remainder after a divide instruction
    push    edx             ; push edx (string representation of an intger) onto the stack
    cmp     eax, 0          ; can the integer be divided anymore?
    jnz     divideLoop      ; jump if not zero to the label divideLoop
 
printLoop:
    dec     ecx             ; count down each byte that we put on the stack
    mov     eax, esp        ; mov the stack pointer into eax for printing
    call    strprint          ; call our string print function
    pop     eax             ; remove last character from the stack to move esp forward
    cmp     ecx, 0          ; have we printed all bytes we pushed onto the stack?
    jnz     printLoop       ; jump is not zero to the label printLoop
 
    pop     esi             ; restore esi from the value we pushed onto the stack at the start
    pop     edx             ; restore edx from the value we pushed onto the stack at the start
    pop     ecx             ; restore ecx from the value we pushed onto the stack at the start
    pop     eax             ; restore eax from the value we pushed onto the stack at the start
ret
 
;------------------------------------------
; void iprintLF(Integer number)
; Integer printing function with linefeed (itoa)
iprintLF:
    call    iprint          ; call our integer printing function
 
    push    eax             ; push eax onto the stack to preserve it while we use the eax register in this function
    mov     eax, 0Ah        ; move 0Ah into eax - 0Ah is the ascii character for a linefeed
    push    eax             ; push the linefeed onto the stack so we can get the address
    mov     eax, esp        ; move the address of the current stack pointer into eax for sprint
    call    strprint          ; call our sprint function
    pop     eax             ; remove our linefeed character from the stack
    pop     eax             ; restore the original value of eax before our function was called
ret

;------------------------------------------
;Lee una cadena del tamaño de n bytes especificados en edx
strread:	
	mov edx, ebx					;ebx contiene la cantidad de bytes a leer
	mov ecx, eax					;contiene la dirección de la cadena en la cual guardar
	mov ebx, 0						;Entrada estándar
	mov eax, 3						;sys_read
	int 0x80						;Llamada al sistema
ret 	 							;Salimos de la función

;------------------------------------------
;Imprime una cadena	
strprint:
	push edx						;Guardamos el valor de los registros a ocupar
	push ecx	
	push ebx
	push eax
	call strlen						;Llamamos a strlen

	mov edx, eax					;edx contiene ahora la longitud de la cadena
	pop eax							;eax contiene la dirección de la cadena a imprimir

	mov ecx, eax					;Le pasamos la dirección de la cadena a ecx
	mov ebx, 1						;Salida estándar
	mov eax, 4						;sys_write
	int 0x80						;Llamada al sistema

	pop ebx							;Rescatamos el valor de nuestros registros
	pop ecx
	pop edx
ret 	 							;Salimos de la función

;------------------------------------------
; int strlen(String message)
; String length calculation function
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

 ;-----------------------------------------
;Cierra el programa
exit:
	mov eax, 1						;sys_exit
	int 0x80						;Llamada al sistema
ret