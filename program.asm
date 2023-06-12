global _start
section .data
M:
	resb 10000h ;temporários
section .text
_start:
section .data
;Declaração de Variável
	resd 1
section .text
;guardando valor constante Digite um numero inteiro:  em temporário
section .data
	db 'Digite um numero inteiro: ', 26
section .text
;Escrevendo na saida do terminal
	mov rsi, M+65540
	mov rdx, 26
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
mov rsi, M + 1
mov rdx, 100h
mov rax, 0
mov rdi, 0
syscall
mov byte [M+65536+rax-1], 0
mov rax, 0                  ;acumul. parte int.
subss xmm0,xmm0             ;acumul. parte frac.
mov rbx, 0                  ;caractere
mov rcx, 10                 ;base 10
cvtsi2ss xmm3,rcx           ;base 10
movss xmm2,xmm3             ;potência de 10
mov rdx, 1                  ;sinal
mov rsi, M+1
mov bl,[rsi]                ;carrega caractere
cmp bl, '-'                 ;sinal?
jne Rot0                    ;se dif -, salta
mov rdx, -1                 ;senão, armazena -
add rsi, 1                  ;inc. ponteiro string
mov bl, [rsi]               ;carrega caractere
Rot0:
push rdx                     ;empilha sinal
mov rdx, 0                  ;reg. multiplicação
Rot1:
cmp bl, 0Ah                     ;verifica fim string
je Rot2
cmp bl, '.'                     ;senão verifica ponto
je Rot3
imul ecx                        ;mult. eax por 10
sub bl, '0'                     ;converte caractere
add eax, ebx                    ;soma valor caractere
add rsi, 1                      ;incrementa base
mov bl, [rsi]                   ;carrega caractere
jmp Rot1
Rot3:
;calcula parte fracionária em xmm0
add rsi, 1                      ;inc. ponteiro string
cmp bl, 0Ah                     ;*verifica fim string
je Rot2
sub bl, '0'                     ;converte caractere
cvtsi2ss xmm1,rbx               ;conv real
divss xmm1,xmm2                 ;transf. casa decimal
addss xmm0,xmm1                 ;soma acumul.
mulss xmm2,xmm3                 ;atualiza potência
jmp Rot3
Rot2:
cvtsi2ss xmm1,rax               ;conv parte inteira
addss xmm0,xmm1                 ;soma parte frac.
pop rcx                         ;desempilha sinal
cvtsi2ss xmm1,rcx               ;conv sinal
mulss xmm0,xmm1                 ;mult. sinal
movss [qword M+65536], xmm0
	mov rax, 60 ;chamada de saída
	mov rdi, 0 ;código de saida sem erros
	syscall ;chama o kernel
