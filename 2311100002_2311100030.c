// Autores: Arthur Grasnievicz e Gabriel Gois
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de dados modificada para a contagem de frequência de caracteres e construção da árvore de Huffman
typedef struct HuffmanNode {
    char caractere;
    int frequencia;
    struct HuffmanNode *esquerda, *direita;
} HuffmanNode;

//Estrutura de dados Criada para armazenar a codificacao de Huffman
typedef struct {
    char caractere;
    char *codigo;
} CodificacaoHuffman;

// Função para criar um novo nó da árvore de Huffman
HuffmanNode* criarNo(char caractere, int frequencia, HuffmanNode* esquerda, HuffmanNode* direita) {
    HuffmanNode* novoNo = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    novoNo->caractere = caractere;
    novoNo->frequencia = frequencia;
    novoNo->esquerda = esquerda;
    novoNo->direita = direita;
    return novoNo;
}

// Função que ordena as frequências de caracteres em ordem crescente através de um bubble sort com flag
void ordenarHuffmanNodes(HuffmanNode *nodes, int tamanho) {
    int i, j;
    int trocou;
    for (i = 0; i < tamanho - 1; i++) {
        trocou = 0;
        for (j = 0; j < tamanho - i - 1; j++) {
            // ordenação por ordem crescente de frequência e, em caso de empate, por ordem crescente de caractere
            if (nodes[j].frequencia > nodes[j + 1].frequencia ||
                (nodes[j].frequencia == nodes[j + 1].frequencia && nodes[j].caractere > nodes[j + 1].caractere)) {
                HuffmanNode temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
                trocou = 1;
            }
        }
        if (trocou == 0) {
            break; 
        }
    }
}

// Função que conta a frequência de caracteres em um texto criando um array de HuffmanNodes
HuffmanNode* contarFrequencia(const char* texto, int* tamanho){
    
     HuffmanNode *nodes = NULL;
    *tamanho = 0;
    while(*texto){
        int encontrado = 0;
        for(int i = 0; i < *tamanho; i++){ 
            if(nodes[i].caractere == *texto){
                nodes[i].frequencia++;
                encontrado = 1;
                break;
            }
        }
        if(!encontrado){
            (*tamanho)++;
            nodes = realloc(nodes, *tamanho * sizeof(HuffmanNode));
            nodes[*tamanho - 1].caractere = *texto;
            nodes[*tamanho - 1].frequencia = 1;
            nodes[*tamanho - 1].esquerda = NULL;
            nodes[*tamanho - 1].direita = NULL;
        }
        texto++;
    }

    return nodes;
}

// Função para imprimir o array de HuffmanNode
void printarFrequencia(HuffmanNode *nodes, int tamanho) {
    printf("Posicao | Caractere | Frequencia\n");
    printf("-------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        // Verifica se o caractere é imprimível
        if (nodes[i].caractere >= 32 && nodes[i].caractere <= 126) {
            printf("  %d     |    '%c'    | %d\n", i, nodes[i].caractere, nodes[i].frequencia);
        } else {
            printf("  %d     |  0x%X | %d\n", i, nodes[i].caractere, nodes[i].frequencia); // Imprime o valor hexadecimal para caracteres não imprimíveis
        }
    }
}

// Função para construir a árvore de Huffman
HuffmanNode* construirArvoreHuffman(HuffmanNode *nodes, int tamanho) {
    while (tamanho > 1) {
        //Inicialmente Ordena a lista de nós em ordem crescente de frequência (Considerando a frequencia de ocorrencia dos dois caracteres filhos para a frequencia do pai)
        ordenarHuffmanNodes(nodes, tamanho);
        
        // Cria um novo nó com os dois menores elementos que já foram ordenados como filhos
        HuffmanNode *esquerda = criarNo(nodes[0].caractere, nodes[0].frequencia, nodes[0].esquerda, nodes[0].direita);
        HuffmanNode *direita = criarNo(nodes[1].caractere, nodes[1].frequencia, nodes[1].esquerda, nodes[1].direita);

        //Soma as frequencias dos filhos para criar o nó pai, o nó pai é um HuffmaNode com caractere nulo e frequencia igual a soma das frequencias dos filhos
        HuffmanNode *soma = criarNo('\0', esquerda->frequencia + direita->frequencia, esquerda, direita);

        // Substitui os dois nós menores pelo nó soma
        nodes[0] = *soma;
        for (int i = 1; i < tamanho - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        tamanho--;
    }

    //Quando o tamanho do array de HuffmanNodes for 1, a raiz da arvore de Huffman é o unico elemento restante então retorna o nó raiz.
    return criarNo(nodes[0].caractere, nodes[0].frequencia, nodes[0].esquerda, nodes[0].direita);
}


// Função auxiliar para contar o número de folhas na árvore de Huffman (nós folha são os nós que possuem um caractere)
// Função recursiva que percorre a árvore de Huffman e retorna o numero total de caracteres para gerar os codigos de Huffman
int contarFolhas(HuffmanNode *raiz) {
    if (raiz == NULL) return 0;
    if (raiz->esquerda == NULL && raiz->direita == NULL) return 1;
    return contarFolhas(raiz->esquerda) + contarFolhas(raiz->direita);
}

// Função auxiliar para gerar os códigos de Huffman recursivamente e armazenar em um array de CodificacaoHuffman
//Essa função é chamada por gerarCodigosHuffman para gerar os codigos em sí.

void gerarCodigos(HuffmanNode *raiz, char *codigoAtual, int tamanhoAtual, CodificacaoHuffman *codificacoes, int *index) {
    if (raiz == NULL) return;

    // Se é uma folha, armazena a codificacao
    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        codificacoes[*index].caractere = raiz->caractere;
        codificacoes[*index].codigo = (char*)malloc((tamanhoAtual + 1) * sizeof(char));
        strncpy(codificacoes[*index].codigo, codigoAtual, tamanhoAtual);
        codificacoes[*index].codigo[tamanhoAtual] = '\0';
        (*index)++;
        return;
    }

    // Adiciona '0' e vai para a esquerda
    codigoAtual[tamanhoAtual] = '0';
    gerarCodigos(raiz->esquerda, codigoAtual, tamanhoAtual + 1, codificacoes, index);

    // Adiciona '1' e vai para a direita
    codigoAtual[tamanhoAtual] = '1';
    gerarCodigos(raiz->direita, codigoAtual, tamanhoAtual + 1, codificacoes, index);
}

