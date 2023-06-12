{ Exemplo de Programa na linguagem L }

integer n;
char nome = 'a';
final MAXITER=10;
real percentual=5;

percentual=3;


for (n=1;n<=MAXITER;n=n+1) begin
  percentual=real(n)/MAXITER;
  writeln(n);
  if (n mod 2 == 0) writeln(percentual,": Ola' ",nome);
end