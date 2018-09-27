// Estrutura de Dados 1 - Turma B - Prof. Mateus Menselson
// André Lucas - 15/0005563
// Anderson Chaves - 15/0005466
// Caio Brandão - 17/0007413
// Problema 03

#include <stdio.h>
#include <stdlib.h>

void TorreHanoi(char origem, char destino, char auxiliar, int discos);

int main(int argc, char const *argv[]) {
  int discos;

  printf("\nDigite a quantidade de discos: ");
  scanf("%d", &discos);

  system("clear");

  TorreHanoi('A', 'C', 'B', discos);

  return 0;
}

void TorreHanoi(char origem, char destino, char auxiliar, int discos){
  if( discos == 1 )
    printf("Mover disco de %c para %c\n", origem, destino);
  else{
    TorreHanoi(origem, auxiliar, destino, discos-1);
    TorreHanoi(origem, destino, auxiliar, 1);
    TorreHanoi(auxiliar, destino, origem, discos-1);
  }
}
