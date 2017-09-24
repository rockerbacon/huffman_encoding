#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "lista.h"
#include "huffman.h"

//flags para definir tamanho do codigo gerado
#define FLAGS		4
const unsigned char FLAG_SIZE[FLAGS] = {
						sizeof(unsigned char)*8,
						sizeof(unsigned short int)*8,
						sizeof(unsigned int)*8,
						sizeof(unsigned long int)*8
					 };	

//#define TESTE_HUFFMAN_STRING "Hello World"
//#define TESTE_HUFFMAN_ENTRADA "hello.txt"
//#define TESTE_HUFFMAN_SAIDA "hello.izi"

static unsigned long int rangeof_u (unsigned char bytes) {

	if (bytes > sizeof(unsigned long int) || bytes == 0) {
		return 0;
	}

	//range de um byte
	static const byte_range = (1<<8)-1;

	unsigned long int range;

	range = byte_range;
	bytes--;
	while (bytes > 0) {
		range <<= 8;
		range |= byte_range;
		bytes--;
	}

	return range;

}

//verificar se nome possui extensao valida
bool Huffman_extensaoValida(char *nome_arquivo) {

	unsigned int i;

	i = 0;
	//ESTADO INICIAL
	while (nome_arquivo[i] != '.') {
		if (nome_arquivo == '\0') {
			return false;
		}
		i++;
	}
	i++;
	//ESTADO Q1
	if (nome_arquivo[i] != EXTENSAO[0]) {
		return false;
	}
	i++;
	//ESTADO Q2
	if (nome_arquivo[i] != EXTENSAO[1]) {
		return false;
	}
	i++;
	//ESTADO Q3
	if (nome_arquivo[i] != EXTENSAO[2]) {
		return false;
	}
	//ESTADO Q4
	i++;
	if (nome_arquivo[i] != '\0') {
		return false;
	}

	return true;

}

//gera lista a partir de string
static unsigned long int gerarLista (Lista *lista, FILE *arquivo, char *nome_arquivo) {

	if (lista == NULL || arquivo == NULL || nome_arquivo == NULL) {
		return 0;
	}else if (lista->tamanho > 0 || nome_arquivo[0] == '\0') {
		return 0;
	}

	char c;
	unsigned int i;
	unsigned long int tamanho;

	i = 0;
	while (nome_arquivo[i] != '\0') {
		if ( !(Lista_incrementar(lista, nome_arquivo[i])) ) {
			return false;
		}
		i++;
	}
	tamanho = 0;
	while (fscanf(arquivo, "%c", &c) != EOF) {
		if ( !(Lista_incrementar(lista, c)) ) {
			return false;
		}
		tamanho++;
	}

	return tamanho;

}

//gera arvore a partir de lista de frequencia de caracteres
static No_Arvore* gerarArvore (Lista *lista) {

	if (lista->tamanho == 1) {
		return lista->inicio->no;
	}

	No_Arvore *filho_esquerdo,
		  *filho_direito,
		  *pai;
	Lista *alfabeto = (Lista*)malloc(sizeof(Lista));

	Lista_inicializar(alfabeto);

	//ordena lista por ordem de frequencia
	Lista_ordenarFreq(lista);
	//Lista_imprimir(stdout, lista);	//debug
	while (lista->tamanho > 1) {

		//criar no pai que tenha como filhos os dois nos com menor frequencia
		filho_esquerdo = lista->inicio->no;
		filho_direito = lista->inicio->proximo->no;
		pai = Arvore_novaRaiz(0, filho_esquerdo->freq + filho_direito->freq, filho_esquerdo, filho_direito);

		//remover nos filhos da lista
		Lista_remover(lista, filho_esquerdo);
		Lista_remover(lista, filho_direito);
		//Lista_imprimir(stdout, lista);	//debug

		if (folha(filho_esquerdo)) {
			Lista_adicionarInicio(alfabeto, filho_esquerdo, NULL);
		}
		if (folha(filho_direito)) {
			Lista_adicionarInicio(alfabeto, filho_direito, NULL);
		}

		//adiciona no pai na lista de maneira ordenada
		Lista_adicionarOrd(lista, pai, NULL);

		//Lista_imprimir(stdout, lista);	//debug

	}

	//copia alfabeto para lista
	Lista_esvaziar(lista);
	lista->inicio = alfabeto->inicio;
	lista->fim = alfabeto->fim;
	lista->tamanho = alfabeto->tamanho;
	free(alfabeto);

	return pai;

}

