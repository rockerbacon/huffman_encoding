#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"
#include "arvore.h"

//#define TESTE_TAM 6

/*OPERACOES PILHA*/
//inicializar pilha
void Pilha_inicializar (Pilha *pilha, unsigned int tamanho) {

	pilha->vetor = (No_Arvore**)malloc(tamanho*sizeof(No_Arvore*));
	pilha->fim = 0;
	pilha->tamanho = tamanho;

}

//adicionar item na pilha
bool Pilha_push (Pilha *pilha, No_Arvore *no) {

	if (pilha->fim == pilha->tamanho) {
		return false;
	}

	pilha->vetor[pilha->fim] = no;
	pilha->fim++;

	return true;

}

//remover item da pilha
struct _No_Arvore* Pilha_pop (Pilha *pilha) {

	if (pilha->fim == 0) {
		return NULL;
	}

	pilha->fim--;

	return pilha->vetor[pilha->fim];

}

//esvaziar pilha
void Pilha_esvaziar (Pilha *pilha) {
	pilha->fim = 0;
}

//excluir pilha
void Pilha_free (Pilha *pilha) {

	if (pilha == NULL) {
		return;
	}else if (pilha->vetor == NULL){
		return;
	}

	free(pilha->vetor);
	pilha->fim = 0;
	pilha->tamanho = 0;

}

//imprimir pilha
void Pilha_imprimir (FILE *saida, Pilha *pilha) {

	unsigned int i;

	for (i = 0; i < pilha->fim; i++) {
		fprintf(saida, "%c%u ", pilha->vetor[i]->c, pilha->vetor[i]->freq);
	}

	fprintf(saida, "\n");

}

//verifica se um elemento se encontra na pilha
bool naPilha (Pilha *pilha, No_Arvore *no) {

	unsigned int i;

	for (i = 0; i < pilha->fim; i++) {

		if (pilha->vetor[i] == no) {
			return true;
		}

	}

	return false;

}

//retorna o no que esta no topo da pilha
struct _No_Arvore* Pilha_topo (Pilha *pilha) {

	if (pilha == NULL) {
		return NULL;
	}

	return pilha->vetor[pilha->fim-1];

}

/*TESTE*/
/*
int main (void) {

	Pilha teste;
	char c;
	unsigned int i;

	Pilha_inicializar(&teste, TESTE_TAM);
	c = 'a';
	for (i = 0; i < TESTE_TAM; i++) {
		
		Pilha_push(&teste, c);
		c++;

	}

	Pilha_imprimir(stdout, &teste);

	Pilha_pop(&teste);
	Pilha_pop(&teste);

	Pilha_imprimir(stdout, &teste);

	Pilha_esvaziar(&teste);
	Pilha_free(&teste);

	return 0;
}
*/
