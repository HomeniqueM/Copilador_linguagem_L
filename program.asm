global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .text
;guardando valor constante 2 em temporário
section .text
	mov eax, 2
	mov [qword M+0], eax
;guardando valor constante 3 em temporário
section .text
	mov eax, 3
	mov [qword M+4], eax
mov eax, [qword M+0]
mov ebx, [qword M+4]
cmp eax, ebx
jg Rot2
mov cl, 0
mov [qword M+8], cl
jmp Rot3
Rot2:
mov cl, 1
mov [qword M+8], cl
Rot3:
mov al, [qword M + 8]
cmp al, 0
je Rot0
;guardando valor constante teste em temporário
section .data
	db 'teste', 5
section .text
;Escrevendo na saida do terminal
	mov rsi, M+65536
	mov rdx, 5
	mov rax, 1
	mov rdi, 1
	syscall
mov al, 0ah
mov [qword M+9], al
mov rsi, M+9
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
Rot0:
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