// Função principal para gerar o array de codificações de Huffman a partir da árvore de Huffman
// A função Utiliza de suas funções auxiliares para gerar os codigos de Huffman e armazenar em um array de CodificacaoHuffman
CodificacaoHuffman* gerarCodigosHuffman(HuffmanNode *raiz, int *totalCodificacoes) {

    if (raiz == NULL) {
        *totalCodificacoes = 0;
        return NULL;
    }

    *totalCodificacoes = contarFolhas(raiz);
    CodificacaoHuffman *codificacoes = (CodificacaoHuffman*)malloc(*totalCodificacoes * sizeof(CodificacaoHuffman));
    char codigoAtual[100]; // Buffer temporário para armazenar o código atual
    int index = 0;
    gerarCodigos(raiz, codigoAtual, 0, codificacoes, &index);
    return codificacoes;
}

//Função para codificar o texto de entrada recebendo o Texto de entrada e o Array de codificações
char* codificarTexto(char* texto, CodificacaoHuffman* codificacoes) {
    size_t bufferSize = 64; // Tamanho inicial do buffer
    char* textoCodificado = malloc(bufferSize);

    //Verifica falha ao aloca memória
    if (!textoCodificado) return NULL; // Falha na alocação de memória

    textoCodificado[0] = '\0'; // Inicializa a string de texto codificado como vazia

    size_t currentLength = 0;
    for (size_t i = 0; texto[i] != '\0'; i++) {
        for (int j = 0; codificacoes[j].caractere != '\0'; j++) {
            if (texto[i] == codificacoes[j].caractere) {
                size_t codeLength = strlen(codificacoes[j].codigo);

                // Verifica se precisa realocar mais memoria para o buffer.
                if (currentLength + codeLength >= bufferSize) {
                    bufferSize *= 2; // Dobra o tamanho do buffer
                    char* temp = realloc(textoCodificado, bufferSize);
                    if (!temp) {
                        free(textoCodificado);
                        return NULL; // Falha na realocação de memória
                    }

                    textoCodificado = temp;
                }
                strcat(textoCodificado, codificacoes[j].codigo);
                currentLength += codeLength;
                break;
            }
        }
    }
    return textoCodificado;
}


//Função para decodificar o texto recebendo o texto codificado e o array de codificações
char* decodificarTexto(char* textoCodificado, CodificacaoHuffman* codificacoes) {
    size_t bufferSize = 64; // Tamanho inicial do buffer para o texto decodificado
    char* textoDecodificado = malloc(bufferSize);

    //Verificações de erro na alocação de memória.
    if (!textoDecodificado) return NULL; // Falha na alocação de memória
    textoDecodificado[0] = '\0'; // Inicializa como string vazia

    char* tempCodigo = malloc(strlen(textoCodificado) + 1); // Buffer temporário para o código atual
    if (!tempCodigo) {
        free(textoDecodificado);
        return NULL; // Falha na alocação de memória
    }

    
    tempCodigo[0] = '\0';

    size_t indexDecodificado = 0;
    for (size_t i = 0; textoCodificado[i] != '\0'; i++) {
        // Adiciona o próximo caractere ao código temporário
        size_t lenTempCodigo = strlen(tempCodigo);
        tempCodigo[lenTempCodigo] = textoCodificado[i];
        tempCodigo[lenTempCodigo + 1] = '\0';

        // Procura o código temporário nas codificações
        for (CodificacaoHuffman* cod = codificacoes; cod->caractere != '\0'; cod++) {
            if (strcmp(tempCodigo, cod->codigo) == 0) {

                // Verifica se precisa realocar o tamanho do buffer
                if (indexDecodificado + 1 >= bufferSize) {
                    bufferSize *= 2; // Dobra o tamanho do buffer
                    char* temp = realloc(textoDecodificado, bufferSize);
                    if (!temp) {
                        free(textoDecodificado);
                        free(tempCodigo);
                        return NULL; // Falha na realocação de memória
                    }

                    textoDecodificado = temp;
                }
                textoDecodificado[indexDecodificado++] = cod->caractere;
                textoDecodificado[indexDecodificado] = '\0'; // Garante que a string seja terminada corretamente
                tempCodigo[0] = '\0'; // Reseta o código temporário
                break;
            }
        }
    }

    free(tempCodigo); // Libera o buffer temporário
    return textoDecodificado;
}

