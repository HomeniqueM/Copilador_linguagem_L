{ Exemplo de Programa na linguagem L }

integer n;

final MAXITER=10;
real percentual=.0e0;

nome[41] = "5";

write("Digite seu nome: ");
readln(nome);

for (n=1;n<=MAXITER;n=n+1) begin
  percentual=real(n)/MAXITER;
  if ( 2 == 0) writeln(percentual,": Ola' ",nome);
end