global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resd 1
section .text
;guardando valor constante 0 em temporário
section .text
	mov eax, 0
	mov [qword M+0], eax
;Comando de atribuição
section .text
	mov eax,[qword M+0]
	mov [qword M+65536], eax
Rot0:
;guardando valor constante 5 em temporário
section .text
	mov eax, 5
	mov [qword M+4], eax
mov al, [qword M+65536]
mov bl, [qword M+4]
cmp al,bl
jl Rot4
mov cl, 0
mov [qword M+8], cl
jmp Rot5
Rot4
mov cl, 1
mov [qword M+8], cl
Rot5
mov al, [qword M+8]
cmp al, 1
jne Rot3
jmp Rot2
Rot1:
;guardando valor constante 1 em temporário
section .text
	mov eax, 1
	mov [qword M+9], eax
mov eax, [qword M+65536]
mov ebx, [qword M+9]
add eax,ebx
mov [qword M+13], eax
;Comando de atribuição
section .text
	mov eax,[qword M+13]
	mov [qword M+65536], eax
jmp Rot0
Rot2:
;Escrevendo na saida do terminal
mov eax, [qword M+65536]
mov rsi, M+17
mov rcx, 0
mov rdi, 0
cmp eax, 0
jge Rot6
mov bl, '-' 
mov [rsi], bl
add rsi, 1
add rdi, 1
neg eax
Rot6:
mov ebx, 10
Rot7:
add rcx, 1
cdq
idiv ebx
push dx
cmp eax, 0
jne Rot7
add rdi,rcx
Rot8:
pop dx
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rcx, 1
cmp rcx, 0
jne Rot8
mov rsi, M+17
mov rax, 1
mov rdi, 1
syscall
mov al, 0ah
mov [qword M+273], al
mov rsi, M+273
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
;guardando valor constante Cosso em temporário
section .data
	db 'Cosso', 5
section .text
;Escrevendo na saida do terminal
	mov rsi, M+65540
	mov rdx, 5
	mov rax, 1
	mov rdi, 1
	syscall
mov al, 0ah
mov [qword M+274], al
mov rsi, M+274
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
jmp Rot1
Rot3:
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
