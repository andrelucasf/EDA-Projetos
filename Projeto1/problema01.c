// Estrutura de Dados 1 - Turma B - Prof. Mateus Menselson
// André Lucas - 15/0005563
// Anderson Chaves - 15/0005466
// Caio Brandão - 17/0007413
// Problema 01

#include <stdio.h>
#include <stdlib.h>
#define MAX 10

int fneuronio(int *ve, int *vp, int l, int tamanho);

int main(int argc, char const *argv[]) {

  int i, entradas[MAX], pesos[MAX], limiar, fneu;

  printf("Digite dez entradas: \n");
  for (i = 0; i < MAX; i++) {
    scanf("%d", &entradas[i]);
  }
  printf("Digite dez pesos: \n");
  for (i = 0; i < MAX; i++) {
    scanf("%d", &pesos[i]);
  }
  printf("Digite o valor do limiar T: \n");
  scanf("%d", &limiar);

  fneu = fneuronio(entradas, pesos, limiar, MAX);
  system("clear");

  if(fneu==1)
    printf("Neurônio ativado!");
  else
    printf("Neurônio inibido!");

  return 0;
}

int fneuronio(int *ve, int *vp, int l, int tamanho){
  int somap = 0, i;
  for (i = 0; i < MAX; i++) {
    somap=somap+(ve[i] * vp[i]);
  }
  if (somap > l)
    return 1;
  else
    return 0;
}
