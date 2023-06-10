global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
	db 'b' 1
section .text
mov rsi, M+65536
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
