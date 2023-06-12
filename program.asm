global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .text
;guardando valor constante 2.0 em temporário
section .data
	dd 2.0
section .text
;guardando valor constante 3.0 em temporário
section .data
	dd 3.0
section .text
mov al, [qword M+65536]
mov bl, [qword M+65540]
cmp al,bl
jl Rot2
mov cl, 0
mov [qword M+0], cl
jmp Rot3
Rot2:
mov cl, 1
mov [qword M+0], cl
Rot3:
mov al, [qword M + 0]
cmp al, 0
je Rot0
;guardando valor constante teste em temporário
section .data
	db 'teste', 5
section .text
;Escrevendo na saida do terminal
	mov rsi, M+65544
	mov rdx, 5
	mov rax, 1
	mov rdi, 1
	syscall
mov al, 0ah
mov [qword M+1], al
mov rsi, M+1
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
Rot0:
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
