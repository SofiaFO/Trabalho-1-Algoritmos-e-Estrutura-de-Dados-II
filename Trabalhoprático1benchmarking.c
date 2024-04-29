/*Código de Benchmarking dos 5 algoritmos para 4 conjuntos de dados diferentes

Integrantes do grupo:
- Sofia Ferreira de Oliveira- 2024002336
- Jorge Alexandre Teixeira Henriques Luis - 2021010165 
- Rodrigo de Oliveira - 2024001965
- Diogo Conceição Nandes - 2017006513
- Diogo Hyppolito de Andrade - 2022001741

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*Defines importantes para definir variaveis que podem ser usadas em qualquer funcao

Range em específico é usado nas funções de criação de vetor, com a finalidade de limitar até
que número os elementos podem assumir ao serem criados*/

#define RANGE 100
#define SEED 6000
#define LINHAS 5
#define COLUNAS 4
#define PORCENTAGEM 50
#define MIN_MERGE 32

/*Struct com todos os campos necessários para guardar as variáveis que serão exibidos na tabela*/
struct conjDados
{

    int *vet;
    int tam, comp, trocas;
    double tempo;
};

typedef struct conjDados conjDados;

// Prototipação de todas as funções do programa
void pegaTamanhos(int *);

void menu1();
void menu2();

long timediff(clock_t, clock_t);

void selectionSort(conjDados *);
void insertionSort(conjDados *, int, int);
void mergeSort(conjDados *, int, int);
void merge(conjDados *, int, int, int);
void quickSort(conjDados *, int, int);
int particiona(conjDados *, int, int);
void timSort(conjDados *);

void preencheMatrizSelection(conjDados *, int *);
void preencheMatrizInsertion(conjDados *, int *);
void preencheMatrizMerge(conjDados *, int *);
void preencheMatrizQuick(conjDados *, int *);
void preencheMatrizTim(conjDados *, int *);

void ordenaCrescente(int *, int);
void ordenaDecrescente(int *, int);

int *geraAleatorios(int, int);
int *geraQuaseOrdenados(int , int );
int *geraOrdenados(int, int);

void geraTabelaComp(conjDados *);
void geraTabelaTrocas(conjDados *);
void geraTabelaTempo(conjDados *);

// Leias as explicações a seguir

/*Explicação de como vai funcionar o armazenamento:

Pelo o fato de termos que guardar muitas informações (3 métricas de avaliação, 5 tamanhos de conj.
de dados, 4 tipos de conjunto de dados segundo a sua ordenação, etc) nós resolvemos por criar um
struct que armazenasse a maioria dessas informações, e criasse uma "matriz" para cada método de
ordenação que guardasse todas as métricas de avaliação para cada elemento da tabela

Os tamanhos dos conjuntos de dados foi armazenado a um vetor próprio e os tipos diferentes de
ordenação de conjuntos de dados são separados como colunas da nossa "matriz". Quanto mais se
avança nas "linhas" da nossa matriz, maior vai sendo o tamanho do conjunto de dados, e a medida
que vai se avançando nas colunas vai modificando qual ordenação de conjunto de dado está se tra-
tando

*/

/*Explicação da "matriz" para cada método de algoritmo

Estamos basicamente representando uma matriz por meio de um vetor dinâmico

Resolvemos fazer dessa forma pois ao tentarmos fazer uma matriz dinamica e usar [i][j] nos de-
paramos com várias mensagens de erro e não conseguimos compilar

Ou seja, trate os vetores dinamicos do tipo "conjDados" como se fossem matrizes que foram planifica
das para ser representadas por um vetor, ou seja, se era pra ser uma matriz[3][4] agora vai ser um
vetor vet[12] em que os primeiros quatro indíces são os elementos da primeira linha, e por ai vai.
*/

/*Explicação geral de como esse programa funciona

Resolvemos por atribuir uma "matriz" de struct para cada método de ordenação, e o intuito é mostrar
cada "matriz" nas tres tabelas de tempo, comparações e trocas. Logo, alocamos todas essas "matrizes"
dinamicamente no main

No main chamamos todas as funções necessárias para executar as funcionalidades do programa, temos
a "pegaTamanhos" para pegar quanto que deve ser o tamanho de cada conjunto de dados do programa

Temos as funções "preencheMatriz" que pegam as "matrizes" e preenchem todos os seus elementos
com as respectivas trocas, comparações e tempo demorado para ordenar o conjunto de dado de tamanho
X (linhas) de ordenação Y (colunas)

E por fim temos o menu, que possui vários switch cases, que juntos vão disponibilizar um menu que
permita que você consiga navegar pelas tabelas de todos os métodos de ordenação.

O ciclo de vida da execução do programa é assim: Se inicia a execução, aloca dinamicamente as "matrizes"
para guardar os elementos das tabelas, se pega o tamanho dos conjuntos de dados, chama as funções
"preencheMatriz" que por sua vez chamam a função de criação de função certa e ordena o vetor DE CADA
ELEMENTO para se obter as três métricas (trocas, comparações e tempo), uma vez obtida as métricas e
guardadas nas "matrizes" nós adentramos aos switch cases que exibem o menu e todas as opções do menu

*/

