segment .bss
string1 resb 101
string2 resb 101
string3 resb 101
string4 resb 101
string5 resb 101
string6 resb 101
string7 resb 101
string8 resb 101
string9 resb 101
string10 resb 101
stringf resb 1001					;Final string
stringfi resb 1001					;Final string inverted

segment .data
msg1 db 'Enter the 1 string: ',0
msg2 db 'Enter the 2 string: ',0
msg3 db 'Enter the 3 string: ',0
msg4 db 'Enter the 4 string: ',0
msg5 db 'Enter the 5 string: ',0
msg6 db 'Enter the 6 string: ',0
msg7 db 'Enter the 7 string: ',0
msg8 db 'Enter the 8 string: ',0
msg9 db 'Enter the 9 string: ',0
msg10 db 'Enter the 10 string: ',0
msg11 db 'The final string is: ',0
msg12 db 'The final string inverted is: ',0
msg13 db 'The length is: ',0

segment .txt
global _start

_start:
	mov eax, msg1					;First string				
	call strprint					

	mov eax, string1				
	mov ebx, 100d					
	call strread					

	mov eax, msg2					;Second string
	call strprint

	mov eax, string2
	mov ebx, 100d
	call strread

	mov eax, msg3					;Third string
	call strprint

	mov eax, string3
	mov ebx, 100d
	call strread
	
	mov eax, msg4					;Fourth string
	call strprint

	mov eax, string4
	mov ebx, 100d
	call strread

	mov eax, msg5					;Fifth string
	call strprint

	mov eax, string5
	mov ebx, 100d
	call strread

	mov eax, msg6					;Sexth string
	call strprint

	mov eax, string6
	mov ebx, 100d
	call strread

	mov eax, msg7					;Seventh string
	call strprint

	mov eax, string7
	mov ebx, 100d
	call strread

	mov eax, msg8					;Eighth string
	call strprint

	mov eax, string8
	mov ebx, 100d
	call strread

	mov eax, msg9					;Ninth string
	call strprint

	mov eax, string9
	mov ebx, 100d
	call strread

	mov eax, msg10					;Tenth string
	call strprint

	mov eax, string10
	mov ebx, 100d
	call strread

	mov esi, stringf 				;ESI now contains the reference to stringf		

	mov edi, string1
	call cat

	mov edi, string2
	call cat

	mov edi, string3
	call cat

	mov edi, string4
	call cat

	mov edi, string5
	call cat

	mov edi, string6
	call cat

	mov edi, string7
	call cat

	mov edi, string8
	call cat

	mov edi, string9
	call cat

	mov edi, string10
	call cat

	mov eax, msg11					
	call strprint

	mov byte [esi], 0				;Print the stringf
	mov eax, stringf
	call strprint	

	mov esi, stringfi				;ESI holds the reference to strinf inverted

	mov edi, string10
	call cat

	mov edi, string9
	call cat

	mov edi, string8
	call cat

	mov edi, string7
	call cat

	mov edi, string6
	call cat

	mov edi, string5
	call cat

	mov edi, string4
	call cat

	mov edi, string3
	call cat

	mov edi, string2
	call cat

	mov edi, string1
	call cat

	mov eax, 0xA					;Print a line feed '\n'
	push eax
	mov eax, esp
	call strprint
	pop eax

	mov eax, msg12
	call strprint

	mov byte [esi], 0				;Print stringf inverted
	mov eax, stringfi
	call strprint

	mov eax, 0xA					;Print a line feed '\n'
	push eax
	mov eax, esp
	call strprint
	pop eax

	mov eax, msg13					
	call strprint

	mov eax, stringf 				
	call strlen						;After calling strlen EAX will contain the length of the stringf

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
;Concatenates two strings (EDI hols the reference of the source string, ESI holds the reference of the dest string)
cat:
	copyChar:
		mov eax, [edi]
		mov [esi], eax

		inc edi
		inc esi

		cmp byte [edi], 0xA
		jne copyChar
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