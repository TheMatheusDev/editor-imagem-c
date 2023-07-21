#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TODO:
- [] Sharpness (filtro)
- [] Blur (filtro)
*/ 

/**
 * @brief Imprime uma ajuda explicando como o programa deve ser usado.
 * @param program_name Nome do arquivo executável.
 */
void print_usage(char *program_name) {
  printf("Forma de usar:\n");
  printf("    %s <operacao> <imagem de entrada> <imagem de saida>\n", program_name);
  printf("Onde <operacao> pode ser:\n");
  printf("    gray: para criar uma versão em escala de cinza da imagem original.\n");
  printf("    enlarge: para ampliar em 2x a imagem original.\n");
  printf("    reduce: para reduzir em 2x a imagem original.\n");
  printf("    rotate: para rotacionar 90 graus (sentido anti-horário) a imagem original.\n");
  printf("\n");
  printf("A imagem original será lida da entrada-padrão e a imagem transformada será enviada para a saída-padrão.\n\n");
}

//Tipo Pixel
typedef struct {
  int r, g, b;
} Pixel;

//Tipo Imagem
typedef struct{
  char P3[3];
  int linha; 
  int coluna; 
  int valorMaxCor;
  Pixel **pixel; //Matriz de pixels alocada dinamicamente
} Imagem;


/**
 * @brief Aloca memória para a matriz da imagem.
 * @param *img Struct onde está armazenada a imagem.
 * @return Nada - A matriz é alterada por referência, portanto não há retorno.
 */
void AlocarMatriz(Imagem *img){
	img->pixel = (Pixel**) malloc(img->linha*sizeof(Pixel*));
	if (img->pixel == NULL) {
    printf("Erro ao alocar memória para imagem!\n");
    return;
  }

	for(int i = 0; i < img->linha; i++){
		img->pixel[i] = (Pixel*) malloc(img->coluna*sizeof(Pixel));
    if (img->pixel[i] == NULL) {
      printf("Erro ao alocar memória para imagem!\n");
      return;
    }
	}
}


/**
* @brief Escreve a imagem no arquivo.ppm.
* @param *nomeArquivo Nome do arquivo a ser escrito.
* @param *imgSaida Imagem a ser escrita.
*/
int EscreverImagem(const char *nomeArquivo, Imagem *imgSaida){
  FILE *ImagemSaida = fopen(nomeArquivo, "w");

  if(ImagemSaida == NULL){
    printf("Não foi possivel abrir o arquivo! Tente novamente\n\n");
    return 0;
  }

  // Escrevendo as informações da imagem no arquivo de saída.
  fprintf(ImagemSaida, "%s\n", imgSaida->P3);
  fprintf(ImagemSaida, "%d %d\n", imgSaida->coluna, imgSaida->linha);
  fprintf(ImagemSaida, "%d\n", imgSaida->valorMaxCor);

  // Escrevendo os pixels no arquivo de saída.
  for(int i = 0; i<imgSaida->linha; i++){
    for(int j = 0; j<imgSaida->coluna; j++){
      fprintf(ImagemSaida, "%hd\n", imgSaida->pixel[i][j].r);
      fprintf(ImagemSaida, "%hd\n", imgSaida->pixel[i][j].g);
      fprintf(ImagemSaida, "%hd\n", imgSaida->pixel[i][j].b);
    }
  }

  printf("\n\nDEBUG: --- Dados da imagem escrita ---\n");
  printf("Magic number: %s\n", imgSaida->P3);
  printf("Color range: %d\n", imgSaida->valorMaxCor);
  printf("Altura: %d\n", imgSaida->linha);
  printf("Largura: %d\n\n\n", imgSaida->coluna);

  printf("Imagem escrita com sucesso!\n");
  fclose(ImagemSaida);
}


