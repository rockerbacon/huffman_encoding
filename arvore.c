#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "pilha.h"

#define IDENT_TAM 2

//#define TESTE_TAM 3

/*OPERACOES ARVORE*/
//inicializacao da arvore
void Arvore_inicializar (Arvore *arvore) {

	if (arvore == NULL) {
		return;
	}

	arvore->raiz = NULL;

}

//cria novo no
No_Arvore* Arvore_novaRaiz(char c, unsigned int freq, No_Arvore *esquerda, No_Arvore *direita) {

	No_Arvore *novo = (No_Arvore*)malloc(sizeof(No_Arvore));

	if (novo != NULL) {

		novo->freq = freq;
		novo->c = c;
		novo->direita = direita;
		novo->esquerda = esquerda;

	}

	return novo;	

}

//verifica se no eh no folha
bool folha (No_Arvore *no) {

	if (no->direita == NULL && no->esquerda == NULL) {
		return true;
	}

	return false;

}

//esvaziar arvore
void Arvore_esvaziar_rec (No_Arvore *no) {


	if ( !(folha(no)) ) {

		Arvore_esvaziar_rec(no->direita);
		Arvore_esvaziar_rec(no->esquerda);

	}

	free(no);

}

void Arvore_esvaziar (Arvore *arvore) {

	if (arvore == NULL || arvore->raiz == NULL) {
		return;
	}

	Arvore_esvaziar_rec(arvore->raiz);

	arvore->raiz = NULL;

}

//imprimir arvore utilizando representacao com identacao
void Arvore_imprimir_rec (FILE *saida, No_Arvore *no, unsigned short int identacao) {

	unsigned short int i;

	if (no != NULL) {

		for (i = identacao*IDENT_TAM; i > 0; i--) {
			fprintf(saida, ".");
		}

		fprintf(saida, "%c%d\n", no->c, no->freq);
		Arvore_imprimir_rec(saida, no->direita, identacao+1);
		Arvore_imprimir_rec(saida, no->esquerda, identacao+1);

	}

}

void Arvore_imprimir (FILE *saida, Arvore *arvore) {

	if (arvore == NULL || saida == NULL) {
		return;
	}

	Arvore_imprimir_rec(saida, arvore->raiz, 0);

}

//retorna altura da arvore
unsigned int Arvore_altura_rec (No_Arvore *no, unsigned int altura) {

	if (no == NULL) {
		return 0;
	}

	unsigned int direita, esquerda;

	if (folha(no)) {

		return altura;

	}else{

		direita = Arvore_altura_rec (no->direita, altura+1);
		esquerda = Arvore_altura_rec (no->esquerda, altura+1);

		return (direita > esquerda) ? direita : esquerda; 

	}

}

unsigned int Arvore_altura (Arvore *arvore) {
	if (arvore == NULL) {
		return 0;
	}
	return Arvore_altura_rec(arvore->raiz, 0);
}

//retorna numero de nos que arvore contem
unsigned int Arvore_tamanho_rec (No_Arvore *no) {

	if (no == NULL) {

		return 0;

	}else if (folha(no)) {

		return 1;

	}else{

		return Arvore_tamanho_rec(no->direita) + Arvore_tamanho_rec(no->esquerda) + 1;		

	}

}

unsigned int Arvore_tamanho (Arvore *arvore) {

	if (arvore == NULL) {
		return 0;
	}

	return Arvore_tamanho_rec(arvore->raiz);
}

//retorna pilha com o caminho necessario para alcancar char a partir da raiz da arvore
Pilha* Arvore_caminho (Arvore *arvore, char c) {

	if (arvore == NULL || arvore->raiz == NULL) {
		return NULL;
	}

	//pilha com nos que fazem parte do caminho
	Pilha visitado;
	Pilha *caminho = (Pilha*)malloc(sizeof(Pilha));
	Pilha *caminho_b = (Pilha*)malloc(sizeof(Pilha));

	//no sendo verificado
	No_Arvore *verificando;

	//inicializacao das pilhas
	Pilha_inicializar(&visitado, Arvore_tamanho(arvore));
	Pilha_inicializar(caminho, Arvore_altura(arvore)+1);
	Pilha_inicializar(caminho_b, caminho->tamanho-1);

	//depth-first search
	Pilha_push(caminho, arvore->raiz);
	verificando = arvore->raiz;
	while (caminho->fim > 0 && verificando->c != c) {

		if ( !(naPilha(&visitado, verificando)) ) {
			Pilha_push(&visitado, verificando);
		}

		if ( !(naPilha(&visitado, verificando->direita)) && verificando->direita != NULL) {

			Pilha_push(caminho, verificando->direita);
			Pilha_push(caminho_b, (No_Arvore*)DIREITA);

		}
		else if ( !(naPilha(&visitado, verificando->esquerda)) && verificando->esquerda != NULL) {

			Pilha_push(caminho, verificando->esquerda);
			Pilha_push(caminho_b, (No_Arvore*)ESQUERDA);

		}
		else{

			Pilha_pop(caminho);
			Pilha_pop(caminho_b);

		}

		verificando = Pilha_topo(caminho);

	}

	Pilha_free(caminho);
	//Pilha_free(caminho_b);

	return caminho_b;

}

/*TESTE*/
/*
char c = 'a';
void main_preenche(No_Arvore *no, unsigned int freq, unsigned int i) {

	if (i > 0) {

		c++;
		no->direita = Arvore_novaRaiz(c, freq+1, NULL, NULL);
		c++;
		no->esquerda = Arvore_novaRaiz(c, freq+2, NULL, NULL);
		main_preenche(no->direita, freq+1, i-1);
		main_preenche(no->esquerda, freq+2, i-1);

	}

}

void main_printCaminho(FILE *saida, Pilha *pilha) {

	unsigned int i;

	for (i = 0; i < pilha->fim; i++) {
		fprintf(saida, "%ld ", (long int)pilha->vetor[i]);
	}

	fprintf(saida, "\n");

}

int main (void) {

	unsigned int i;

	Arvore teste;
	Pilha *caminho;

	Arvore_inicializar(&teste);

	teste.raiz = Arvore_novaRaiz('a', 1, NULL, NULL);

	main_preenche(teste.raiz, 1, TESTE_TAM);
	Arvore_imprimir(stderr, &teste);

	//fprintf (stdout, "%u\n", Arvore_altura(&teste));
	//fprintf (stdout, "%u\n", Arvore_tamanho(&teste));

	caminho = Arvore_caminho(&teste, 'm');
	//Pilha_imprimir(stderr, caminho);
	main_printCaminho(stderr, caminho);

	Arvore_esvaziar(&teste);
	Arvore_imprimir(stderr, &teste);

	Pilha_free(caminho);
	free(caminho);

	return 0;

}
*/