//gerar alfabeto com base na arvore de huffman
static void gerarAlfabeto (Lista *lista, Arvore *arvore) {

	No_Lista *cursor;

	//gera caminho para os caracteres do alfabeto contando os elementos de acordo com o numero de bits necessarios para representa-lo
	cursor = lista->inicio;
	while (cursor != NULL) {
		cursor->caminho = Arvore_caminho(arvore, cursor->no->c);
		cursor = cursor->proximo;
	}

}

//le caminho escrevendo-o como numero no arquivo de saida, retorna o numero de bytes escritos
static unsigned int escreverCod(Pilha *caminho, unsigned char *byte, unsigned char *bit_count, FILE *saida) {

	unsigned int byte_count;
	unsigned char i;

	//percorre caminho para char a ser escrito
	byte_count = 0;
	for (i = 0; i < caminho->fim; i++) {

		//escreve bit a bit em um unsigned char
		if ((unsigned long int)caminho->vetor[i] != 0) {
			*byte |= 1<<FLAG_SIZE[0]-1-(*bit_count);
		}
		(*bit_count)++;
		//se preencheu 1 byte escreve no arquivo e passa para o proximo byte
		if (*bit_count == FLAG_SIZE[0]) {
			fwrite(byte, sizeof(unsigned char), 1, saida);
			*byte = 0;
			*bit_count = 0;
			byte_count++;
		}

	}

	return byte_count;

}

static void gerarSaidaCod(Lista *alfabeto, char *nome_entrada, unsigned char nome_entrada_tam, FILE *entrada, unsigned long int entrada_tam, FILE *saida) {

	char leitura;
	unsigned int i;
	No_Lista *cursor;
	unsigned char	escrita,
			bit_count;

	//escreve tamanho do alfabeto no arquivo
	fwrite(&alfabeto->tamanho, sizeof(unsigned char), 1, saida);

	//escreve alfabeto no arquivo
	//escreve caracteres no arquivo
	for (cursor = alfabeto->inicio; cursor != NULL; cursor = cursor->proximo) {
		fwrite(&cursor->no->c, sizeof(char), 1, saida);
	}
	//escreve numero de bits necessarios para representar os caracteres
	for (cursor = alfabeto->inicio; cursor != NULL; cursor = cursor->proximo) {
		fwrite(&cursor->caminho->fim, sizeof(unsigned char), 1, saida);
	}
	//escreve codigos dos caracteres
	escrita = 0;
	bit_count = 0;
	for (cursor = alfabeto->inicio; cursor != NULL; cursor = cursor->proximo) {
		escreverCod(cursor->caminho, &escrita, &bit_count, saida);
	}
	if (escrita != 0) {
		fwrite(&escrita, sizeof(unsigned char), 1, saida);
		escrita = 0;
		bit_count = 0;
	}

	//escreve nome original do arquivo de forma comprimida
	//escreve tamanho da string com o nome do arquivo
	fwrite(&nome_entrada_tam, sizeof(unsigned char), 1, saida);
	//escreve nome do arquivo
	for (i = 0; i < nome_entrada_tam; i++) {
		//procura char no alfabeto
		cursor = Lista_buscarChar(alfabeto, nome_entrada[i]);
		//escreve no arquivo
		escreverCod(cursor->caminho, &escrita, &bit_count, saida);
	}
	//se existe um byte que nao foi totalmente escrito
	if (escrita != 0) {
		fwrite(&escrita, sizeof(unsigned char), 1, saida);
		escrita = 0;
		bit_count = 0;
	}

	//escreve texto da entrada no arquivo
	//determina numero de bits necessarios para representar numero de caracteres do texto
	i = 0;
	while (entrada_tam > rangeof_u(FLAG_SIZE[i]/8)) {
		i++;
	}
	//escreve numero de bits necessarios para representar numero de caracteres do texto
	fwrite(&FLAG_SIZE[i], sizeof(unsigned char), 1, saida);
	//escreve numero de caracteres do texto
	fwrite(&entrada_tam, FLAG_SIZE[i]/8, 1, saida);

	//escrever texto da entrada na saida
	while (fscanf(entrada, "%c", &leitura) != EOF) {
		//procurar char no alfabeto
		cursor = Lista_buscarChar(alfabeto, leitura);
		//escreve no arquivo
		escreverCod(cursor->caminho, &escrita, &bit_count, saida);
	}
	//se existe um byte que nao foi totalmente escrito
	if (escrita != 0) {
		fwrite(&escrita, sizeof(unsigned char), 1, saida);
	}
	
}