int main()
{

    int opcao = 0, subopcao = 0, flag = 1;

    // Declaração das "matrizes" que armazenaram os dados de cada elemento da tabela
    conjDados *selectionDados = (conjDados *)malloc(sizeof(conjDados) * (LINHAS * COLUNAS));
    conjDados *insertionDados = (conjDados *)malloc(sizeof(conjDados) * (LINHAS * COLUNAS));
    conjDados *mergeDados = (conjDados *)malloc(sizeof(conjDados) * (LINHAS * COLUNAS));
    conjDados *quickDados = (conjDados *)malloc(sizeof(conjDados) * (LINHAS * COLUNAS));
    conjDados *timDados = (conjDados *)malloc(sizeof(conjDados) * (LINHAS * COLUNAS));

    int *vetTamanhos = (int *)malloc(sizeof(int) * LINHAS);

    // Função que pega os tamanhos dos conjuntos de dados
    pegaTamanhos(vetTamanhos);

    printf("Começando preenchimento das matrizes\n");
    /* Funções que preenchem a "matriz" de cada método de ordenação, dando para cada elemento
    as suas três métricas que vão ser mostradas no menu*/
    preencheMatrizSelection(selectionDados, vetTamanhos);
    printf("Terminou o selectionsort\n");
    preencheMatrizInsertion(insertionDados, vetTamanhos);
    printf("Terminou o insertionsort\n");
    preencheMatrizMerge(mergeDados, vetTamanhos);
    printf("Terminou o mergesort\n");
    preencheMatrizQuick(quickDados, vetTamanhos);
    printf("Terminou o quicksort\n");
    preencheMatrizTim(timDados, vetTamanhos);
    printf("Terminou o Timsort\n");

    /*Um while que possui um switch case principal seguido de váris switch cases menores, imple-
    mentamos desse jeito para dar a possibilidade do usuario seguir varios caminhos de opções e
    poupar o algoritmo de varios ifs

    Para saber para cada switch case menor faz, visite as funções "menu1" e "menu2"*/
    while (flag != 0)
    {

        menu1();

        scanf("%d", &opcao);
        fflush(stdin);

        switch (opcao)
        {

        case 1:
            menu2();
            scanf("%d", &subopcao);
            fflush(stdin);

            switch (subopcao)
            {

            case 1:
                geraTabelaTempo(selectionDados);
                break;

            case 2:
                geraTabelaComp(selectionDados);
                break;

            case 3:
                geraTabelaTrocas(selectionDados);
                break;

            case 4:
                flag = 0;
                printf("Saindo do programa...");
                break;

            default:
                printf("Valor digitado errado, retornando ao primeiro menu...\n");
            }

            break;

        case 2:
            menu2();
            scanf("%d", &subopcao);
            fflush(stdin);

            switch (subopcao)
            {

            case 1:
                geraTabelaTempo(insertionDados);
                break;

            case 2:
                geraTabelaComp(insertionDados);
                break;

            case 3:
                geraTabelaTrocas(insertionDados);
                break;

            case 4:
                flag = 0;
                printf("Saindo do programa...");
                break;

            default:
                printf("Valor digitado errado, retornando ao primeiro menu...\n");
            }

            break;

        case 3:
            menu2();
            scanf("%d", &subopcao);
            fflush(stdin);

            switch (subopcao)
            {

            case 1:
                geraTabelaTempo(mergeDados);
                break;

            case 2:
                geraTabelaComp(mergeDados);
                break;

            case 3:
                geraTabelaTrocas(mergeDados);
                break;

            case 4:
                flag = 0;
                printf("Saindo do programa...");
                break;

            default:
                printf("Valor digitado errado, retornando ao primeiro menu...\n");
            }

            break;

        case 4:
            menu2();
            scanf("%d", &subopcao);
            fflush(stdin);

            switch (subopcao)
            {

            case 1:
                geraTabelaTempo(quickDados);
                break;

            case 2:
                geraTabelaComp(quickDados);
                break;

            case 3:
                geraTabelaTrocas(quickDados);
                break;

            case 4:
                flag = 0;
                printf("Saindo do programa...");
                break;

            default:
                printf("Valor digitado errado, retornando ao primeiro menu...\n");
            }

            break;

        case 5:
            menu2();
            scanf("%d", &subopcao);
            fflush(stdin);

            switch (subopcao)
            {

            case 1:
                geraTabelaTempo(timDados);
                break;

            case 2:
                geraTabelaComp(timDados);
                break;

            case 3:
                geraTabelaTrocas(timDados);
                break;

            case 4:
                flag = 0;
                printf("Saindo do programa...");
                break;

            default:
                printf("Valor digitado errado, retornando ao primeiro menu...\n");
            }

            break;

        case 6:
            flag = 0;
            break;

        default:
            printf("Valor invalido, porfavor selecione um valor de opcao valido..\n\n");
        }
    }

    return 0;
}

