#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

//#define TESTE "aaaabbbccd"

/*OPERACOES LISTA*/
//inicializacao da lista
void Lista_inicializar (Lista *lista) {

	if (lista == NULL) {
		return;
	}

	lista->inicio = NULL;
	lista->fim = NULL;
	lista->tamanho = 0;

}

//adicionar no na fim da lista
bool Lista_adicionar (Lista *lista, No_Arvore *no, Pilha *caminho) {

	if (lista == NULL) {
		return false;
	}

	No_Lista *add = (No_Lista*)malloc(sizeof(No_Lista));
	if (add == NULL) {
		return false;
	}

	add->no = no;
	add->caminho = caminho;
	add->proximo = NULL;
	add->anterior = lista->fim;

	if (lista->tamanho > 0) {
		lista->fim->proximo = add;
	} else {
		lista->inicio = add;
	}

	lista->fim = add;
	lista->tamanho++;

	return true;

}
//adicionar no no inicio da lista
bool Lista_adicionarInicio (Lista *lista, No_Arvore *no, Pilha *caminho) {

	if (lista == NULL) {
		return false;
	}

	No_Lista *add = (No_Lista*)malloc(sizeof(No_Lista));
	if (add == NULL) {
		return false;
	}

	add->no = no;
	add->caminho = caminho;
	add->anterior = NULL;
	add->proximo = lista->inicio;

	if (lista->tamanho > 0) {
		lista->inicio->anterior = add;
	} else {
		lista->fim = add;
	}

	lista->inicio = add;
	lista->tamanho++;

	return true;

}
//adicionar no de maneira ordenada
bool Lista_adicionarOrd (Lista *lista, No_Arvore *no, Pilha *caminho) {

	if (lista == NULL || no == NULL) {
		return false;
	}

	No_Lista *cursor;
	No_Lista *add = (No_Lista*)malloc(sizeof(No_Lista));
	if (add == NULL) {
		return false;
	}

	add->no = no;
	add->caminho = caminho;

	if (lista->tamanho > 0) {

		//encontra cursor onde cursor eh menor que no
		cursor = lista->fim;
		while (cursor != NULL) {
			if (cursor->no->freq > no->freq) {
				cursor = cursor->anterior;
			}else{
				break;
			}

		}

		//insere add depois do cursor
		add->anterior = cursor;
		if (cursor != NULL) {
			add->proximo = cursor->proximo;
			if (cursor != lista->fim) {
				cursor->proximo->anterior = add;
			}else{
				lista->fim = add;
			}
			cursor->proximo = add;
		}else{
			add->proximo = lista->inicio;
			lista->inicio->anterior = add;
			lista->inicio = add;
		}

	}else{

		add->proximo = NULL;
		add->anterior = NULL;
		lista->inicio = add;
		lista->fim = add;

	}

	lista->tamanho++;

	return true;

}

//buscar elemento na lista
No_Lista* Lista_buscarNo (Lista *lista, No_Arvore *no) {

	if (lista == NULL) {
		return NULL;
	}

	No_Lista *cursor;

	cursor = lista->inicio;

	while (cursor != NULL) {

		if (cursor->no == no) {
			break;
		}

		cursor = cursor->proximo;

	}

	return cursor;

}
//buscar elemento na lista
No_Lista* Lista_buscarChar (Lista *lista, char c) {

	if (lista == NULL) {
		return NULL;
	}

	No_Lista *cursor;

	cursor = lista->inicio;

	while (cursor != NULL) {

		if (cursor->no->c == c) {
			break;
		}

		cursor = cursor->proximo;

	}

	return cursor;

}