//comprimir arquivo
void Huffman_comprimir(char *nome_entrada, char *nome_saida) {

	unsigned char nome_entrada_tam;

	if (nome_entrada == NULL || nome_saida == NULL) {
		printf ("Erro: arquivo de entrada ou de saida nao especificado(s)\n");
		return;
	}else{
		unsigned int i;
		for (i = 0; nome_entrada[i] != '\0'; i++);
		if (i > 255) {
			printf ("Erro: tamanho do nome do arquivo excede o tamanho limite(255)");
			return;
		}else{
			nome_entrada_tam = (unsigned char)i;
		}
	}

	Lista *lista;
	Arvore arvore;
	unsigned long int entrada_tam;
	FILE *entrada, *saida;

	//abrir arquivos
	entrada = fopen(nome_entrada, "r");
	if (entrada == NULL) {
		printf ("Nao foi possivel abrir arquivo %s\n", nome_entrada);
		return;
	}
	saida = fopen(nome_saida, "w");
	if (saida == NULL) {
		printf ("Nao foi possivel criar arquivo %s\n", nome_saida);
		return;
	}

	//inicializa lista de frequencia dos caracteres
	lista = (Lista*)malloc(sizeof(Lista));
	Lista_inicializar(lista);
	//inicializar arvore
	Arvore_inicializar(&arvore);

	//gerar lista de frequencias a partir do arquivo
	entrada_tam = gerarLista (lista, entrada, nome_entrada);
	//printf ("%lu\n", entrada_tam);		//debug
	//Lista_imprimir(stdout, lista);		//debug

	//gerar arvore de acordo com a lista
	arvore.raiz = gerarArvore(lista);
	//Arvore_imprimir(stdout, &arvore);	//debug;
	//Lista_imprimir(stdout, lista);		//debug;

	//gerar alfabeto com base na arvore
	gerarAlfabeto(lista, &arvore);
	//Lista_imprimirCaminho(stdout, lista, '\n');	//debug

	//escrever dados comprimidos no arquivo
	rewind(entrada);
	gerarSaidaCod(lista, nome_entrada, nome_entrada_tam, entrada, entrada_tam, saida);

	//liberar espaco alocado
	Arvore_esvaziar(&arvore);
	Lista_esvaziar(lista);
	free(lista);
	fclose(entrada);
	fclose(saida);

}

//descodifica e retorna char a partir da entrada
static char lerEntradaCod(Alfabeto *alfabeto, unsigned char *byte, unsigned char *bit_count, FILE *entrada) {

	char c;
	static const unsigned char check = 1<<7;
	unsigned char	bit,
			bit_cod,
			i;
	Alfabeto verificar;

	Alfabeto_inicializar(&verificar, alfabeto->tamanho);

	//adiciona todos os elementos do alfabeto para serem verificados
	for (i = 0; i < alfabeto->fim; i++) {
		Alfabeto_adicionar(&verificar, alfabeto->letra[i]);
	}

	//Alfabeto_imprimir(stdout, &verificar);	//debug
	bit_cod = 0;
	while (verificar.fim > 1) {

		//le proximo bit e remove todas as impossibilidades do alfabeto verificar
		bit = *byte & check;
		i = 0;
		while (i < verificar.fim) {
			//printf ("%hhu %hhu %c %hhu %hhu\n", bit_cod, *bit_count, verificar.letra[i]->c, verificar.letra[i]->codigo[bit_cod], bit);
			if (verificar.letra[i]->codigo[bit_cod] != bit) {
				Alfabeto_remover(&verificar, verificar.letra[i]);
			} else {
				i++;
			}

		}
		(*bit_count)++;
		bit_cod++;

		//se terminou de ler o byte atual le novo byte
		if (*bit_count == 8) {
			fread(byte, sizeof(unsigned char), 1, entrada);
			*bit_count = 0;
		} else {
			*byte <<= 1;
		}
		//Alfabeto_imprimir(stdout, &verificar);	//debug

	}
	c = verificar.letra[0]->c;

	Alfabeto_free(&verificar, false);

	return c;

}

