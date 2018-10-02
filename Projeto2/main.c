#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define FILES 50
#define MAX_GRAY_LEVEL 256
#define ILBP_MAX 512
#define METRICS 24

double ** processo_imagem(char * tipo, double ** descritor);
int ** ler_imagem(char* fileName, int *linha, int *coluna);
int* func_ILBP(int **, int, int);
double* direcao_glcm(int direcao[2], int ** matriz, int linha, int col);
double* func_GLCM(int ** matriz, int linha, int col);
double * treinamento(double ** treinamento_descritor);
double teste(double* asphalt_treinado, double* grass_treinado, double** descritor_tipo, int is_grass);

int main(int argc, char const *argv[]) {
   char *tipo = "./DataSet/asphalt/asphalt_";
   char *tipo2 = "./DataSet/grass/grass_";
   double ** asphalt_descritor;
   double ** grass_descritor;
   double * asphalt_treinamento;
   double * grass_treinamento;
   double acertos;

   srand(time(NULL));

   asphalt_descritor = processo_imagem(tipo, asphalt_descritor);
   grass_descritor = processo_imagem(tipo2, asphalt_descritor);

   asphalt_treinamento = treinamento(asphalt_descritor);
   grass_treinamento = treinamento(grass_descritor);

   acertos = teste(asphalt_treinamento, grass_treinamento, asphalt_descritor, 0);
   acertos += teste(asphalt_treinamento, grass_treinamento, grass_descritor, 1);

   acertos = (acertos * 100)/50.0;

   free(asphalt_descritor);
   free(grass_descritor);
   free(asphalt_treinamento);
   free(grass_treinamento);
   printf("Acertos: %.0lf%%\n", acertos);


   return 0;
}

//=============FUNCTIONS=============


int ** ler_imagem(char* fileName, int *linha, int *coluna) {
	FILE* file = fopen(fileName, "r");
  *linha = 0;
  *coluna = 1;
	if (file == NULL) {
		printf("Cannot open file: %s", fileName);
	}

		char count = 0;

		bool final_coluna = false;

		while (feof(file) == 0) {

			count = fgetc(file);

			if (!final_coluna && count == ';')
        (*coluna)++;

			if (count == '\n') {
				(*linha)++;
				final_coluna = true;
			}
		}



	rewind(file);

	int ** matriz = (int **) malloc((*linha)* sizeof(int*));

  for(int i = 0; i < (*linha); i++){
    matriz[i] = (int*) malloc((*coluna)*sizeof(int));
  }


	for (int i = 0; i < *linha; i++) {
		for (int j = 0; j < *coluna; j++) {
			fscanf(file, "%d;", &matriz[i][j]);
		}
	}

	fclose(file);
  printf("%s\n", fileName);
  return matriz;

}
int* func_ILBP(int** matriz, int linha, int coluna) {

	int* ilbp = malloc(512* sizeof(int));

  int matriz_secundaria[3][3];
  double media;
  unsigned short binario = 0;
  int x, y;
  int menor_num_binario;
  unsigned short extraido;

	for (int i = 1; i < linha-1; i++) {
		for (int j = 1; j < coluna-1; j++) {

      matriz_secundaria[0][0] = matriz[i-1][j-i];
      matriz_secundaria[0][1] = matriz[i-1][j];
      matriz_secundaria[0][2] = matriz[i-1][j+1];

      matriz_secundaria[1][0] = matriz[i][j-1];
      matriz_secundaria[1][1] = matriz[i][j];
      matriz_secundaria[1][2] = matriz[i][j+1];

      matriz_secundaria[2][0] = matriz[i+1][j-1];
      matriz_secundaria[2][1] = matriz[i+1][j];
      matriz_secundaria[2][2] = matriz[i+1][j+1];

			media = 0;
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					media += matriz_secundaria[x][y];
				}
			}

			media = media / 9;

			binario = 0;

			x = 0;
      y = 0;
			while (true) {
				binario = binario << 1;

				if (matriz_secundaria[x][y] >= media) {
					binario = binario | 0x0001;
				}


				if (x == 1 && y == 1) {
					break;
				}

				if ((x == 0 || x == 1) && y != 2) {
					y++;
					continue;
				}

				if (y == 2 && x != 2) {
					x++;
					y = 2;
					continue;
				}

				if (x == 2 && y != 0) {
					x = 2;
					y--;
					continue;
				}
				else {
					x = 1;
					y = 0;
				}
			}

			menor_num_binario =  (int) binario;

			for (int k = 0; k < 9; k++) {

				extraido = binario & 0x0100;
				extraido = extraido >> 8;

				binario = binario << 1;

				binario = binario | extraido;
				binario = binario & 0x01FF;

				if (binario < menor_num_binario)
					menor_num_binario = (int) binario;

			}

			*(ilbp + menor_num_binario) += 1;
		}
	}

	return ilbp;
}

double* func_GLCM(int ** matriz, int linha, int coluna) {

	double *glcm = malloc(24 * sizeof(double));

	int k = 0;
	for (int direcao_x = 0; direcao_x < 3; direcao_x++) {
		for (int direcao_y = 0; direcao_y < 3; direcao_y++) {
			if (direcao_x == 1 && direcao_y == 1) {
				continue;
			}

			int vec[2] = { direcao_x, direcao_y };

			double *metricas = direcao_glcm(vec, matriz, linha, coluna);

			for (int i = 0; i < 3; i++) {
				glcm[k] = metricas[i];
				k++;
			}

			free(metricas);
		}
	}

	return glcm;

}

