global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resb 1
;guardando valor constante 2 em temporário
section .text
	mov eax, 2
	mov [qword M+0], eax
;Comando de atribuição
section .text
	mov eax,[qword M+0]
	mov [qword M+65536], eax
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
