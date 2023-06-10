global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
	resb 100h
section .text
section .data
db 'Banana', 6
section .text
mov rsi, qword M+65536
mov rdi, qword M+65792
Rot0:
mov al, [rdi]
mov [rsi],al
cmp al, '' 
je Rot1
add rdi, 1
add rsi, 1
jmp Rot0
Rot1:
	mov rsi, M+65536
	mov rdx, 6
	mov rax, 1
	mov rdi, 1
	syscall
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
