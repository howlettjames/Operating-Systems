segment .bss
string1 resb 21						;Save 1 byte more for the '0' (null)
string2 resb 21
string3 resb 21
string4 resb 61

segment .data
msg1 db 'Enter the first string: ',0
msg2 db 'Enter the second string: ',0
msg3 db 'Enter the third string: ',0
msg4 db 'The fourth string is: '
segment .text
global _start

_start:
	mov eax, msg1					;Move the address of msg1 into eax
	call strprint					;Print the first message

	mov eax, string1				;Move the address of string1 into eax
	mov ebx, 20d					;Size of bytes to read
	call strread					;Call strwrite	

	mov eax, msg2					;Print the second message
	call strprint					

	mov eax, string2				;Read the second string2
	mov ebx, 20d					
	call strread					

	mov eax, msg3					;Print the third message
	call strprint					

	mov eax, string3				;Read the third string3
	mov ebx, 20d					
	call strread					

	mov eax, string1			;eax contains the address of string1
	mov ebx, string2			;ebx contains the address of string2
	mov ecx, string3			;ecx contains the address of string3
	mov esi, string4			;esi contains the address of string4

	copiaChar:
		mov edi, [eax]			;edi is used just to hold the value of the string
		mov [esi], edi			;we copy then the value into the desired string (in this case string4)
		inc esi

		mov edi, [ebx]
		mov [esi], edi
		inc esi

		mov edi, [ecx]
		mov [esi], edi
		inc esi

		inc eax					;move the pointer in the string
		cmp byte [eax], 0xA		;check if it has reached the end of the string
		jz finish				;in case of, jump to the finish process

		inc ebx
		cmp byte [ebx], 0xA
		jz finish

		inc ecx
		cmp byte [ebx], 0xA
		jz finish

		jmp copiaChar			;in the case any string has reached the end continue

	finish:
		mov eax, msg4
		call strprint
		
		mov byte [esi], 0		;put the NULL value into the string
		mov eax, string4		;print the string4
		call strprint

		mov eax, 0xA
		push eax
		mov eax, esp
		call strprint

	call exit						;exit program	

;Lee una cadena
strread:	
	mov edx, ebx					;ebx contiene la cantidad de bytes a leer
	mov ecx, eax					;contiene la dirección de la cadena en la cual guardar
	mov ebx, 0						;Entrada estándar
	mov eax, 3						;sys_read
	int 0x80						;Llamada al sistema
	ret 							;Salimos de la función

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

;Calcula la longitud de una cadena
strlen:
	push ebx						;Guardamos el valor de ebx
	mov ebx, eax					;Movemos la dirección de eax en ebx

	nextchar:
		cmp byte [eax], 0			;Es el fin de la candena?
		jz finished					;Caso de Sí terminamos
		inc eax						;Caso de No nos seguimos moviendo en la candena
		jmp nextchar				

	finished:
		sub eax, ebx				;eax contendrá la longitud de la cadena
		pop ebx						;Rescatamos el valor que tenía ebx
ret 								;Salimos de la función

;Cierra el programa
exit:
	mov eax, 1						;sys_exit
	int 0x80						;Llamada al sistema
ret