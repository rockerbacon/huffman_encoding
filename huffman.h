#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "bool.h"

#define EXTENSAO "izi"

//compressao de arquivo
extern void Huffman_comprimir(char *nome_entrada, char *nome_saida);

//descompressao de arquivo
extern void Huffman_descomprimir(char *nome_entrada);

//verifica se nome do arquivo possui extensao correta
extern bool Huffman_extensaoValida(char *nome_arquivo);

#endif
