global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resd 1
section .data
;Declaração de Variável
	resb 1
;guardando valor constante a em temporário
section .text
	mov al, 'a'
	mov [qword M+0], al
;Comando de atribuição
section .text
	mov al,[qword M+0]
	mov [qword M+65540], al
section .data
;Declaração de constante(final)
	dd 10
section .data
;Declaração de Variável
	resd 1
;guardando valor constante 0.5 em temporário
section .data
	dd 0.5
section .text
;Comando de atribuição
section .text
	movss xmm0,[qword M+65549]
	movss [qword M+65545], xmm0
section .text
;guardando valor constante 0.2 em temporário
section .data
	dd 0.2
section .text
;Comando de atribuição
section .text
	movss xmm0,[qword M+65553]
	movss [qword M+65545], xmm0
section .text
;guardando valor constante 1 em temporário
section .text
	mov eax, 1
	mov [qword M+1], eax
;Comando de atribuição
section .text
	mov eax,[qword M+1]
	mov [qword M+65536], eax
Rot0:
mov al, [qword M+65536]
mov bl, [qword M+65541]
cmp al,bl
jle Rot4
mov cl, 0
mov [qword M+5], cl
jmp Rot5
Rot4
mov cl, 1
mov [qword M+5], cl
Rot5
mov al, [qword M+5]
cmp al, 1
jne Rot3
jmp Rot2
Rot1:
;guardando valor constante 1 em temporário
section .text
	mov eax, 1
	mov [qword M+6], eax
mov eax, [qword M+65536]
mov ebx, [qword M+6]
add eax,ebx
mov [qword M+10], eax
;Comando de atribuição
section .text
	mov eax,[qword M+10]
	mov [qword M+65536], eax
jmp Rot0
Rot2:
mov rax, [qword M+65536]
cvtsi2ss xmm0, rax
movss [qword M+14], xmm0
movss xmm0, [qword M+14]
mov rax, [qword M+65541]
cvtsi2ss xmm1, rax
divss xmm0, xmm1
movss [qword M+18], xmm0
;Comando de atribuição
section .text
	movss xmm0,[qword M+18]
	movss [qword M+65545], xmm0
;Escrevendo na saida do terminal
mov eax, [qword M+65536]
mov rsi, M+22
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
mov rsi, M+22
mov rax, 1
mov rdi, 1
syscall
mov al, 0ah
mov [qword M+278], al
mov rsi, M+278
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
;guardando valor constante 2 em temporário
section .text
	mov eax, 2
	mov [qword M+279], eax
mov eax, [qword M+65536]
mov ebx, [qword M+65536]
idiv ebx
mov [qword M+283], edx
;guardando valor constante 0 em temporário
section .text
	mov eax, 0
	mov [qword M+287], eax
mov al, [qword M+283]
mov bl, [qword M+287]
cmp al,bl
je Rot11
mov cl, 0
mov [qword M+291], cl
jmp Rot12
Rot11
mov cl, 1
mov [qword M+291], cl
Rot12
mov al, [qword M + 291]
cmp al, 0
je Rot9
;Escrevendo na saida do terminal
movss xmm0, [qword M+65545]
mov rsi, M+292
mov rcx, 0
mov rdi, 6
mov rbx, 10 
cvtsi2ss xmm2, rbx
subss xmm1, xmm1
comiss xmm0, xmm1
jae Rot13
mov dl, '-'
mov [rsi], dl
mov rdx, -1
cvtsi2ss xmm1, rdx
mulss xmm0, xmm1
add rsi, 1
Rot13:
roundss xmm1, xmm0, 0b0011
subss xmm0, xmm1
cvtss2si rax, xmm1
Rot14:
add rcx, 1
cdq
idiv ebx
push dx
cmp eax, 0
jne Rot14
sub rdi, rcx
Rot15:
pop dx
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rcx, 1
cmp rcx, 0
jne Rot15
mov dl, '.'
mov [rsi], dl
add rsi, 1
Rot16:
cmp rdi, 0
jle Rot17
mulss xmm0,xmm2
roundss xmm1,xmm0,0b0011
subss xmm0,xmm1
cvtss2si rdx, xmm1
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rdi, 1
jmp Rot16
Rot17:
mov dl, 0
mov [rsi], dl
mov rdx, rsi
mov rbx, M+292
sub rdx, rbx
mov rsi, M+292
mov rax, 1
mov rdi, 1
syscall
;guardando valor constante : Ola  em temporário
section .data
	db ': Ola ', 6
section .text
;Escrevendo na saida do terminal
	mov rsi, M+65557
	mov rdx, 6
	mov rax, 1
	mov rdi, 1
	syscall
;Escrevendo na saida do terminal
mov rsi, M+65540
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
mov al, 0ah
mov [qword M+548], al
mov rsi, M+548
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
Rot9:
jmp Rot1
Rot3:
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
