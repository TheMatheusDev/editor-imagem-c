#include <stdio.h>

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
