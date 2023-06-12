global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resd 1
;guardando valor constante 5 em temporário
section .text
	mov eax, 5
	mov [qword M+0], eax
;Comando de atribuição
section .text
	mov eax,[qword M+0]
	mov [qword M+65536], eax
section .data
;Declaração de Variável
	resd 1
;guardando valor constante 2 em temporário
section .text
	mov eax, 2
	mov [qword M+4], eax
;Comando de atribuição
section .text
	mov eax,[qword M+4]
	mov [qword M+65540], eax
section .data
;Declaração de Variável
	resd 1
section .text
mov al, [qword M+65536]
mov bl, [qword M+65540]
cmp al,bl
jl Rot2
mov cl, 0
mov [qword M+8], cl
jmp Rot3
Rot2
mov cl, 1
mov [qword M+8], cl
Rot3
mov al, [qword M + 8]
cmp al, 0
je Rot0
;guardando valor constante 0 em temporário
section .text
	mov eax, 0
	mov [qword M+9], eax
;Comando de atribuição
section .text
	mov eax,[qword M+9]
	mov [qword M+65544], eax
;guardando valor constante 45657 em temporário
section .text
	mov eax, 45657
	mov [qword M+13], eax
;Escrevendo na saida do terminal
mov eax, [qword M+13]
mov rsi, M+17
mov rcx, 0
mov rdi, 0
cmp eax, 0
jge Rot4
mov bl, '-' 
mov [rsi], bl
add rsi, 1
add rdi, 1
neg eax
Rot4:
mov ebx, 10
Rot5:
add rcx, 1
cdq
idiv ebx
push dx
cmp eax, 0
jne Rot5
add rdi,rcx
Rot6:
pop dx
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rcx, 1
cmp rcx, 0
jne Rot6
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
jmp Rot1
Rot0:
;guardando valor constante 1 em temporário
section .text
	mov eax, 1
	mov [qword M+274], eax
;Comando de atribuição
section .text
	mov eax,[qword M+274]
	mov [qword M+65544], eax
;guardando valor constante 45657 em temporário
section .text
	mov eax, 45657
	mov [qword M+278], eax
;Escrevendo na saida do terminal
mov eax, [qword M+278]
mov rsi, M+282
mov rcx, 0
mov rdi, 0
cmp eax, 0
jge Rot7
mov bl, '-' 
mov [rsi], bl
add rsi, 1
add rdi, 1
neg eax
Rot7:
mov ebx, 10
Rot8:
add rcx, 1
cdq
idiv ebx
push dx
cmp eax, 0
jne Rot8
add rdi,rcx
Rot9:
pop dx
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rcx, 1
cmp rcx, 0
jne Rot9
mov rsi, M+282
mov rax, 1
mov rdi, 1
syscall
mov al, 0ah
mov [qword M+538], al
mov rsi, M+538
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
Rot1:
section .text
;Escrevendo na saida do terminal
mov eax, [qword M+65544]
mov rsi, M+539
mov rcx, 0
mov rdi, 0
cmp eax, 0
jge Rot10
mov bl, '-' 
mov [rsi], bl
add rsi, 1
add rdi, 1
neg eax
Rot10:
mov ebx, 10
Rot11:
add rcx, 1
cdq
idiv ebx
push dx
cmp eax, 0
jne Rot11
add rdi,rcx
Rot12:
pop dx
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rcx, 1
cmp rcx, 0
jne Rot12
mov rsi, M+539
mov rax, 1
mov rdi, 1
syscall
mov al, 0ah
mov [qword M+795], al
mov rsi, M+795
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
