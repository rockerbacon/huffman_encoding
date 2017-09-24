CC = gcc
objetos = arvore.o lista.o pilha.o huffman.o
flags = -I.
nome_compactador = rolo_compressor
main_compactador = compactador
nome_descompactador = rolo_descompressor
main_descompactador = descompactador

null:

$(nome_compactador): $(objetos)
	$(CC) -c $(main_compactador).c
	$(CC) $(objetos) $(main_compactador).o -o $@ $(flags)
	make clean

$(nome_descompactador): $(objetos)
	$(CC) -c $(main_descompactador).c
	$(CC) $(objetos) $(main_descompactador).o -o $@ $(flags)
	make clean

%.o: %.c
	$(CC) -c $<

clean:
	rm -f $(objetos) $(main_compactador).o $(main_descompactador).o