long timediff(clock_t t1, clock_t t2)
{
    long elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
    return elapsed;
}

/*Nas funções de ordenação pegamos a "matriz" struct de cada método de ordenação e vamos indo
elemento por elemento ordenando o seu vetor e fornecendo as três métricas pedidas pelas dire-
trizes do trabalho

Note que em cada método de ordenação, sempre quando ocorre uma troca ou comparação, o campo
"trocas" e "comp" é incrementado, e nas funções posteriores (as funções de preenchimento da
nossa matriz) é feito o calculo do tempo gasto para cada ordenação*/

// Método de ordenação selection sort
void selectionSort(conjDados *selectionElem)
{
    int marcador = 0;
    int menor;
    int aux;
    int i, j, m, n;

    selectionElem->tempo = 0;
    selectionElem->comp = 0;
    selectionElem->trocas = 0;

    while (marcador < (selectionElem->tam) - 1)
    {
        menor = marcador;
        for (int m = marcador + 1; m < selectionElem->tam; m++)
        {
            if (selectionElem->vet[m] < selectionElem->vet[menor])
            {
                menor = m;
            }
            selectionElem->comp++;
        }
        if (menor != marcador)
        {
            aux = selectionElem->vet[marcador];
            selectionElem->vet[marcador] = selectionElem->vet[menor];
            selectionElem->vet[menor] = aux;
            selectionElem->trocas++;
        }
        marcador++;
    }
}

// Método de ordenação Insertion Sort
void insertionSort(conjDados *insertionElem, int inicio, int tam)
{
    int marcador;
    int aux;
    int pos;
    int i, j;

    insertionElem->tempo = 0;
    insertionElem->comp = 0;
    insertionElem->trocas = 0;

    // Cria um ponteiro para o vetor com o início deslocado
    int *vet = insertionElem->vet + inicio;

    for (marcador = 1; marcador < tam; marcador++)
    {
        pos = marcador - 1;
        aux = vet[marcador];
        while (aux < vet[pos] && pos >= 0)
        {
            vet[pos + 1] = vet[pos];
            pos = pos - 1;
            insertionElem->trocas++;
            insertionElem->comp++;
        }
        vet[pos + 1] = aux;
        insertionElem->trocas++;
    }
}

// Método de ordenação mergesort
void mergeSort(conjDados *mergeElem, int inicio, int fim)
{
    clock_t t1, t2;
    int meio;

    meio = (inicio + fim) / 2;
    if (inicio < fim)
    {
        mergeSort(mergeElem, inicio, meio);
        mergeSort(mergeElem, meio + 1, fim);
        merge(mergeElem, inicio, meio, fim);
    }
}

/* Função que faz as junções no vetor auxiliar do merge sort e depois copia para o vetor
original*/
void merge(conjDados *mergeElem, int inicio, int meio, int fim)
{
    int marcador1 = inicio;
    int marcador2 = meio + 1;
    int i = 0;
    int *vetoraux;

    vetoraux = (int *)malloc(((fim - inicio) + 1) * sizeof(int));
    if (vetoraux == NULL)
    {
        printf("ERRO AO ALOCAR MEMÓRIA\n");
        exit(1);
    }

    while (marcador1 <= meio && marcador2 <= fim)
    {

        if (mergeElem->vet[marcador1] <= mergeElem->vet[marcador2])
        {
            vetoraux[i] = mergeElem->vet[marcador1];
            i++;
            marcador1++;
            mergeElem->comp++;
            mergeElem->trocas++;
        }
        else
        {
            vetoraux[i] = mergeElem->vet[marcador2];
            i++;
            marcador2++;
            mergeElem->comp++;
            mergeElem->trocas++;
        }
    }

    while (marcador1 <= meio)
    {
        vetoraux[i] = mergeElem->vet[marcador1];
        i++;
        marcador1++;
        mergeElem->trocas++;
    }

    while (marcador2 <= fim)
    {
        vetoraux[i] = mergeElem->vet[marcador2];
        i++;
        marcador2++;
        mergeElem->trocas++;
    }

    for (marcador1 = inicio; marcador1 <= fim; marcador1++)
    {
        mergeElem->vet[marcador1] = vetoraux[marcador1 - inicio];
    }

    free(vetoraux);
}

// Método de ordenação quicksort
void quickSort(conjDados *quickElem, int inicio, int fim)
{
    int posPivo;

    if (inicio < fim)
    {
        posPivo = particiona(quickElem, inicio, fim);
        quickSort(quickElem, inicio, posPivo - 1);
        quickSort(quickElem, posPivo + 1, fim);
    }
}