//remover elemento na lista
bool Lista_remover (Lista *lista, No_Arvore *no) {

	if (lista == NULL || no == NULL) {
		return false;
	}

	No_Lista *remove = Lista_buscarNo(lista, no);

	if (remove == NULL) {
		return false;
	}

	if (lista->tamanho == 1) {

		lista->inicio = NULL;
		lista->fim = NULL;

	}

	else if (remove == lista->fim) {

		remove->anterior->proximo = NULL;
		lista->fim = remove->anterior;

	}

	else if (remove == lista->inicio) {

		remove->proximo->anterior = NULL;
		lista->inicio = remove->proximo;

	}

	else {

		remove->proximo->anterior = remove->anterior;
		remove->anterior->proximo = remove->proximo;

	}

	lista->tamanho--;
	
	return true;

}

//remove o ultimo elemento da lista
No_Lista* Lista_pop (Lista *lista) {

	if (lista == NULL || lista->tamanho == 0) {
		return NULL;
	}

	No_Lista *remove = lista->fim;
	lista->fim = remove->anterior;

	if (lista->tamanho > 1) {
		remove->anterior->proximo = NULL;
	}else{
		lista->inicio = NULL;
	}

	lista->tamanho--;

	return remove;

}

//esvaziar lista
void Lista_esvaziar (Lista *lista) {

	No_Lista *remove;

	while ( remove = Lista_pop(lista) ) {
		Pilha_free(remove->caminho);
		free(remove);
	}

}

//incrementar frequencia de um elemento na lista, adiciona elemento caso nao exista na lista
bool Lista_incrementar (Lista *lista, char c) {

	if (lista == NULL) {
		return false;
	}

	No_Lista *no = Lista_buscarChar(lista, c);

	if (no != NULL) {
		no->no->freq++;
	}else{
		No_Arvore *add = Arvore_novaRaiz(c, 1, NULL, NULL);
		Lista_adicionar(lista, add, NULL);
	}

	return true;

}

//imprimir lista na saida desejada (c e freq)
void Lista_imprimir (FILE *saida, Lista *lista) {

	if (lista == NULL || saida == NULL) {
		return;
	}else if (lista->tamanho == 0){
		return;
	}

	No_Lista *cursor = lista->inicio;

	while (cursor != NULL) {

		fprintf (saida, "%c%u ", cursor->no->c, cursor->no->freq);

		cursor = cursor->proximo;

	}

	fprintf (saida, "\n");

}
//imprimir lista na saida desejada (c e caminho)
void Lista_imprimirCaminho (FILE *saida, Lista *lista, char separacao) {

	if (lista == NULL || saida == NULL) {
		return;
	}else if (lista->tamanho == 0){
		return;
	}

	No_Lista *cursor = lista->inicio;
	unsigned int i;

	while (cursor != NULL) {

		fprintf (saida, "%c", cursor->no->c);
		if (cursor->caminho != NULL) {
			for (i = 0; i < cursor->caminho->fim; i++) {
				fprintf (saida, "%lu", (unsigned long int)cursor->caminho->vetor[i]);
			}
		}
		fprintf (saida, "%c", separacao);

		cursor = cursor->proximo;

	}

}

//ordena lista de maneira crescente de acordo com as frequencias dos caracteres (insertion sort)
bool Lista_ordenarFreq (Lista *lista) {

	if (lista == NULL) {
		return false;
	}else if (lista->tamanho < 2) {
		return false;
	}

	No_Lista *cursorA, *cursorB, *retornoA;

	cursorA = lista->inicio->proximo;
	while (cursorA != NULL) {

		cursorB = cursorA->anterior;
		retornoA = cursorA->proximo;

		//encontra B onde B eh menor que A 
		while (cursorB != NULL) {
			if (cursorB->no->freq > cursorA->no->freq) {
				cursorB = cursorB->anterior;
			}else{
				break;
			}

		}

		//remove A de sua posicao
		cursorA->anterior->proximo = cursorA->proximo;
		if (cursorA == lista->fim) {
			lista->fim = cursorA->anterior;
		}else{
			cursorA->proximo->anterior = cursorA->anterior;
		}
		//Lista_imprimir(stdout, lista);		//debug
		//inserir A depois de B
		cursorA->anterior = cursorB;
		if (cursorB != NULL) {
			if (cursorB->proximo != cursorA) {
				cursorA->proximo = cursorB->proximo;
				cursorB->proximo->anterior = cursorA;
				cursorB->proximo = cursorA;
			}
		}else{
			cursorA->proximo = lista->inicio;
			lista->inicio->anterior = cursorA;
			lista->inicio = cursorA;
		}
		//Lista_imprimir(stdout, lista);		//debug

		cursorA = retornoA;

	}
	

}

