global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resd 1
;guardando valor constante 2 em temporário
section .text
	mov eax, 2
	mov [qword M+0], eax
;Comando de atribuição
section .text
	mov eax,[qword M+0]
	mov [qword M+65536], eax
section .data
;Declaração de Variável
	resd 1
;guardando valor constante 3 em temporário
section .text
	mov eax, 3
	mov [qword M+4], eax
;Comando de atribuição
section .text
	mov eax,[qword M+4]
	mov [qword M+65540], eax
section .data
;Declaração de Variável
	resd 1
section .text
mov eax, [qword M+65540]
mov ebx, [qword M+65536]
idiv ebx
mov [qword M+8], eax
mov eax, [qword M+65536]
mov ebx, [qword M+65540]
add eax,ebx
mov [qword M+12], eax
mov eax, [qword M+8]
mov ebx, [qword M+12]
imul ebx
mov [qword M+16], eax
;Comando de atribuição
section .text
	mov eax,[qword M+16]
	mov [qword M+65544], eax
section .text
;Escrevendo na saida do terminal
mov eax, [qword M+65544]
mov rsi, M+20
mov rcx, 0
mov rdi, 0
cmp eax, 0
jge Rot0
mov bl, '-' 
mov [rsi], bl
add rsi, 1
add rdi, 1
neg eax
Rot0:
mov ebx, 10
Rot1:
add rcx, 1
cdq
idiv ebx
push dx
cmp eax, 0
jne Rot1
add rdi,rcx
Rot2:
pop dx
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rcx, 1
cmp rcx, 0
jne Rot2
mov rsi, M+20
mov rax, 1
mov rdi, 1
syscall
mov al, 0ah
mov [qword M+276], al
mov rsi, M+276
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