// Função do quicksort que faz as trocas do quicksort
int particiona(conjDados *quickElem, int inicio, int fim)
{
    int pivo = quickElem->vet[inicio];
    int pos = inicio;
    int aux;


    for (int i = inicio + 1; i <= fim; i++)
    {
        if (quickElem->vet[i] < pivo)
        {
            pos++;
            if (i != pos)
            {
                aux = quickElem->vet[i];
                quickElem->vet[i] = quickElem->vet[pos];
                quickElem->vet[pos] = aux;
                quickElem->trocas++;
            }
        }
        quickElem->comp++;
    }

    if (pos != inicio)
    {
        aux = quickElem->vet[inicio];
        quickElem->vet[inicio] = quickElem->vet[pos];
        quickElem->vet[pos] = aux;
        quickElem->trocas++;
    }

    return pos;
}

// Método de ordenação timsort
void timSort(conjDados *timElem)
{
    int minRun = MIN_MERGE;
    int tam = timElem->tam;

    // Aplicar o insertionSort para "runs" mínimos
    for (int i = 0; i < tam / minRun; i++)
        insertionSort(timElem, i * minRun, minRun);

    // Caso o tamanho do vetor não seja divisível por minRun, chama o insertionSort para a última "run"
    if (tam % minRun != 0)
        insertionSort(timElem, tam / minRun, tam % minRun);

    // Merge os "runs" para obter um array ordenado
    for (int size = minRun; size < tam; size *= 2)
    { 
        for (int esq = 0; esq < tam; esq += 2 * size)
        { 
            int meio = esq + size - 1;
            int dir = (esq + 2 * size - 1 < tam - 1) ? esq + 2 * size - 1 : tam - 1; // + 1 comp

            if (meio < dir) // + 1 comp
                merge(timElem, esq, meio, dir);

            timElem->comp += 2;
        }
    }
}

/*A seguir estão as funções de preencher as "matrizes" de cada método de ordenação, as seguintes
etapas ocorrem nessas funções

Dois whiles para passar pela "matriz" inteira, dentro desse while é usando um switch case ou varios 
ifs para definir qual conjunto de dados estamos tratando (coluna), e para cada case é executado uma 
coisa diferente para ordenar o elemento da matriz da vez. Adicionalmente, é usado as funcoes clock()
e a timediff() para calcular o tempo gasto por cada funcao de ordenacao. Finalizada todas as funções
nós obtemos as três métricas cruciais ao nosso programa de benchmarking
*/

// Função que preenche os elementos da "matriz" do selection sort
void preencheMatrizSelection(conjDados *selectionDados, int *vetTam)
{
    clock_t t1, t2;
    int i, j = 0, k = 0, l = 0;
    int mediaComp = 0, mediaTrocas = 0;
    double mediaTemp = 0.0;

    /*Note que em cada iteração dos fors é ordenado apenas UM elemento da "matriz" selectionDados*/

    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            selectionDados[i * COLUNAS + j].tam = vetTam[i];
            if (j == 0)
            {

                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {
                    selectionDados[i * COLUNAS + j].vet = geraAleatorios(vetTam[i], SEED);
                    t1 = clock();
                    selectionSort(&selectionDados[i * COLUNAS + j]);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += selectionDados[i * COLUNAS + j].comp;
                    mediaTrocas += selectionDados[i * COLUNAS + j].trocas;
                }

                selectionDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                selectionDados[i * COLUNAS + j].comp = mediaComp / 5;
                selectionDados[i * COLUNAS + j].trocas = mediaTrocas / 5;
            }
            else if (j == 1)
            {

                // O zero do geraOrdenados representa se é crescente ou decrescente
                selectionDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 0);
                t1 = clock();
                selectionSort(&selectionDados[i * COLUNAS + j]);
                t2 = clock();
                selectionDados[i * COLUNAS + j].tempo = timediff(t1, t2);
            }
            else if (j == 2)
            {

                // O zero do geraOrdenados representa se é crescente ou decrescente
                selectionDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 1);
                t1 = clock();
                selectionSort(&selectionDados[i * COLUNAS + j]);
                t2 = clock();
                selectionDados[i * COLUNAS + j].tempo = timediff(t1, t2);
            }
            else if (j == 3)
            {

                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {

                    selectionDados[i * COLUNAS + j].vet = geraQuaseOrdenados(vetTam[i], PORCENTAGEM);
                    t1 = clock();
                    selectionSort(&selectionDados[i * COLUNAS + j]);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += selectionDados[i * COLUNAS + j].comp;
                    mediaTrocas += selectionDados[i * COLUNAS + j].trocas;
                }

                selectionDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                selectionDados[i * COLUNAS + j].comp = mediaComp / 5;
                selectionDados[i * COLUNAS + j].trocas = mediaTrocas / 5;
            }
            else
            {
                printf("O j saiu do seu limite...\n");

            }

            mediaTemp = 0;
            mediaComp = 0;
            mediaTrocas = 0;

        }
    }


}

