#ifndef ARVORE_H
#define ARVORE_H

#define DIREITA 1
#define ESQUERDA 0

#include "bool.h"

struct _Pilha;

/*DEFINICAO ARVORE*/
typedef struct _No_Arvore {

	char c;
	unsigned int freq;
	struct _No_Arvore *direita;
	struct _No_Arvore *esquerda;

} No_Arvore;

typedef struct _Arvore {

	No_Arvore *raiz;

} Arvore;

/*OPERACOES ARVORE*/
//iniciliza arvore
extern void Arvore_inicializar (Arvore *arvore);

//cria novo no para ser adicionado a uma arvore
extern No_Arvore* Arvore_novaRaiz(char c, unsigned int freq, No_Arvore *esquerda, No_Arvore *direita);

//verifica se no eh no folha
extern bool folha (No_Arvore *no);

//esvazia arvore
extern void Arvore_esvaziar (Arvore *arvore);

//imprime arvore na saida escolhida utilizando representacao com identacao
//nos mais a direita sao imprimidos antes dos nos mais a esquerda
extern void Arvore_imprimir (FILE *saida, Arvore *arvore);

//retorna altura da arvore
extern unsigned int Arvore_altura (Arvore *arvore);

//retorna pilha com o caminho necessario para alcancar no com o char desejado a partir do no raiz
//1 = direita, 0 = esquerda
extern struct _Pilha* Arvore_caminho (Arvore *arvore, char c);

//retorna numero de nos que arvore contem
extern unsigned int Arvore_tamanho (Arvore *arvore);

#endif
