#include <stdio.h>
#include <stdlib.h>
/**
 * Imprime uma ajuda explicando como o programa deve ser usado.
 * @param program_name Nome do arquivo executável.
 */
void print_usage(char *program_name) {
  printf("Forma de usar:\n");
  printf("    %s <operacao>\n", program_name);
  printf("Onde <operacao> pode ser:\n");
  printf("    gray: para criar uma versão em escala de cinza da imagem original.\n");
  printf("    enlarge: para ampliar em 2x a imagem original.\n");
  printf("    reduce: para reduzir em 2x a imagem original.\n");
  printf("    rotate: para rotacionar 90 graus (sentido horário) a imagem original.\n");
  printf("    sharp: para aplicar um filtro de sharpening na imagem original.\n");
  printf("    blur: para aplicar um filtro de blurring na imagem original.\n");
  printf("\n");
  printf("A imagem original será lida da entrada-padrão e a imagem transformada será enviada para a saída-padrão.\n\n");
}

int rotate() {

typedef struct {
    int r, g, b;
} Pixel;

Pixel** image;

int main() {
    char formato[6];
    int largura, altura, cormax;
// Teste de entrada para ver se a função desenvolvida é funcional
    printf("Type: ");
    scanf("%s", formato);
    printf("largura: ");
    scanf("%d", &largura);
    printf("altura: ");
    scanf("%d", &altura);

    // Troca natural entre as dimensões da imagem para rotacionar 90 graus
    int rotacaoLargura = altura;
    int rotacaoAltura = largura;

    // Etapa utikizada para alocação da memória de imagem rotacionada
    image = malloc(rotacaoAltura * sizeof(Pixel*));
    for (int i = 0; i < rotacaoAltura; i++) {
        image[i] = malloc(rotacaoLargura * sizeof(Pixel));
    }

    // Leitura dos pixels da imagem pré rotação
    printf("Informe os valores RGB dos pixels:\n");
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            printf("Pixel [%d][%d]:\n", i, j);
            scanf("%d", &image[rotacaoAltura - 1 - j][i].r);
            scanf("%d", &image[rotacaoAltura - 1 - j][i].g);
            scanf("%d", &image[rotacaoAltura - 1 - j][i].b);
        }
    }

    // Cabeçalho da imagem pós rotação
    printf("\nImagem rotacionada:\n");
    printf("Formato: %s\n", formato);
    printf("Largura: %d\n", rotacaoLargura);
    printf("Altura: %d\n", rotacaoAltura);

    // Saída de pixels pós rotação (teste de saída para ver se o programa está funcional)
    for (int i = 0; i < rotacaoAltura; i++) {
        for (int j = 0; j < rotacaoLargura; j++) {
            printf("Pixel [%d][%d]:\n", i, j);
            printf("R: %d\n", image[i][j].r);
            printf("G: %d\n", image[i][j].g);
            printf("B: %d\n", image[i][j].b);
        }
    }
    for (int i = 0; i < rotacaoAltura; i++) {
        free(image[i]);
    }
    free(image);
  // resquicio para teste em outra aba, lembrar de adicionar <stdio.h> e <stdlib.h>
    return 0;
}

}
/**
 * Função principal: ponto de partida do programa.
 */
int main(int argc, char* argv[]) {
  /*
    argc e argv são parâmetros padrões na função main(). Enquanto argc indica quantos
    argumentos foram passados para o programa na linha de comando, argv contém esses argumentos
    na ordemem que eles foram passados. argc será sempre >= 1, pois argv[0] terá o nome do programa.
    Por exemplo, se um programa com `meu_prog` for chamado assim:
    $ ./meu_prog abc def ghi
    argc terá o valor 4, indicando que há 4 argumentos, sendo o 1º (argv[0]) o nome do
    próprio programa ("./meu_prog"), o 2º (argv[1]) será "abc", o 3º (argv[2]) será "def"
    e, por fim, o 4º (argv[3]) será "ghi."
  */
  if (argc != 2) {
    // se não houver 2 argumentos, então o programa está sendo usado incorretamente.
    // deve-se portanto imprimir como usá-lo.
    print_usage(argv[0]);
  }
  else {
    char *operation = argv[1]; // operação passada no argumento.
    /*
    A partir daqui, você deve:
    1) ler a imagem da entrada-padrão;
    2) criar uma nova imagem em função da operação informada;
    3) enviar a imagem criada para a saída-padrão.
    Obs: faça isso organizando as operações em funções.
    */
  }

  return 0;
}