//Função para imprimir as codificações de Huffman
void imprimirCodificacoes(CodificacaoHuffman *codificacoes, int totalCodificacoes) {
    printf("Caractere | Codigo Huffman\n");
    printf("-------------------------\n");
    for (int i = 0; i < totalCodificacoes; i++) {   
        if (codificacoes[i].caractere >= 32 && codificacoes[i].caractere <= 126) {
            printf("   '%c'    | %s\n", codificacoes[i].caractere, codificacoes[i].codigo);
        } else {
            printf("  0x%X | %s\n", codificacoes[i].caractere, codificacoes[i].codigo);
        }
    }
}

// Função para liberar a memória da árvore de Huffman
void liberarArvore(HuffmanNode* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}
// Função para liberar a memória das codificações de Huffman
void liberarCodificacoes(CodificacaoHuffman *codificacoes, int totalCodificacoes) {
    for (int i = 0; i < totalCodificacoes; i++) {
        free(codificacoes[i].codigo);
    }
    free(codificacoes);
}
// Função para liberar a memória dos nós da árvore de Huffman
void liberarNodes(HuffmanNode *nodes, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free(nodes[i].esquerda);
        free(nodes[i].direita);
    }
    free(nodes);
}

//Função para liberar a memória de todos os elementos
void liberarMemoria(HuffmanNode *raiz, HuffmanNode *nodes, CodificacaoHuffman *codificacoes, int totalCodificacoes) {
    liberarArvore(raiz);
    liberarNodes(nodes, totalCodificacoes);
    liberarCodificacoes(codificacoes, totalCodificacoes);
}

//Função para unificar as chamadas de Contar Frequencia e construção da arvore de Huffman retornando a raiz da arvore:
HuffmanNode* huffmanTree(const char* texto){
    int tamanho;
    HuffmanNode* nodes = contarFrequencia(texto, &tamanho);
    HuffmanNode* raiz = construirArvoreHuffman(nodes, tamanho);
    return raiz;
}
//Função para salvar codificacao e decodificacao em arquivo .txt
void salvarCodificacao(char* pasta,char *texto) {
    FILE *arquivo = fopen(pasta, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        exit(1);
    }else{
        fprintf(arquivo, "%s", texto);
    }
    fclose(arquivo);
}

char *lerTexto(char *nomeArquivo){
    FILE *fp = fopen(nomeArquivo, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // Vendo tamanho do arquivo para alocar memória para armazená-lo no heap
    fseek(fp, 0, SEEK_END);
    long tamanho = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Alocando memória necessária, após saber tamanho do arquivo 
    char *conteudo = (char *)malloc(tamanho + 1);
    if (conteudo == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    fread(conteudo, 1, tamanho, fp);//lendo caractrere do arquivo
    conteudo[tamanho] = '\0'; // colocando fim da string
    fclose(fp);

    return conteudo;
}

int main() {
    //char texto[] = "AAABBB!";
    char *texto = lerTexto("amostra.txt");//alocando memória para armazenar 
    HuffmanNode* raiz = huffmanTree(texto);//criando árvore

    int totalCodificacoes;
    CodificacaoHuffman* codificacoes = gerarCodigosHuffman(raiz, &totalCodificacoes);
    imprimirCodificacoes(codificacoes, totalCodificacoes);//mostrando códigos para letras

    char* textocodificado = codificarTexto(texto, codificacoes);
    salvarCodificacao("codificado.txt",textocodificado);//salvando texto codificado em codificado.txt
    //printf("Texto codificado: %s\n", textocodificado);

    char* textodecodificado = decodificarTexto(lerTexto("codificado.txt"), codificacoes);//lendo e decodificando texto de codificado.txt
    salvarCodificacao("decodificado.txt",textodecodificado);//salvando texto decodificado em decodificado.txt
    //printf("Texto decodificado: %s\n", textodecodificado);
    return 0;
}