// Função que preenche os elementos da "matriz" do insertion sort
void preencheMatrizInsertion(conjDados *insertionDados, int *vetTam)
{
    clock_t t1, t2;
    int i, j = 0, k = 0, l = 0;
    int mediaComp = 0, mediaTrocas = 0;
    double mediaTemp = 0.0;

    /*Note que em cada iteração dos fors é ordenado apenas UM elemento da "matriz" insertionDados*/


    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            insertionDados[i * COLUNAS + j].tam = vetTam[i];
            switch (j)
            {

            case 0:
                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {

                    insertionDados[i * COLUNAS + j].vet = geraAleatorios(vetTam[i], SEED);
                    t1 = clock();
                    insertionSort(&insertionDados[i * COLUNAS + j], 0, vetTam[i]);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += insertionDados[i * COLUNAS + j].comp;
                    mediaTrocas += insertionDados[i * COLUNAS + j].trocas;
                }

                insertionDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                insertionDados[i * COLUNAS + j].comp = mediaComp / 5;
                insertionDados[i * COLUNAS + j].trocas = mediaTrocas / 5;

                break;

            case 1:
                // O zero do geraOrdenados representa se é crescente ou decrescente
                insertionDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 0);
                t1 = clock();
                insertionSort(&insertionDados[i * COLUNAS + j], 0, vetTam[i]);
                t2 = clock();
                insertionDados[i * COLUNAS + j].tempo = timediff(t1, t2);
                break;

            case 2:
                // O zero do geraOrdenados representa se é crescente ou decrescente
                insertionDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 1);
                t1 = clock();
                insertionSort(&insertionDados[i * COLUNAS + j], 0, vetTam[i]);
                t2 = clock();
                insertionDados[i * COLUNAS + j].tempo = timediff(t1, t2);

                break;

            case 3:
                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {

                    insertionDados[i * COLUNAS + j].vet = geraQuaseOrdenados(vetTam[i], PORCENTAGEM);
                    t1 = clock();
                    insertionSort(&insertionDados[i * COLUNAS + j], 0, vetTam[i]);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += insertionDados[i * COLUNAS + j].comp;
                    mediaTrocas += insertionDados[i * COLUNAS + j].trocas;
                }

                insertionDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                insertionDados[i * COLUNAS + j].comp = mediaComp / 5;
                insertionDados[i * COLUNAS + j].trocas = mediaTrocas / 5;
                break;
            }

            mediaTemp = 0;
            mediaComp = 0;
            mediaTrocas = 0;
        }
    }

}
// Função que preenche os elementos da "matriz" do merge sort
void preencheMatrizMerge(conjDados *mergeDados, int *vetTam)
{
    // Inicialize o campos da struct com 0
    clock_t t1, t2;
    int i, j = 0, k = 0, l = 0;
    int mediaComp = 0, mediaTrocas = 0;
    double mediaTemp = 0.0;

    /*Note que em cada iteração dos fors é ordenado apenas UM elemento da "matriz" mergeDados*/

    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            mergeDados[i * COLUNAS + j].tam = vetTam[i];
            mergeDados[i * COLUNAS + j].comp = 0;
            mergeDados[i * COLUNAS + j].trocas = 0;

            switch (j)
            {

            case 0:
                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {

                    mergeDados[i * COLUNAS + j].vet = geraAleatorios(vetTam[i], SEED);
                    t1 = clock();
                    mergeSort(&mergeDados[i * COLUNAS + j], 0, vetTam[i] - 1);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += mergeDados[i * COLUNAS + j].comp;
                    mediaTrocas += mergeDados[i * COLUNAS + j].trocas;
                }

                mergeDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                mergeDados[i * COLUNAS + j].comp = mediaComp / 5;
                mergeDados[i * COLUNAS + j].trocas = mediaTrocas / 5;

                break;

            case 1:
                // O zero do geraOrdenados representa se é crescente ou decrescente
                mergeDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 0);
                t1 = clock();
                mergeSort(&mergeDados[i * COLUNAS + j], 0, vetTam[i] - 1);
                t2 = clock();
                mergeDados[i * COLUNAS + j].tempo = timediff(t1, t2);

                break;

            case 2:
                // O zero do geraOrdenados representa se é crescente ou decrescente
                mergeDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 1);
                t1 = clock();
                mergeSort(&mergeDados[i * COLUNAS + j], 0, vetTam[i] - 1);
                t2 = clock();
                mergeDados[i * COLUNAS + j].tempo = timediff(t1, t2);

                break;

            case 3:
                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {

                    mergeDados[i * COLUNAS + j].vet = geraQuaseOrdenados(vetTam[i], PORCENTAGEM);
                    t1 = clock();
                    mergeSort(&mergeDados[i * COLUNAS + j], 0, vetTam[i] - 1);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += mergeDados[i * COLUNAS + j].comp;
                    mediaTrocas += mergeDados[i * COLUNAS + j].trocas;
                }

                mergeDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                mergeDados[i * COLUNAS + j].comp = mediaComp / 5;
                mergeDados[i * COLUNAS + j].trocas = mediaTrocas / 5;
                break;

            default:
                printf("O j saiu do seu limite...\n");
            }

            mediaTemp = 0;
            mediaComp = 0;
            mediaTrocas = 0;
        }
    }
    

}
// Função que preenche os elementos da "matriz" do selection sort
void preencheMatrizQuick(conjDados *quickDados, int *vetTam)
{
    // Inicialize os campos da struct com 0
    clock_t t1, t2;
    int i, j = 0, k = 0, l = 0;
    int mediaComp = 0, mediaTrocas = 0;
    double mediaTemp = 0.0;

    /*Note que em cada iteração dos fors é ordenado apenas UM elemento da "matriz" quickDados*/

    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            quickDados[i * COLUNAS + j].tam = vetTam[i];
            quickDados[i * COLUNAS + j].comp = 0;
            quickDados[i * COLUNAS + j].trocas = 0;

            switch (j)
            {

            case 0:
                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {

                    quickDados[i * COLUNAS + j].vet = geraAleatorios(vetTam[i], SEED);
                    t1 = clock();
                    quickSort(&quickDados[i * COLUNAS + j], 0, vetTam[i] - 1);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += quickDados[i * COLUNAS + j].comp;
                    mediaTrocas += quickDados[i * COLUNAS + j].trocas;
                }

                quickDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                quickDados[i * COLUNAS + j].comp = mediaComp / 5;
                quickDados[i * COLUNAS + j].trocas = mediaTrocas / 5;

                break;

            case 1:
                // O zero do geraOrdenados representa se é crescente ou decrescente
                quickDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 0);
                t1 = clock();
                quickSort(&quickDados[i * COLUNAS + j], 0, vetTam[i] - 1);
                t2 = clock();
                quickDados[i * COLUNAS + j].tempo = timediff(t1, t2);

                break;

            case 2:
                // O zero do geraOrdenados representa se é crescente ou decrescente
                quickDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 1);
                t1 = clock();
                quickSort(&quickDados[i * COLUNAS + j], 0, vetTam[i] - 1);
                t2 = clock();
                quickDados[i * COLUNAS + j].tempo = timediff(t1, t2);

                break;

            case 3:
                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {

                    quickDados[i * COLUNAS + j].vet = geraQuaseOrdenados(vetTam[i], PORCENTAGEM);
                    t1 = clock();
                    quickSort(&quickDados[i * COLUNAS + j], 0, vetTam[i] - 1);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += quickDados[i * COLUNAS + j].comp;
                    mediaTrocas += quickDados[i * COLUNAS + j].trocas;
                }

                quickDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                quickDados[i * COLUNAS + j].comp = mediaComp / 5;
                quickDados[i * COLUNAS + j].trocas = mediaTrocas / 5;
                break;

            default:
                printf("O j saiu do seu limite...\n");
            }

            mediaTemp = 0;
            mediaComp = 0;
            mediaTrocas = 0;
        }
    }
}

