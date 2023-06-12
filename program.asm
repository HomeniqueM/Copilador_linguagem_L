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
;guardando valor constante 3 em temporário
section .text
	mov eax, 3
	mov [qword M+0], eax
mov ebx, [qword M+0]
mov eax, -1
imul ebx
mov [qword M+4], eax
;guardando valor constante 5 em temporário
section .text
	mov eax, 5
	mov [qword M+8], eax
mov eax, [qword M+4]
mov ebx, [qword M+8]
add eax,ebx
mov [qword M+12], eax
mov rax, [qword M+12]
cvtsi2ss xmm0, rax
movss [qword M+16], xmm0
;Comando de atribuição
section .text
	movss xmm0,[qword M+16]
	movss [qword M+65536], xmm0
section .text
;Escrevendo na saida do terminal
movss xmm0, [qword M+65536]
mov rsi, M+20
mov rcx, 0
mov rdi, 6
mov rbx, 10 
cvtsi2ss xmm2, rbx
subss xmm1, xmm1
comiss xmm0, xmm1
jae Rot0
mov dl, '-'
mov [rsi], dl
mov rdx, -1
cvtsi2ss xmm1, rdx
mulss xmm0, xmm1
add rsi, 1
Rot0:
roundss xmm1, xmm0, 0b0011
subss xmm0, xmm1
cvtss2si rax, xmm1
Rot1:
add rcx, 1
cdq
idiv ebx
push dx
cmp eax, 0
jne Rot1
sub rdi, rcx
Rot2:
pop dx
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rcx, 1
cmp rcx, 0
jne Rot2
mov dl, '.'
mov [rsi], dl
add rsi, 1
Rot3:
cmp rdi, 0
jle Rot4
mulss xmm0,xmm2
roundss xmm1,xmm0,0b0011
subss xmm0,xmm1
cvtss2si rdx, xmm1
add dl, '0'
mov [rsi], dl
add rsi, 1
sub rdi, 1
jmp Rot3
Rot4:
mov dl, 0
mov [rsi], dl
mov rdx, rsi
mov rbx, M+20
sub rdx, rbx
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
