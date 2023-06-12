global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resb 100h
;guardando valor constante 123456789012341567890123456789 em temporário
section .data
	db '123456789012341567890123456789', 30
section .text
;Comando de atribuição
section .text
	mov rsi, qword M+65536
	mov rdi, qword M+65792
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
	mov rdx, 30
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