/**
* @brief Lê os dados do arquivo.ppm e armazena-os na struct img.
* @param nomeArquivo Nome do arquivo a ser lido.
* @param img A variável onde a imagem será armazenada.
*/
Imagem* LerImagem(const char *nomeArquivo) {
  Imagem *img = malloc(sizeof(Imagem));
  if(img == NULL) {
    printf("Erro ao alocar memória para imagem!\n");
    return NULL;
  }
  char magicNumber[3] = "P3";
  FILE *imagemEntrada = fopen(nomeArquivo, "r");

  if (imagemEntrada == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return NULL;
  }

  fgets(img->P3, 3, imagemEntrada);
  if(strcmp(img->P3, magicNumber) != 0){
    printf("Formato de imagem não suportado!\n");
    return NULL;
  }

  fscanf(imagemEntrada, "%d %d", &img->coluna, &img->linha);
  fscanf(imagemEntrada, "%d", &img->valorMaxCor);
  
  AlocarMatriz(img);

  //Lendo os pixels e armazenando-os na matriz.
  for(int i = 0; i<img->linha; i++){
    for(int j = 0; j<img->coluna; j++){
      fscanf(imagemEntrada, "%d", &img->pixel[i][j].r);
      fscanf(imagemEntrada, "%d", &img->pixel[i][j].g);
      fscanf(imagemEntrada, "%d", &img->pixel[i][j].b);
    }
  }

  printf("\n\nDEBUG: --- Dados da imagem lida ---\n");
  printf("Magic number: %s\n", img->P3);
  printf("Color range: %d\n", img->valorMaxCor);
  printf("Altura: %d\n", img->linha);
  printf("Largura: %d\n\n\n", img->coluna);

  printf("Imagem lida com sucesso!\n");
  fclose(imagemEntrada);
  return img;
}

/**
* @brief Rotaciona a imagem em 90 graus no sentido anti-horário.
* @param *imgEntrada A imagem a ser rotacionada.
* @returns Uma struct do tipo Imagem contendo a imagem rotacionada.
*/
Imagem *Rotacionar(Imagem *imgEntrada) {
  // Criamos uma nova imagem para não alterar a original.
  Imagem *imgSaida = malloc(sizeof(Imagem));
  if(imgSaida == NULL) {
    printf("Erro ao alocar memória para imagem!\n");
    return NULL;
  }
  strcpy(imgSaida->P3, imgEntrada->P3);
	imgSaida->coluna = imgEntrada->linha;
	imgSaida->linha = imgEntrada->coluna;
	imgSaida->valorMaxCor = imgEntrada->valorMaxCor;

  AlocarMatriz(imgSaida);

  // Recompõe a imagem rotacionada.
  for (int j = 0; j < imgEntrada->linha; j++) {
    for (int i = 0; i < imgEntrada->coluna; i++) {
      imgSaida->pixel[i][j] = imgEntrada->pixel[j][imgEntrada->coluna - i - 1];
    }
  }

  return imgSaida;
}

/**
* @brief Amplia as dimensões da imagem em 2x (dobra).
* @param *imgEntrada Imagem a ser ampliada.
* @returns Uma struct do tipo Imagem contendo a imagem com as dimensões ampliadas.
*/
Imagem *AmpliarImagem(Imagem *imgEntrada) {
  // Criamos uma nova imagem para não alterar a original.
  Imagem *imgSaida = malloc(sizeof(Imagem));
  if(imgSaida == NULL) {
    printf("Erro ao alocar memória para imagem!\n");
    return NULL;
  }
  strcpy(imgSaida->P3, imgEntrada->P3);
  imgSaida->coluna = imgEntrada->coluna * 2;
  imgSaida->linha = imgEntrada->linha * 2;
  imgSaida->valorMaxCor = imgEntrada->valorMaxCor;

  AlocarMatriz(imgSaida);

  /*
  O efeito funciona copiando o valor de cada pixel da imgEntrada para 4 pixels (2x2) na imgSaida.
  Efetivamente duplicando o tamanho da imagem.
  */
  for (int i = 0; i < imgEntrada->linha; i++) {
    for (int j = 0; j < imgEntrada->coluna; j++) {
      imgSaida->pixel[i * 2][j * 2] = imgEntrada->pixel[i][j];
      imgSaida->pixel[i * 2][j * 2 + 1] = imgEntrada->pixel[i][j];
      imgSaida->pixel[i * 2 + 1][j * 2] = imgEntrada->pixel[i][j];
      imgSaida->pixel[i * 2 + 1][j * 2 + 1] = imgEntrada->pixel[i][j];
    }
  }

  return imgSaida;
}

