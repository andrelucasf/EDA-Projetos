// Estrutura de Dados 1 - Turma B - Prof. Mateus Menselson
// André Lucas - 15/0005563
// Anderson Chaves - 15/0005466
// Caio Brandão - 17/0007413
// Problema 02

#include <stdio.h>
#include <stdlib.h>
#define MAX 10

int *recebe_notas(float *, int , int *);
void conta_notas(int *, int , int *, int *);
int percent_aprov(int *, int *, int *, int *, int );


int main(int argc, char const *argv[]) {

  float notas[MAX];
  int i, *resultado, turma, apr[MAX], aprovados=0, reprovados=0, p_aprovados=0, p_reprovados=0;

  printf("Digite dez notas: \n");
  for (i = 0; i < MAX; i++) {
    scanf("%f", &notas[i]);
  }

  system("clear");
  resultado = recebe_notas(notas, MAX, apr);
  conta_notas(resultado, MAX, &aprovados, &reprovados);
  turma = percent_aprov(&aprovados, &reprovados, &p_aprovados, &p_reprovados, MAX);

  printf("Quantidade de aprovados: %d\n", aprovados);
  printf("Quantidade de reprovados: %d\n", reprovados);
  printf("Percentual de aprovados: %d\n", p_aprovados);
  printf("Percentual de reprovados: %d\n", p_reprovados);

  if(turma == 1)
    printf("Mais da metade da turma aprovada!\n");
  else
    printf("Mais da metade da turma reprovada!\n");

  return 0;
}

int *recebe_notas(float *notas, int n, int *apr){
  int i;

  for (i = 0; i < n; i++) {
    if (notas[i] >= 6.0) {
      apr[i] = 1;
    } else {
      apr[i] = 0;
    }
  }
  return apr;
}

void conta_notas(int *apr, int n, int *naprovados, int *nreprovados){
  int i;

  for (i = 0; i < n; i++) {
    if (apr[i] == 1) {
      *naprovados += 1;
    } else {
      *nreprovados += 1;
    }
  }
}

int percent_aprov(int *naprov, int *nreprov, int *p_aprov, int *p_reprov, int n){
  *p_aprov = (*naprov * 100) / n;
  *p_reprov = (*nreprov * 100) / n;

  if (*p_aprov >= 51) {
    return 1;
  } else {
    return 0;
  }

}
