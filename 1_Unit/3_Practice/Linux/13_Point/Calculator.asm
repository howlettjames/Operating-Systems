segment .data
msg1 db 'MENU',0
msg2 db '1. Addition',0
msg3 db '2. Subtraction',0
msg4 db '3. Multiplication',0
msg5 db '4. Division',0
msg6 db '5. Exit',0
msg7 db 'Choose your option: ',0
msg8 db 'Enter the first number: ',0
msg9 db 'Enter the second number: ',0
msg10 db 'The result is: ',0
msg11 db 'Choose a correct option',0

segment .bss
option resb 5
number1 resb 5
number2 resb 5

segment .text
global _start

_start:
	menu:
		mov eax, msg1 					;Print the MENU
		call strprintLF

		mov eax, msg2
		call strprintLF

		mov eax, msg3
		call strprintLF

		mov eax, msg4
		call strprintLF

		mov eax, msg5
		call strprintLF

		mov eax, msg6
		call strprintLF

		mov eax, msg7
		call strprint

		mov eax, option					;Read the option
		mov ebx, 2
		call strread 

		mov eax, option					;Compare the option

		cmp byte [eax], 49
		jz addition

		cmp byte [eax], 50
		jz subtraction

		cmp byte [eax], 51
		jz multiplication

		cmp byte [eax], 52
		jz division

		cmp byte [eax], 53 
		jz _exit

		mov eax, msg11
		call strprintLF
		
	jmp menu

	_exit:
		call exit			

	addition:
		mov eax, msg8				;Enter the first number
		call strprint

		mov eax, number1
		mov ebx, 5
		call strread

		mov eax, msg9
		call strprint 				;Enter the second number

		mov eax, number2
		mov ebx, 5
		call strread

		mov eax, msg10				;The result is: 
		call strprint
		
		mov eax, number1
		call atoi
		mov edx, eax
		
		mov eax, number2
		call atoi
		add edx, eax

		mov eax, edx	
		call iprintLF

	jmp menu

	subtraction:
		mov eax, msg8				;Enter the first number
		call strprint

		mov eax, number1
		mov ebx, 5
		call strread

		mov eax, msg9
		call strprint 				;Enter the second number

		mov eax, number2
		mov ebx, 5
		call strread

		mov eax, msg10				;The result is: 
		call strprint
		
		mov eax, number1
		call atoi
		mov edx, eax
		
		mov eax, number2
		call atoi
		sub edx, eax

		mov eax, edx	
		call iprintLF

	jmp menu

	multiplication:
		mov eax, msg8				;Enter the first number
		call strprint

		mov eax, number1
		mov ebx, 5
		call strread

		mov eax, msg9
		call strprint 				;Enter the second number

		mov eax, number2
		mov ebx, 5
		call strread

		mov eax, msg10				;The result is: 
		call strprint
		
		mov eax, number1
		call atoi
		mov edx, eax
		
		mov eax, number2
		call atoi
		mov ebx, eax
		mov eax, edx

		mul ebx

		call iprintLF

	jmp menu

	division:
		mov eax, msg8				;Enter the first number
		call strprint

		mov eax, number1
		mov ebx, 5
		call strread

		mov eax, msg9
		call strprint 				;Enter the second number

		mov eax, number2
		mov ebx, 5
		call strread

		mov eax, msg10				;The result is: 
		call strprint
		
		mov eax, number1
		call atoi
		mov edx, eax

		mov eax, number2
		call atoi
		mov ebx, eax
		mov eax, edx
		mov edx, 0					;Clean the EDX register to avoid segmentation fault

		div ebx

		call iprintLF

	jmp menu

;------------------------------------------
; int atoi(Integer number)
; Ascii to integer function (atoi)
atoi:
    push    ebx             ; preserve ebx on the stack to be restored after function runs
    push    ecx             ; preserve ecx on the stack to be restored after function runs
    push    edx             ; preserve edx on the stack to be restored after function runs
    push    esi             ; preserve esi on the stack to be restored after function runs
    mov     esi, eax        ; move pointer in eax into esi (our number to convert)
    mov     eax, 0          ; initialise eax with decimal value 0
    mov     ecx, 0          ; initialise ecx with decimal value 0
 
.multiplyLoop:
    xor     ebx, ebx        ; resets both lower and uppper bytes of ebx to be 0
    mov     bl, [esi+ecx]   ; move a single byte into ebx register's lower half
    cmp     bl, 48          ; compare ebx register's lower half value against ascii value 48 (char value 0)
    jl      .finished       ; jump if less than to label finished
    cmp     bl, 57          ; compare ebx register's lower half value against ascii value 57 (char value 9)
    jg      .finished       ; jump if greater than to label finished
    cmp     bl, 10          ; compare ebx register's lower half value against ascii value 10 (linefeed character)
    je      .finished       ; jump if equal to label finished
    cmp     bl, 0           ; compare ebx register's lower half value against decimal value 0 (end of string)
    jz      .finished       ; jump if zero to label finished
 
    sub     bl, 48          ; convert ebx register's lower half to decimal representation of ascii value
    add     eax, ebx        ; add ebx to our interger value in eax
    mov     ebx, 10         ; move decimal value 10 into ebx
    mul     ebx             ; multiply eax by ebx to get place value
    inc     ecx             ; increment ecx (our counter register)
    jmp     .multiplyLoop   ; continue multiply loop
 
.finished:
    mov     ebx, 10         ; move decimal value 10 into ebx
    div     ebx             ; divide eax by value in ebx (in this case 10)
    pop     esi             ; restore esi from the value we pushed onto the stack at the start
    pop     edx             ; restore edx from the value we pushed onto the stack at the start
    pop     ecx             ; restore ecx from the value we pushed onto the stack at the start
    pop     ebx             ; restore ebx from the value we pushed onto the stack at the start
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
    call    strprint        ; call our string print function
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
    call    strprint        ; call our sprint function
    pop     eax             ; remove our linefeed character from the stack
    pop     eax             ; restore the original value of eax before our function was called
ret
 
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

;------------------------------------------
; void strprint(String message)
; String printing function
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
    int     80h
 
    pop     ebx
    pop     ecx
    pop     edx
ret
	
;------------------------------------------
; void strprintLF(String message)
; String printing with line feed function
strprintLF:
    call    strprint
 
    push    eax         ; push eax onto the stack to preserve it while we use the eax register in this function
    mov     eax, 0Ah    ; move 0Ah into eax - 0Ah is the ascii character for a linefeed
    push    eax         ; push the linefeed onto the stack so we can get the address
    mov     eax, esp    ; move the address of the current stack pointer into eax for sprint
    call    strprint      ; call our sprint function
    pop     eax         ; remove our linefeed character from the stack
    pop     eax         ; restore the original value of eax before our function was called
ret 	                ; return to our program

;-----------------------------------------
;Cierra el programa
exit:
	mov eax, 1						;sys_exit
	int 0x80						;Llamada al sistema
ret