// Função que preenche os elementos da "matriz" do tim sort
void preencheMatrizTim(conjDados *timDados, int *vetTam)
{
    // Inicialize o campos da struct com 0
    clock_t t1, t2;
    int i, j = 0, k = 0, l = 0;
    int mediaComp = 0, mediaTrocas = 0;
    double mediaTemp = 0.0;

    /*Note que em cada iteração dos fors é ordenado apenas UM elemento da "matriz" timDados*/

    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            timDados[i * COLUNAS + j].tam = vetTam[i];
            timDados[i * COLUNAS + j].comp = 0;
            timDados[i * COLUNAS + j].trocas = 0;

            switch (j)
            {

            case 0:
                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {

                    timDados[i * COLUNAS + j].vet = geraAleatorios(vetTam[i], SEED);
                    t1 = clock();
                    timSort(&timDados[i * COLUNAS + j]);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += timDados[i * COLUNAS + j].comp;
                    mediaTrocas += timDados[i * COLUNAS + j].trocas;
                }

                timDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                timDados[i * COLUNAS + j].comp = mediaComp / 5;
                timDados[i * COLUNAS + j].trocas = mediaTrocas / 5;

                break;

            case 1:
                // O zero do geraOrdenados representa se é crescente ou decrescente
                timDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 0);
                t1 = clock();
                timSort(&timDados[i * COLUNAS + j]);
                t2 = clock();
                timDados[i * COLUNAS + j].tempo = timediff(t1, t2);

                break;

            case 2:
                // O zero do geraOrdenados representa se é crescente ou decrescente
                timDados[i * COLUNAS + j].vet = geraOrdenados(vetTam[i], 1);
                t1 = clock();
                timSort(&timDados[i * COLUNAS + j]);
                t2 = clock();
                timDados[i * COLUNAS + j].tempo = timediff(t1, t2);

                break;

            case 3:
                // Média de 5 vezes para o conj. dados aleatorio e quase ordenado
                for (k = 0; k < 5; k++)
                {
                    timDados[i * COLUNAS + j].vet = geraQuaseOrdenados(vetTam[i], PORCENTAGEM);
                    t1 = clock();
                    timSort(&timDados[i * COLUNAS + j]);
                    t2 = clock();
                    mediaTemp += timediff(t1, t2);
                    mediaComp += timDados[i * COLUNAS + j].comp;
                    mediaTrocas += timDados[i * COLUNAS + j].trocas;
                }

                timDados[i * COLUNAS + j].tempo = mediaTemp / 5;
                timDados[i * COLUNAS + j].comp = mediaComp / 5;
                timDados[i * COLUNAS + j].trocas = mediaTrocas / 5;
                break;

            default:
                printf("O j saiu do seu limite...\n");
            }

            mediaTemp = 0;
            mediaComp = 0;
            mediaTrocas = 0;
        }
    }
}

