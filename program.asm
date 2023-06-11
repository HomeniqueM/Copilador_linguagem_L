global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de constante(final)
	dd 2
section .data
;Declaração de constante(final)
	db 'cocococo', 8
section .data
;Declaração de constante(final)
	db 'g' 1
section .text
;Escrevendo na saida do terminal
mov rsi, M+65548
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
;Escrevendo na saida do terminal
mov rsi, M+65548
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
mov al, 0ah
mov [qword M+0], al
mov rsi, M+0
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