//descomprimir arquivo
void Huffman_descomprimir(char *nome_entrada) {

	FILE *entrada,
	     *saida;
	char	*nome_saida;
	Alfabeto alfabeto;
	unsigned char	flag,
			byte,
			bit_count;
	unsigned long int	i, j,
				tamanho_arquivo;

	entrada = fopen(nome_entrada, "rb");
	if (entrada == NULL) {
		printf ("Erro: nao foi possivel abrir arquivo %s\n", nome_entrada);
		return;
	}

	//ler numero de elementos do alfabeto
	fread(&flag, sizeof(unsigned char), 1, entrada);
	Alfabeto_inicializar(&alfabeto, flag);
	//ler caracteres do alfabeto
	alfabeto.fim = flag;
	for (i = 0; i < alfabeto.tamanho; i++) {
		alfabeto.letra[i] = (Letra*)malloc(sizeof(Letra));
		fread(&alfabeto.letra[i]->c, sizeof(char), 1, entrada);
	}
	//ler numero de bits dos caracteres do alfabeto
	for (i = 0; i < alfabeto.tamanho; i++) {
		fread(&alfabeto.letra[i]->bits, sizeof(char), 1, entrada);
	}
	//ler codigo dos caracteres do alfabeto
	fread(&byte, sizeof (unsigned char), 1, entrada);
	bit_count = 0;
	flag = 1<<7;
	for (i = 0; i < alfabeto.tamanho; i++) {
		alfabeto.letra[i]->codigo = (unsigned char*)malloc(alfabeto.letra[i]->bits*sizeof(unsigned char));
		for (j = 0; j < alfabeto.letra[i]->bits; j++) {
			alfabeto.letra[i]->codigo[j] = flag & byte;
			bit_count++;
			if (bit_count == 8) {
				fread(&byte, sizeof(unsigned char), 1, entrada);
				bit_count = 0;
			} else {
				byte <<= 1;
			}
		}
	}
	//Alfabeto_imprimir(stdout, &alfabeto);	//debug

	//ler tamanho do nome original do arquivo
	fread(&flag, sizeof(unsigned char), 1, entrada);
	//ler nome do arquivo
	nome_saida = (char*)malloc((flag+1)*sizeof(char));
	fread(&byte, sizeof(unsigned char), 1, entrada);
	bit_count = 0;
	for (i = 0; i < flag; i++) {
		nome_saida[i] = lerEntradaCod(&alfabeto, &byte, &bit_count, entrada);
		//printf ("%lu %c\n", i, nome_saida[i]);	//debug
	}
	nome_saida[i] = '\0';
	//printf ("%s\n", nome_saida);	//debug

	//abrir arquivo de saida
	saida = fopen(nome_saida, "w");
	if (saida == NULL) {
		printf ("Erro: nao foi possivel criar arquivo %s\n", nome_saida);
		return;
	}
	//ler numero de caracteres do arquivo
	fread(&flag, sizeof(unsigned char), 1, entrada);
	tamanho_arquivo = 0;
	fread(&tamanho_arquivo, flag/8, 1, entrada);
	fread(&byte, sizeof(unsigned char), 1, entrada);
	bit_count = 0;
	for (i = 0; i < tamanho_arquivo; i++) {
		fprintf(saida, "%c", lerEntradaCod(&alfabeto, &byte, &bit_count, entrada));
	}

	Alfabeto_free(&alfabeto, true);
	fclose(entrada);
	fclose(saida);
		
}

/*TESTE*/
/*
int main (void) {

	FILE *entrada,
	     *saida;

	//escreve string de teste no arquivo
	entrada = fopen(TESTE_HUFFMAN_ENTRADA, "w");
	fprintf (entrada, "%s", TESTE_HUFFMAN_STRING);
	fclose(entrada);

	//abre os arquivos de entrada e saida
	entrada = fopen(TESTE_HUFFMAN_ENTRADA, "r");
	saida = fopen(TESTE_HUFFMAN_SAIDA, "wb");

	Huffman_comprimir(entrada, saida, TESTE_HUFFMAN_ENTRADA);

	fclose(entrada);
	fclose(saida);

	return 0;	

}
*/
