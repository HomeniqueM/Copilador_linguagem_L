global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resb 100h
section .text
;Leitura do teclado
mov rsi, M + 65536
mov rdx, 100h
mov rax, 0
mov rdi, 0
syscall
mov byte [M+65536+rax-1], 0
	mov rsi, qword M+65536
	mov rdi, qword M+0
Rot0:
	mov al, [rdi]
	mov [rsi], al
	cmp al, 0
	je Rot1
	add rdi, 1
	add rsi, 1
	jmp Rot0
Rot1:
section .text
;Escrevendo na saida do terminal
	mov rsi, M+65536
	mov rdx, 256
	mov rax, 1
	mov rdi, 1
	syscall
mov al, 0ah
mov [qword M+256], al
mov rsi, M+256
mov rdx, 1
mov rax, 1
mov rdi, 1
syscall
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
