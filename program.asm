global _start    ; Ponto inicial do programa
section .data    ; Sessão de dados
M:                ; Rótulo para demarcar o
                ; início da sessão de dados
resb 65536        ; Reserva de temporários
                ; inicio da declaracao das variaveis
resb 4        ; < x > em 65536
                ; ultima pos. disponivel:  65540

section .text    ; Sessão de código
_start:            ; Início do programa
     ; atribuindo valor a variaveis declaradas...
MOV EAX, 0 
MOV [qword M+65536], EAX     ; atribui no endereco
    ; fim das atribuicoes de declaracao 🙂

; Halt - fim do programa! ------------
MOV RAX, 60 ; Chamada de saída 
MOV RDI, 0 ; Código de saida sem erros 
syscall ; Chama o kernel