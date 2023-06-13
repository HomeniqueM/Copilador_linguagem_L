global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resb 1
section .text
;Leitura do teclado
mov rsi, M + 0
mov rdx, 1
mov rax, 0
mov rdi, 0
syscall
mov al,[qword M+0]
mov [qword M+65536], al
section .text
;Escrevendo na saida do terminal
mov rsi, M+65536
mov rdx, 1
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
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
