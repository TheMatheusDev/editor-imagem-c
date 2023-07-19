#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TODO:
- [X] Liberar as memorias das matrizes
- [X] Fazer a ampliação da imagem
- [X] Fazer a redução da imagem
- [X] Gray (filtro)
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
  printf("    rotate: para rotacionar 90 graus (sentido horário) a imagem original.\n");
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
  Pixel **pixel; //Matriz de pixels.
} Imagem;


/**
 * @brief Aloca memória para a matriz da imagem. O magic code (P3) e as dimensões não são dinamicamente alocadas.
 * @param *img Struct onde está armazenada a imagem.
 * @return Nada - A matriz é alterada por referência, portanto não há retorno.
 */
void AlocarMatriz(Imagem *img){
	img->pixel = (Pixel**) malloc(img->linha*sizeof(Pixel*));
	
	for(int i = 0; i < img->linha; i++){
		img->pixel[i] = (Pixel*) malloc(img->coluna*sizeof(Pixel));
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

  printf("Imagem escrita com sucesso!\n");
  fclose(ImagemSaida);
}


/**
* @brief Lê os dados do arquivo.ppm e armazena-los na struct img.
* @param nomeArquivo Nome do arquivo a ser lido.
* @param img A imagem em si.
*/
int LerImagem(const char *nomeArquivo, Imagem *img) {
	char magicNumber[3] = "P3";
	FILE *imagemEntrada = fopen(nomeArquivo, "r");

  if (imagemEntrada == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return 0;
  }

  fgets(img->P3, 3, imagemEntrada);
  if(strcmp(img->P3, magicNumber) != 0){
    printf("Formato de imagem não suportado!\n");
    return 0;
  }

  fscanf(imagemEntrada, "%d %d", &img->coluna, &img->linha);
  fscanf(imagemEntrada, "%d", &img->valorMaxCor);
  
  //Alocando espaço necessário para os pixels.
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
  printf("Row size: %d\n", img->linha);
  printf("Column size: %d\n\n\n", img->coluna);

  printf("Imagem lida com sucesso!\n");
  fclose(imagemEntrada);
}

/**
* @brief Rotaciona a imagem em 90 graus no sentido anti-horário.
* @param *imgEntrada A imagem de saida.
* @returns Uma struct do tipo Imagem com a imagem rotacionada.
*/
Imagem Rotacionar(Imagem *imgEntrada) {
  // Necessário pois a imagem rotacionada terá dimensões diferentes da original
  Imagem imgSaida;
  strcpy(imgSaida.P3, imgEntrada->P3);
	imgSaida.coluna = imgEntrada->linha;
	imgSaida.linha = imgEntrada->coluna;
	imgSaida.valorMaxCor = imgEntrada->valorMaxCor;

  AlocarMatriz(&imgSaida);

  // Recompõe a imagem rotacionada
  for (int j = 0; j < imgEntrada->linha; j++) {
    for (int i = 0; i < imgEntrada->coluna; i++) {
      imgSaida.pixel[i][j] = imgEntrada->pixel[j][imgEntrada->coluna - i - 1];
    }
  }

  return imgSaida;
}

/**
* @brief Amplia as dimensões da imagem em 2x (dobra).
* @param *imgEntrada Imagem a ser ampliada.
* @returns Uma struct do tipo Imagem com as dimensões ampliadas.
*/
Imagem AmpliarImagem(Imagem *imgEntrada) {
  // Necesário pois a imagem ampliada terá dimensões diferentes da original
  Imagem imgSaida;
  strcpy(imgSaida.P3, imgEntrada->P3);
  imgSaida.coluna = imgEntrada->coluna * 2;
  imgSaida.linha = imgEntrada->linha * 2;
  imgSaida.valorMaxCor = imgEntrada->valorMaxCor;

  AlocarMatriz(&imgSaida);

  for (int i = 0; i < imgEntrada->linha; i++) {
    for (int j = 0; j < imgEntrada->coluna; j++) {
      imgSaida.pixel[i * 2][j * 2] = imgEntrada->pixel[i][j];
      imgSaida.pixel[i * 2][j * 2 + 1] = imgEntrada->pixel[i][j];
      imgSaida.pixel[i * 2 + 1][j * 2] = imgEntrada->pixel[i][j];
      imgSaida.pixel[i * 2 + 1][j * 2 + 1] = imgEntrada->pixel[i][j];
    }
  }

  return imgSaida;
}

/**
* @brief Reduz as dimensões da imagem em 2x (pela metade).
* @param *imgEntrada Imagem a ser ampliada.
* @returns Uma struct do tipo Imagem com as dimensões reduzidas.
*/
Imagem ReduzirImagem(Imagem *imgEntrada) {
  // Necesário pois a imagem reduzida terá dimensões diferentes da original
  Imagem imgSaida;
  strcpy(imgSaida.P3, imgEntrada->P3);
  imgSaida.coluna = imgEntrada->coluna / 2;
  imgSaida.linha = imgEntrada->linha / 2;
  imgSaida.valorMaxCor = imgEntrada->valorMaxCor;

  AlocarMatriz(&imgSaida);

  for (int i = 0; i < imgEntrada->linha; i++) {
    for (int j = 0; j < imgEntrada->coluna; j++) {
      imgSaida.pixel[i / 2][j / 2] = imgEntrada->pixel[i][j];
    }
  }

  return imgSaida;
}

/**
* @brief Aplica um efeito de escala de cinza na imagem.
* @param *imgEntrada Imagem a ter o efeito aplicado.
* @return Uma struct do tipo Imagem com o efeito aplicado.
*/
Imagem EscalaDeCinza(Imagem *imgEntrada) {
  Imagem imgSaida;
  strcpy(imgSaida.P3, imgEntrada->P3);
  imgSaida.coluna = imgEntrada->coluna;
  imgSaida.linha = imgEntrada->linha;
  imgSaida.valorMaxCor = imgEntrada->valorMaxCor;

  AlocarMatriz(&imgSaida);

  for (int i = 0; i < imgEntrada->linha; i++) {
    for (int j = 0; j < imgEntrada->coluna; j++) {
      int gray = (imgEntrada->pixel[i][j].r + imgEntrada->pixel[i][j].g + imgEntrada->pixel[i][j].b) / 3;
      imgSaida.pixel[i][j].r = gray;
      imgSaida.pixel[i][j].g = gray;
      imgSaida.pixel[i][j].b = gray;
    }
  }

  return imgSaida;
}

/**
 * @brief Libera a memória alocada para a matriz de pixels da imagem.
 * @param *img Struct onde está armazenada a imagem a ser liberada. 
 */
void LimparMemoria(Imagem *img){
	for(int i = 0; i < img->linha; i++){
		free(img->pixel[i]);
	}
	free(img->pixel);
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

  Imagem imgEntrada; // A estrct que armazena a imagem de entrada
  LerImagem(argv[2], &imgEntrada);

  if (strcmp(argv[1], "rotate") == 0) {
    Imagem imgSaida = Rotacionar(&imgEntrada);
    EscreverImagem(argv[3], &imgSaida);
  } 

  if (strcmp(argv[1], "enlarge") == 0) {
    Imagem imgSaida = AmpliarImagem(&imgEntrada);
    EscreverImagem(argv[3], &imgSaida);
  }

  if (strcmp(argv[1], "reduce") == 0) {
    Imagem imgSaida = ReduzirImagem(&imgEntrada);
    EscreverImagem(argv[3], &imgSaida);
  }

  if (strcmp(argv[1], "gray") == 0) {
    Imagem imgSaida = EscalaDeCinza(&imgEntrada);
    EscreverImagem(argv[3], &imgSaida);
  }

  LimparMemoria(&imgEntrada);
  return 0;
}
