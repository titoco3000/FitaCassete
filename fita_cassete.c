/*
    A entrada é uma arquivo de texto cujo primeiro numero indica a quantidade de testes
    que contém. Para cada teste, o primeiro numero indica o tempo total da fita (lado A+B)
    e o segundo, a quantidade de músicas. As proximas linhas indicam os tempos da musica,
    com os minutos seguidos dos segundos. Se foi possível organizar as músicas no espaço
    dado, imprime os subconjuntos de cada lado; se não, imprime que não foi possível.

    Tito Guidotti

    13/9/2022
*/
#include <stdio.h>
#include <stdlib.h>

// verifica se a combinação de musicas atual é valida
int Valido(int *musicas, int *combinacao, int nA, int nB, int tempo, int nM)
{
  //  está repetido?
  for (int i = 0; i < (nA + nB) - 1; i++)
    if (combinacao[nA + nB - 1] == combinacao[i])
      return 0;

  // calcula tempo A e B
  int tamanhoA = 0;
  int tamanhoB = 0;
  for (int i = 0; i < nA; i++)
    tamanhoA += musicas[combinacao[i]];
  for (int i = nA; i < nA + nB; i++)
    tamanhoB += musicas[combinacao[i]];

  // tempo ultrapassa limites?
  return tamanhoA <= tempo && tamanhoB <= tempo;
}

// retorna se as nM musicas (em segundos) podem ser divididas em duas divisoes de
// tempo (também em segundos). Se for verdadeiro, combinacao vai possuir, seguidas,
// as nA musicas do lado A e as nB musicas do lado B
int Divide(int *musicas, int *combinacao, int *nA, int *nB, int tempo, int nM)
{
  // se não é valido:
  if (!Valido(musicas, combinacao, *nA, *nB, tempo, nM))
    // retorna falso
    return 0;

  // se é:
  // se já tiverem sido todas as musicas, finaliza
  if (*nA + *nB == nM)
    return 1;

  for (int m = 0; m < nM; m++)
  {
    // sobreescreve a musica da pos max
    combinacao[*nA + *nB] = m;

    // verifica se deve subir A ou B
    int sizeA = *nA;
    int sizeB = *nB;
    //  se já começou a preencher B
    if (sizeB > 0)
      sizeB++;
    //  se ainda não começou a preencher B
    else
    {
      int tempo_total = 0;
      for (int i = 0; i < sizeA + sizeB; i++)
        tempo_total += musicas[combinacao[i]];
      // se deve começar a preencher B
      if (tempo_total + musicas[m] > tempo)
        sizeB++;
      // se deve continuar preenchendo A
      else
        sizeA++;
    }

    // chama Divide para essa nova subsequencia
    if (Divide(musicas, combinacao, &sizeA, &sizeB, tempo, nM))
    {
      *nA = sizeA;
      *nB = sizeB;
      // se for valido, retorna como verdadeiro
      return 1;
    }
  }

  return 0;
}

int main()
{
  FILE *arq = fopen("entrada1.txt", "r");

  if (!arq)
  {
    perror("Erro ao abrir arquivo x");
    exit(1); // encerra o programa com status 1
  }

  int qnt_testes;
  int qnt_tempo;
  int qnt_musicas;

  // lê quantos destes serão feitos
  fscanf(arq, "%d", &qnt_testes);

  // para cada teste do arquivo
  for (int teste = 0; teste < qnt_testes; teste++)
  {
    // lê o tempo total da fita
    fscanf(arq, "%d", &qnt_tempo);
    // lê o número de musicas total
    fscanf(arq, "%d", &qnt_musicas);

    printf("%d minutos\n", qnt_tempo);

    // cria um vetor para os tempos lidos
    int *musicas = malloc(sizeof(int) * qnt_musicas);
    // cria um vetor para os tempos serem organizados
    int *musicas_escolhidas = malloc(sizeof(int) * qnt_musicas);

    // preenche o vetor musicas
    for (int m = 0; m < qnt_musicas; m++)
    {
      int minutos;
      int segundos;
      fscanf(arq, "%d", &minutos);
      fscanf(arq, "%d", &segundos);
      musicas[m] = minutos * 60 + segundos;
    }

    int musicas_no_A = 0;
    int musicas_no_B = 0;
    if (Divide(musicas, musicas_escolhidas, &musicas_no_A, &musicas_no_B,
               qnt_tempo * 30, qnt_musicas))
    {
      printf("Lado A\n");
      for (int i = 0; i < qnt_musicas; i++)
      {
        if (i == musicas_no_A)
          printf("\nLado B\n");

        int segundos = musicas[musicas_escolhidas[i]] % 60;
        int minutos = (musicas[musicas_escolhidas[i]] - segundos) / 60;
        printf("%dm %ds\n", minutos, segundos);
      }
    }
    else
      printf("Impossível gravar todas as músicas nessa fita\n");

    printf("\n");
    // libera os recursos alocados para esse teste
    free(musicas);
    free(musicas_escolhidas);
  }
  // fecha o arquivo
  fclose(arq);

  return 0;
}