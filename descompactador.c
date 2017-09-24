#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main (int argc, char **args) {

	if (args[1] == NULL) {
		printf ("Especifique arquivo de origem\n");
		return 1;
	} else if (args[1][0] == '\0' || args[1][0] == '\n') {
		printf ("Especifique arquivo de origem\n");
		return 1;
	}

	char	*nome_entrada,
		*nome_saida;

	unsigned int tamanho, i;

	//formatar nome do arquivo de destino caso necessario
	//verificar arquivo terminado na extensao correta
	if (Huffman_extensaoValida(args[1])) {
		for (i = 0; args[1][i] != '.'; i++);
		args[1][i] = '\0';
	}

	//descobre tamanho da string do nome do arquivo de destino
	for (tamanho = 0; args[1][tamanho] != '\0'; tamanho++);

	//alloca espaco para nome do arquivo + extensao
	nome_entrada = (char*)malloc((tamanho+5)*sizeof(char));

	//copia nome do arquivo para novo nome
	for (i = 0; i < tamanho; i++) {
		nome_entrada[i] = args[1][i];
	}
	//adiciona extensao ao novo nome
	nome_entrada[i] = '.';
	for (i = 0, tamanho++; i < 3; i++, tamanho++) {
		nome_entrada[tamanho] = EXTENSAO[i];
	}
	nome_entrada[tamanho] = '\0';

	//descomprime arquivo de entrada criando arquivo de saida
	Huffman_descomprimir(nome_entrada);

	return 0;

	free(nome_entrada);

}
