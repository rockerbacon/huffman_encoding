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
	} else	if (args[2] == NULL) {
		printf ("Especifique arquivo de destino\n");
		return 2;
	} else if (args[2][0] == '\0' || args[2][0] == '\n') {
		printf ("Especifique arquivo de destino\n");
		return 2;
	}

	char *nome_saida;

	//formatar nome do arquivo de destino caso necessario
	//verificar arquivo terminado na extensao correta
	if (Huffman_extensaoValida(args[2])) {
		nome_saida = args[2];
	}
	//se nao termina na extensao correta da append da extensao
	else {

		unsigned int tamanho, i;

		//descobre tamanho da string do nome do arquivo de destino
		for (tamanho = 0; args[2][tamanho] != '\0'; tamanho++);

		//alloca espaco para nome do arquivo + extensao
		nome_saida = (char*)malloc((tamanho+5)*sizeof(char));

		//copia nome do arquivo para novo nome
		for (i = 0; i < tamanho; i++) {
			nome_saida[i] = args[2][i];
		}
		//adiciona extensao ao novo nome
		nome_saida[i] = '.';
		for (i = 0, tamanho++; i < 3; i++, tamanho++) {
			nome_saida[tamanho] = EXTENSAO[i];
		}
		nome_saida[tamanho] = '\0';

	}

	//comprime arquivo de entrada criando arquivo de saida
	Huffman_comprimir(args[1], nome_saida);

	//exclui arquivo original
	remove(args[1]);

	return 0;

}
