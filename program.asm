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
	resb 2
section .text
;guardando valor constante banana em temporário
section .data
	db "banana", 6
section .text
;Comando de atribuição
section .text
	mov al,[qword M+65539]
	mov [qword M+65537], al
section .text
;Escrevendo na saida do terminal
	mov rsi, M+65537
	mov rdx, 0
	mov rax, 1
	mov rdi, 1
	syscall
section .text
;fim do programa
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