// Função de geração de vetor para cada elemento das "matrizes" structs

/*Nesse daqui é gerado o vetor com elementos totalmente aleatórios*/
int *geraAleatorios(int tam, int semente)
{
    int i;

    int *vetAleatorio = (int *)malloc(sizeof(int) * tam);
    if (vetAleatorio == NULL)
    {

        printf("Falha ao alocar memoria...\n");
        exit(1);
    }

    srand(semente);

    for (i = 0; i < tam; i++)
    {

        vetAleatorio[i] = (rand() % (RANGE + 1));
    }

    return vetAleatorio;
}

// Função de geração de vetor para cada elemento das "matrizes" structs

/* Nesse daqui é gerado um vetor que vai ser ordenado de forma crescente
ou decrescente*/
int *geraOrdenados(int tam, int ordem)
{
    int i;

    int *vetOrdenado = (int *)malloc(sizeof(int) * tam);
    if (vetOrdenado == NULL)
    {

        printf("Falha ao alocar memoria...\n");
        exit(1);
    }

    srand(time(NULL));

    for (i = 0; i < tam; i++)
    {

        vetOrdenado[i] = (rand() % (RANGE + 1));
    }

    if (ordem == 0)
    {

        ordenaCrescente(vetOrdenado, tam);
    }
    else if (ordem == 1)
    {

        ordenaDecrescente(vetOrdenado, tam);
    }
    else
    {

        printf("Opcao de ordem invalida, porfavor insira um valor de ordem valido... (0 ou 1)\n");
        return NULL;
    }

    return vetOrdenado;
}

int *geraQuaseOrdenados(int tam, int porcentagem)
{
    int i, quantIter, aux;
    float porcent;

    // quantIter = quantidade de iteracoes pra desordenador o vetor

    /*Essa funcao desordena o vetor trocando o primeiro elemento com o ultimo, depois o segundo
    com o penultimo, e vai até onde a porcentagem do programa pediu*/

    int *vetQuaseOrdenado = (int *)malloc(sizeof(int) * tam);
    if (vetQuaseOrdenado == NULL)
    {

        printf("Falha ao alocar memoria\n");
        exit(1);
    }

    srand(time(NULL));

    for (i = 0; i < tam; i++)
    {

        vetQuaseOrdenado[i] = (rand() % (RANGE + 1));
    }

    ordenaCrescente(vetQuaseOrdenado, tam);

    porcent = ((float)porcentagem) / 100;
    quantIter = round((tam * porcent) / 2);

    for (i = 1; i <= quantIter; i++)
    {

        aux = vetQuaseOrdenado[i - 1];
        vetQuaseOrdenado[i - 1] = vetQuaseOrdenado[tam - i];
        vetQuaseOrdenado[tam - i] = aux;
    }

    return vetQuaseOrdenado;
}

/* Função de ordenação para as funções criadores de vetores

O motivo pra ter criado essa funcao mesmo que outras funcoes de ordenação ja existam eh porque
essa funcao aqui recebe menos parametros, ai por isso achei mais facil criar uma outra funcao
como menos parametros*/
// Usa selectionsort
void ordenaCrescente(int *vetOrdenado, int tam)
{

    int marcador = 0;
    int menor;
    int aux;

    while (marcador < tam - 1)
    {
        menor = marcador;
        for (int i = marcador + 1; i < tam; i++)
        {
            if (vetOrdenado[i] < vetOrdenado[menor])
            {
                menor = i;
            }
        }
        if (menor != marcador)
        {
            aux = vetOrdenado[marcador];
            vetOrdenado[marcador] = vetOrdenado[menor];
            vetOrdenado[menor] = aux;
        }
        marcador++;
    }
}

