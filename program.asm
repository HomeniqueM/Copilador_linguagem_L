global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resb 1
;Declaração de vetor
	resb 50
section .text
;guardando valor constante banana em temporário
section .data
	db "banana", 6
section .text
;Comando de atribuição
section .text
	mov rsi, qword M+65537
	mov rdi, qword M+65587
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
	mov rsi, M+65537
	mov rdx, 6
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
section .text
;fim do programa
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
