/*
----------------------------------------------------------
Projeto e Análise de Algoritmos I 
Prof. Antonio Luiz Basile
Universidade Presbiteriana Mackenzie
----------------------------------------------------------
Aluno: 
- Vinicius Rampazi de Oliveira  RA: 10426499
----------------------------------------------------------
Descrição:
Este programa implementa os algoritmos de ordenação:
1. Insertion Sort
2. Merge Sort

Objetivo:
- Ler até 10.000 palavras de um arquivo texto ("in.txt").
- Ordenar as palavras lexicograficamente (ordem alfabética).
- Gerar dois arquivos:
   * "out1.txt" com ordenação via Insertion Sort
   * "out2.txt" com ordenação via Merge Sort
- Imprimir na tela o número de passos (comparações) de cada algoritmo.
----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PALAVRAS 10000 // Número máximo de palavras que podem ser lidas
#define MAX_TAM 100       // Tamanho máximo de cada palavra

// Contadores globais de passos (comparações/movimentações)
long long passosInsertion = 0;
long long passosMerge = 0;

// ---------- Função: Insertion Sort ----------
void insertionSort(char **palavras, int n) {
    for (int i = 1; i < n; i++) {
        char *chave = palavras[i]; // Palavra atual a ser inserida na posição correta
        int j = i - 1;

        // Move elementos maiores que a chave uma posição à frente
        while (j >= 0 && strcmp(palavras[j], chave) > 0) {
            passosInsertion++; // Conta a comparação
            palavras[j + 1] = palavras[j];
            j--;
        }
        passosInsertion++; // Conta a última comparação ou inserção
        palavras[j + 1] = chave;
    }
}

// ---------- Funções: Merge Sort ----------
void merge(char **palavras, int esq, int meio, int dir) {
    int n1 = meio - esq + 1; // tamanho do subvetor esquerdo
    int n2 = dir - meio;     // tamanho do subvetor direito

    // Vetores temporários
    char **L = malloc(n1 * sizeof(char *));
    char **R = malloc(n2 * sizeof(char *));
    
    // Copia os elementos para os vetores auxiliares
    for (int i = 0; i < n1; i++) L[i] = palavras[esq + i];
    for (int j = 0; j < n2; j++) R[j] = palavras[meio + 1 + j];

    int i = 0, j = 0, k = esq;

    // Intercala os dois subvetores na ordem correta
    while (i < n1 && j < n2) {
        passosMerge++; // Conta cada comparação
        if (strcmp(L[i], R[j]) <= 0) {
            palavras[k++] = L[i++];
        } else {
            palavras[k++] = R[j++];
        }
    }

    // Copia os elementos restantes (se houver)
    while (i < n1) palavras[k++] = L[i++];
    while (j < n2) palavras[k++] = R[j++];

    // Libera memória alocada
    free(L);
    free(R);
}

// Função recursiva do Merge Sort
void mergeSort(char **palavras, int esq, int dir) {
    if (esq < dir) {
        int meio = esq + (dir - esq) / 2; // Calcula o ponto médio
        mergeSort(palavras, esq, meio);   // Ordena a metade esquerda
        mergeSort(palavras, meio + 1, dir); // Ordena a metade direita
        merge(palavras, esq, meio, dir);  // Intercala as duas metades
    }
}

// ---------- Função principal ----------
int main() {
    FILE *fin = fopen("in.txt", "r"); // Abre o arquivo de entrada
    if (!fin) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    char **palavras = malloc(MAX_PALAVRAS * sizeof(char *)); // Vetor de ponteiros para armazenar palavras
    char buffer[MAX_TAM]; // Buffer temporário para leitura
    int n = 0;

    // Leitura das palavras do arquivo "in.txt"
    while (fscanf(fin, "%s", buffer) != EOF && n < MAX_PALAVRAS) {
        palavras[n] = malloc((strlen(buffer) + 1) * sizeof(char)); // Aloca memória para cada palavra
        strcpy(palavras[n], buffer); // Copia a palavra lida para o vetor
        n++;
    }
    fclose(fin);

    // Cria cópias do vetor original para aplicar os dois algoritmos
    char **palavrasInsertion = malloc(n * sizeof(char *));
    char **palavrasMerge = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        palavrasInsertion[i] = strdup(palavras[i]); // Cópia para Insertion Sort
        palavrasMerge[i] = strdup(palavras[i]);     // Cópia para Merge Sort
    }

    // ---------- Insertion Sort ----------
    insertionSort(palavrasInsertion, n);
    FILE *fout1 = fopen("out1.txt", "w"); // Abre arquivo de saída
    for (int i = 0; i < n; i++) {
        fprintf(fout1, "%s\n", palavrasInsertion[i]); // Escreve palavra ordenada
    }
    fclose(fout1);

    // ---------- Merge Sort ----------
    mergeSort(palavrasMerge, 0, n - 1);
    FILE *fout2 = fopen("out2.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(fout2, "%s\n", palavrasMerge[i]);
    }
    fclose(fout2);

    // ---------- Saída dos resultados ----------
    printf("Passos Insertion Sort: %lld\n", passosInsertion);
    printf("Passos Merge Sort: %lld\n", passosMerge);

    // ---------- Liberação da memória ----------
    for (int i = 0; i < n; i++) {
        free(palavras[i]);
        free(palavrasInsertion[i]);
        free(palavrasMerge[i]);
    }
    free(palavras);
    free(palavrasInsertion);
    free(palavrasMerge);

    return 0;
}
