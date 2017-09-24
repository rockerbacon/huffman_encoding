#ifndef LISTA_H
#define LISTA_H

#include "bool.h"
#include "arvore.h"
#include "pilha.h"

/*DEFINICAO LISTA*/
typedef struct _No_Lista {

	No_Arvore *no;
	Pilha *caminho;
	struct _No_Lista *proximo;
	struct _No_Lista *anterior;

} No_Lista;

typedef struct _Lista {

	No_Lista *inicio;
	No_Lista *fim;
	unsigned int tamanho;

} Lista;

/*OPERACOES LISTA*/
//inicializar lista
extern void Lista_inicializar (Lista *lista);

//adicionar no no final lista
extern bool Lista_adicionar (Lista *lista, No_Arvore *no, Pilha *caminho);
//adicionar no no inicio da lista
extern bool Lista_adicionarInicio (Lista *lista, No_Arvore *no, Pilha *caminho);
//adicionar no de forma ordenada em uma lista
extern bool Lista_adicionarOrd (Lista *lista, No_Arvore *no, Pilha *caminho);

//retorna elemento na lista que contenha o no
extern No_Lista* Lista_buscarNo (Lista *lista, No_Arvore *no);
//retorna elemento na lista cujo no contenha o char c
extern No_Lista* Lista_buscarChar (Lista *lista, char c);

//remove elemento na lista que contenha o no
extern bool Lista_remover (Lista *lista, No_Arvore *no);

//remove o ultimo elemento da lista
extern No_Lista* Lista_pop (Lista *lista);

//esvaziar lista
extern void Lista_esvaziar (Lista *lista);

//incrementa freq do no do elemento que contenha char c na lista, adiciona no com o elemento caso lista nao o contenha
extern bool Lista_incrementar (Lista *lista, char c);

//imprimir lista na saida desejada
extern void Lista_imprimir (FILE *saida, Lista *lista);

//ordena lista em ordem crescente de freq dos nos (Insertion sort)
extern bool Lista_ordenarFreq (Lista *lista);

/*DEFINICAO ALFABETO*/
typedef struct _Letra {

	char c;
	unsigned char bits;
	unsigned char *codigo;

} Letra;

typedef struct _Alfabeto {

	Letra **letra;
	unsigned char tamanho;
	unsigned char fim;

} Alfabeto;

/*OPERACOES ALFABETO*/
//inicializar alfabeto
extern bool Alfabeto_inicializar(Alfabeto *alfabeto, unsigned char tamanho);

//liberar espaco alocado pelo alfabeto
extern void Alfabeto_free(Alfabeto *alfabeto, bool limpar);

//adicionar letra no fim do alfabeto
extern bool Alfabeto_adicionar (Alfabeto *alfabeto, Letra *letra);

//remover letra do alfabeto
extern bool Alfabeto_remover (Alfabeto *alfabeto, Letra *letra);

//imprimir alfabeto
extern void Alfabeto_imprimir (FILE *saida, Alfabeto *alfabeto);

#endif