double* direcao_glcm(int direcao[2], int ** matriz, int linha, int coluna) {

	direcao[0] -= 1;
	direcao[1] -= 1;

	double contraste = 0;
	double energia = 0;
	double homogeneidade = 0;

	double* glcm_matrix = (double *) calloc(pow(MAX_GRAY_LEVEL, 2), sizeof(double));

	for (int x = 0; x < linha; x++) {
		for (int y = 0; y < coluna; y++) {

			int j = matriz[x][y];
			if (x + direcao[0] < 0 || x + direcao[0] >= linha || y + direcao[1] < 0 || y + direcao[1] >= coluna )
				continue;

			int i = matriz[x + direcao[0]][y + direcao[1]];

			glcm_matrix[(i*MAX_GRAY_LEVEL) + j] += 1;
		}
	}

	for (int i = 0; i < MAX_GRAY_LEVEL; i++) {
		for (int j = 0; j < MAX_GRAY_LEVEL; j++) {

			double valor_atual = glcm_matrix[(i*MAX_GRAY_LEVEL) + j];

			contraste += pow(abs(i - j), 2) * valor_atual;
			energia += pow(valor_atual, 2);
			homogeneidade += valor_atual / (1 + abs(i - j));
		}
	}

	double *metricas = calloc(3, sizeof(double));

	metricas[0] = contraste;
	metricas[1] = energia;
	metricas[2] = homogeneidade;

	free(glcm_matrix);

	return metricas;
}

double ** processo_imagem(char * tipo, double ** descritor){
  char *extensao = ".txt";
  char fileName[strlen(tipo)+strlen(extensao)+3];
  int aleatorio[FILES];
  int ** matrix[FILES];
  int matrix_linha[FILES];
  int matrix_coluna[FILES];
  int * ilbp[FILES];
  double * glcm[FILES];

  descritor = malloc(FILES * sizeof(double *));

  for (size_t i = 0; i < FILES; i++) {
    do{
      aleatorio[i] = rand()%51;
      for (size_t j = 0; j < i; j++) {
        if (aleatorio[j] == aleatorio[i]) {
          aleatorio[i] = 0;
        }
      }
    }while(aleatorio[i] == 0);

    snprintf( fileName, sizeof( fileName ), "%s%.2d%s", tipo, aleatorio[i], extensao );

    matrix[i] = ler_imagem(fileName, &matrix_linha[i], &matrix_coluna[i]);
    ilbp[i] = func_ILBP(matrix[i], matrix_linha[i], matrix_coluna[i]);
    glcm[i] = func_GLCM(matrix[i], matrix_linha[i], matrix_coluna[i]);

    descritor[i] = calloc((ILBP_MAX  + METRICS), sizeof(double));

		for (int j = 0; j < (ILBP_MAX + 24); j++) {

			descritor[i][j] = (j < ILBP_MAX) ? (double) ilbp[i][j] : (double) glcm[i][j - ILBP_MAX];
		}

    double max = descritor[i][0];

    for(int b = 0; b < ILBP_MAX  + METRICS; b++){
        if(descritor[i][b] > max){
            max = descritor[i][b];
        }
    }

    double min = descritor[i][0];

    for(int c = 0; c < ILBP_MAX  + METRICS; c++){
        if(descritor[i][c] < min){
            min = descritor[i][c];
        }
    }

    for(int k = 0; k < ILBP_MAX  + METRICS; k++){
        descritor[i][k] = ( descritor[i][k] - min) / (max - min);
    }
    free(matrix[i]);
    free(ilbp[i]);
    free(glcm[i]);
  }
  return descritor;
}

double * treinamento(double ** treinamento_descritor){
  double * vector = calloc(ILBP_MAX + METRICS, sizeof(double));

  for(int i = 0; i < ILBP_MAX  + METRICS; i++){
      for(int j = 0; j < FILES/2; j++){
          vector[i] += treinamento_descritor[j][i];
      }
      vector[i] /= FILES/2;
  }

  return vector;
}

double teste(double* asphalt_treinado, double* grass_treinado, double** descritor_tipo, int is_grass){

    double distancia_asphalt = 0;
    double distancia_grass = 0;
    double acertos = 0;
    double aux = 0;
    for(int i = 0; i < 25; i++){

        distancia_grass = 0;
        distancia_asphalt = 0;

        for(int j = 0; j < ILBP_MAX  + METRICS; j++){
            distancia_asphalt  += pow(asphalt_treinado[i] - descritor_tipo[i+25][j], 2); //taking the last 25
            distancia_grass  += pow(grass_treinado[i] - descritor_tipo[i+25][j], 2);
        }

        distancia_asphalt = sqrt(distancia_asphalt);
        distancia_grass = sqrt(distancia_grass);

        int resultado_teste = (distancia_grass < distancia_asphalt) ? 1 : 0;

        if(resultado_teste == is_grass){
          acertos++;
        }else{
          aux++;
        }
    }
    is_grass ? printf("Falso negativo: %.0lf%%\n", aux) : printf("Falso positivo: %.0lf%%\n", (aux * 100)/50);
    return acertos;
}