/*OPERACOES ALFABETO*/
//inicializar alfabeto
bool Alfabeto_inicializar (Alfabeto *alfabeto, unsigned char tamanho) {

	if (alfabeto == NULL || tamanho == 0) {
		return false;
	}

	alfabeto->letra = (Letra**)calloc(tamanho, sizeof(Letra*));
	if (alfabeto->letra == NULL) {
		return false;
	}
	alfabeto->tamanho = tamanho;
	alfabeto->fim = 0;

	return true;

}

//liberar espaco alocado pelo alfabeto
void Alfabeto_free (Alfabeto *alfabeto, bool limpar) {

	if (alfabeto == NULL) {
		return;
	}

	if (limpar == true) {
		unsigned char i;
		for (i = 0; i < alfabeto->fim; i++) {
			free(alfabeto->letra[i]);
		}
	}

	free(alfabeto->letra);
	alfabeto->tamanho = 0;
	alfabeto->fim = 0;

}

//adicionar letra no fim alfabeto
bool Alfabeto_adicionar (Alfabeto *alfabeto, Letra *letra) {

	if (alfabeto == NULL || letra == NULL) {
		return false;
	} else if (alfabeto->letra == NULL || alfabeto->fim == alfabeto->tamanho) {
		return false;
	}

	alfabeto->letra[alfabeto->fim] = letra;
	alfabeto->fim++;

	return true;

}

//remover letra do alfabeto
bool Alfabeto_remover (Alfabeto *alfabeto, Letra *letra) {

	if (alfabeto == NULL || letra == NULL) {
		return false;
	} else if (alfabeto->letra == NULL) {
		return false;
	}

	unsigned char i;

	i = 0;
	while (alfabeto->letra[i] != letra) {
		i++;
		if (i == alfabeto->tamanho) {
			return false;
		}
	}
	for ( ; i < alfabeto->fim-1; i++) {
		alfabeto->letra[i] = alfabeto->letra[i+1];
	}

	alfabeto->fim--;

	return true;

}

//imprimir alfabeto na saida desejada
void Alfabeto_imprimir (FILE *saida, Alfabeto *alfabeto) {

	unsigned char i, j;

	for (i = 0; i < alfabeto->fim; i++) {
		fprintf (saida, "%c ", alfabeto->letra[i]->c);
		for (j = 0; j < alfabeto->letra[i]->bits; j++) {
			if (alfabeto->letra[i]->codigo[j] != 0) {
				fprintf (saida, "1");
			} else {
				fprintf (saida, "0");
			}
		}
		fprintf (saida, " ");
	}
	fprintf (saida, "\n");

}

//teste
/*
int main (void) {

	Lista teste;

	Lista_inicializar(&teste);

	fprintf(stdout, "%s\n", TESTE);

	Lista_fromString(&teste, TESTE);

	Lista_imprimir(stdout, &teste);

	Lista_ordenarFreq (&teste);

	Lista_imprimir(stdout, &teste);

	Lista_remover(&teste, teste.inicio->proximo->proximo->c);

	Lista_imprimir(stdout, &teste);

	Lista_remover(&teste, teste.inicio->c);

	Lista_imprimir(stdout, &teste);

	Lista_remover(&teste, teste.fim->c);

	Lista_imprimir(stdout, &teste);

	Lista_esvaziar(&teste);

	Lista_imprimir(stdout, &teste);

	return 0;
}
*/
