{ Exemplo de Programa na linguagem L }

integer A,A3h3A;
char A1[40];
final MAXITER=10;
real percentual=.0e0;

write("Digite seu nome: ");
readln(nome);

for (n=1;n<=MAXITER;n=n+1) begin
  percentual=real(n)/MAXITER;
  if (n mod 2 == 0) writeln(percentual,": Ola' ",nome);
end
