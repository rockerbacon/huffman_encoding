#ifndef PILHA_H
#define PILHA_H

#include "bool.h"

struct _No_Arvore;

/*TIPOS DE DADOS*/
typedef struct _Pilha {

	struct _No_Arvore **vetor;
	unsigned int fim;
	unsigned int tamanho;

} Pilha;

/*OPERACOES*/
//alocar espaco para pilha
//dataSize eh o tamanho do tipo de dado a ser utilizado (sizeof(char), para uma pilha de char, por exemplo)
//tamanho eh o numero maximo de elementos que a pilha comporta
extern void Pilha_inicializar (Pilha *pilha, unsigned int tamanho);

//empilhar c na pilha
extern bool Pilha_push (Pilha *pilha, struct _No_Arvore *no);

//desempilhar topo da pilha
extern struct _No_Arvore* Pilha_pop (Pilha *pilha);

//esvaziar pilha
extern void Pilha_esvaziar (Pilha *pilha);

//liberar espaco alocado pela pilha
extern void Pilha_free (Pilha *pilha);

//imprimir pilha na saida desejada
extern void Pilha_imprimir (FILE *saida, Pilha *pilha);

//verifica se no esta na pilha
extern bool naPilha (Pilha *pilha, struct _No_Arvore *no);

//retorna o no que esta no topo da pilha
extern struct _No_Arvore* Pilha_topo(Pilha *pilha);

#endif