/**
* @brief Reduz as dimensões da imagem em 2x (pela metade).
* @param *imgEntrada Imagem a ser reduzida.
* @returns Uma struct do tipo Imagem contendo a imagem com as dimensões reduzidas.
*/
Imagem *ReduzirImagem(Imagem *imgEntrada) {
  // Criamos uma nova imagem para não alterar a original.
  Imagem *imgSaida = malloc(sizeof(Imagem));
  if(imgSaida == NULL) {
    printf("Erro ao alocar memória para imagem!\n");
    return NULL;
  }
  strcpy(imgSaida->P3, imgEntrada->P3);
  imgSaida->coluna = imgEntrada->coluna / 2;
  imgSaida->linha = imgEntrada->linha / 2;
  imgSaida->valorMaxCor = imgEntrada->valorMaxCor;

  AlocarMatriz(imgSaida);

  /*
  O efeito funciona copiando 1 pixel para a imgSaida a cada 4 (2x2) pixels da imgEntrada.
  Efetivamente reduzindo o tamanho da imagem pela metade.
  */
  for (int i = 0; i < imgEntrada->linha; i++) {
    for (int j = 0; j < imgEntrada->coluna; j++) {
      imgSaida->pixel[i / 2][j / 2] = imgEntrada->pixel[i][j];
    }
  }

  return imgSaida;
}

/**
* @brief Aplica um efeito de escala de cinza na imagem.
* @param *imgEntrada Imagem a ter o efeito aplicado.
* @return Uma struct do tipo Imagem contendo a imagem com o efeito aplicado.
*/
Imagem *EscalaDeCinza(Imagem *imgEntrada) {
  // Criamos uma nova imagem para não alterar a original.
 Imagem *imgSaida = malloc(sizeof(Imagem));
  if(imgSaida == NULL) {
    printf("Erro ao alocar memória para imagem!\n");
    return NULL;
  }

  strcpy(imgSaida->P3, imgEntrada->P3);
  imgSaida->coluna = imgEntrada->coluna;
  imgSaida->linha = imgEntrada->linha;
  imgSaida->valorMaxCor = imgEntrada->valorMaxCor;

  AlocarMatriz(imgSaida);

  /*
  O filtro funciona ao definir o valor de cada pixel como a média dos valores de R, G e B.
  Isso reduz as cores dos pixels e mantém a luminosidade.
  */ 
  for (int i = 0; i < imgEntrada->linha; i++) {
    for (int j = 0; j < imgEntrada->coluna; j++) {
      int gray = (imgEntrada->pixel[i][j].r + imgEntrada->pixel[i][j].g + imgEntrada->pixel[i][j].b) / 3;
      imgSaida->pixel[i][j].r = gray;
      imgSaida->pixel[i][j].g = gray;
      imgSaida->pixel[i][j].b = gray;
    }
  }

  return imgSaida;
}

/**
 * @brief Libera a memória alocada para a matriz de pixels da imagem e a imagem.
 * @param *img Imagem a ter a memória liberada.
 */
void LimparMemoria(Imagem *img){
	for(int i = 0; i < img->linha; i++){
		free(img->pixel[i]);
	}
	free(img->pixel);
  free(img);
}

/**
 * @brief Função principal: ponto de partida do programa.
 * @param argc Número de argumentos passados para o programa.
 * @param argv Array de strings contendo os argumentos passados para o programa.
 * @example ./main rotate ./imgs/galinhos.ppm ./saida.ppm
 */
int main(int argc, char* argv[]) {
  if (argc != 4) {
    print_usage(argv[0]);
    return 0;
  }

  if (strcmp(argv[1], "rotate") != 0
      && strcmp(argv[1], "enlarge") != 0
      && strcmp(argv[1], "reduce") != 0
      && strcmp(argv[1], "gray") != 0) {
    print_usage(argv[0]);
    return 0;
  }

  Imagem *imgEntrada = LerImagem(argv[2]);
  Imagem *imgSaida;
  if (imgEntrada == NULL) {
    printf("Ocorreu um erro e nenhuma operaçao foi realizada!\n");
    return 0;
  }


  if (strcmp(argv[1], "rotate") == 0) {
    imgSaida = Rotacionar(imgEntrada);
    EscreverImagem(argv[3], imgSaida);
  } 

  if (strcmp(argv[1], "enlarge") == 0) {
    imgSaida = AmpliarImagem(imgEntrada);
    EscreverImagem(argv[3], imgSaida);
  }

  if (strcmp(argv[1], "reduce") == 0) {
    imgSaida = ReduzirImagem(imgEntrada);
    EscreverImagem(argv[3], imgSaida);
  }

  if (strcmp(argv[1], "gray") == 0) {
    imgSaida = EscalaDeCinza(imgEntrada);
    EscreverImagem(argv[3], imgSaida);
  }

  LimparMemoria(imgEntrada);
  LimparMemoria(imgSaida);
  return 0;
}