/* Função de ordenação para as funções de criação de vetor

O motivo pra ter criado essa funcao mesmo que outras funcoes de ordenação ja existam eh porque
essa funcao aqui recebe menos parametros, ai por isso achamos mais facil criar uma outra funcao
como menos parametros*/
// Usa selectionsort
void ordenaDecrescente(int *vetOrdenado, int tam)
{

    int marcador = tam - 1;
    int maior;
    int aux;
    int i;

    while (marcador > 0)
    {
        maior = marcador;
        for (i = marcador - 1; i >= 0; i--)
        {
            if (vetOrdenado[i] < vetOrdenado[maior])
            {
                maior = i;
            }
        }

        if (maior != marcador)
        {
            aux = vetOrdenado[maior];
            vetOrdenado[maior] = vetOrdenado[marcador];
            vetOrdenado[marcador] = aux;
        }

        marcador--;
    }
}

// Primeiro menu mais abrangente
void menu1()
{

    printf("Qual tabela voce deseja mostrar?:\n(escolha o numero correspondente)\n\n");
    printf("1: Selection Sort.\n");
    printf("2: Insertion Sort.\n");
    printf("3: Merge Sort.\n");
    printf("4: Quick Sort.\n");
    printf("5: Tim Sort.\n");
    printf("6: Sair do programa.\n");
}

// Segundo menu mais especifico
void menu2()
{

    printf("Qual tipo de tabela voce deseja mostrar?:\n\n");
    printf("1: Tabela de tempo.\n");
    printf("2: Tabela de comparacoes.\n");
    printf("3: Tabela de trocas.\n");
    printf("4: Sair do programa.\n");
}

// Função que pega os tamanhos dos conjuntos de dados do usuário
void pegaTamanhos(int *vetTamanhos)
{
    int i;

    printf("Porfavor informe os %d tamanhos que voce deseja utilizar no benchmarking\n", LINHAS);

    for (i = 0; i < LINHAS; i++)
    {
        scanf("%d", &vetTamanhos[i]);
        fflush(stdin);
    }
}

/*Funções de exibição de todos os tipos de tabelas com base em quam método de ordenação
foi escolhido*/

// Função que gera tabela de comparações
void geraTabelaComp(conjDados *ordenacaoDados)
{
    int i, j;

    printf("Gerando tabela de comparacao:\n\n");
    printf("A = Aleatorio\tOC = Ordem Crescente \t OD = Ordem Descrescente \t QO = Quase Ordenao\n\n");
    printf("\t\t A        OC      OD     QO\n");

    for (i = 0; i < LINHAS; i++)
    {
        j = 0;
        printf("TAMANHO: %d", ordenacaoDados[i * COLUNAS + j].tam);
        for (j = 0; j < COLUNAS; j++)
        {
            printf("\t%d", ordenacaoDados[i * COLUNAS + j].comp);
        }
        printf("\n");
    }

    printf("\n\n");
}

// Função que gera tabela de trocas
void geraTabelaTrocas(conjDados *ordenacaoDados)
{
    int i, j;

    printf("Gerando tabela de trocas:\n\n");
    printf("A = Aleatorio\tOC = Ordem Crescente \t OD = Ordem Descrescente \t QO = Quase Ordenao\n\n");
    printf("\t\t A        OC      OD     QO\n");

    for (i = 0; i < LINHAS; i++)
    {
        j = 0;
        printf("TAMANHO: %d", ordenacaoDados[i * COLUNAS + j].tam);
        for (j = 0; j < COLUNAS; j++)
        {
            printf("\t%d", ordenacaoDados[i * COLUNAS + j].trocas);
        }
        printf("\n");
    }

    printf("\n\n");
}

// Função que gera tabela de tempo que cada método de ordenação demorou para executar
void geraTabelaTempo(conjDados *ordenacaoDados)
{
    int i, j;

    printf("Gerando tabela de tempo: (em milisegundos)\n\n");
    printf("A = Aleatorio\tOC = Ordem Crescente \t OD = Ordem Descrescente \t QO = Quase Ordenao\n\n");
    printf("\t\t A        OC      OD     QO\n");

    for (i = 0; i < LINHAS; i++)
    {
        j = 0;
        printf("TAMANHO: %d", ordenacaoDados[i * COLUNAS + j].tam);
        for (j = 0; j < COLUNAS; j++)
        {
            printf("\t%lf", ordenacaoDados[i * COLUNAS + j].tempo);
        }
        printf("\n");
    }

    printf("\n\n");
}
