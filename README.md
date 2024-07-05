# HuffmanTreeCompressorDescompressor
Este repositório contém um código para compressão e descompressão de textos utilizando a Árvore de Huffman. O objetivo principal é ler um texto de um arquivo de entrada, compactá-lo e gerar um arquivo de saída com o texto "compactado". Além disso, o código também permite descompactar o texto compactado.

# Funcionalidades:
Leitura de Texto: Lê um texto de um arquivo de entrada.
Criação de Huffman Nodes: Percorre o texto e cria nós de Huffman.
Ordenação com Bubble Sort: Usa um algoritmo de ordenação Bubble Sort com flag para ordenar os nós.
Construção da Árvore de Huffman: Junta os nós de forma iterativa até formar a Árvore de Huffman completa.
Compactação de Texto: Gera um arquivo de saída com o texto compactado.
Descompactação de Texto: Lê o arquivo compactado e retorna o texto original descompactado.

# Como usar:
Preparar o Arquivo de Entrada: Coloque o texto que deseja compactar em um arquivo de entrada chamado "amostra.txt".
Executar o Código: Execute o código fornecendo o arquivo de entrada.
Gerar Arquivo Compactado: O código irá gerar um arquivo de saída com o texto compactado chamado "codificado.txt".
Descompactar o Texto: Utilize a funcionalidade de descompactação para retornar o texto original em um arquivo chamado "decodificado.txt".

# Requisitos:
Linguagem de Programação: C
Bibliotecas utilizadas: stdio.h , stdlib.h e string.h
