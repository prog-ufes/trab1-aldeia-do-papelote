#include "../headers/KNN_FileManager.h"
#include "../headers/KNN_Dimension.h"
#include <stdlib.h>
#include <string.h>

// ABRE UM ARQUIVO DE ACORDO COM O MODO
FILE* openFile(char* name, char mode){
	switch( mode ){
		case 'w':
			return fopen(name, "wt");
		break;
		case 'r':
			return fopen(name, "rt");
		break;
		case 'a':
			return fopen(name, "a");
		break;
	}
}
// FECHA UM ARQUIVO
void closeFile(FILE* f){ fclose(f); }
// LE UMA LINHA DE UM ARQUIVO
char* readLineFile(FILE* f){
	char* path;
	char static_path[100];
	
	fscanf(f, "%s", static_path);
	
	path = (char*) malloc((sizeof(char)*strlen(static_path)) + sizeof(char));
	
	strcpy(path, static_path);
	
	return path;
}
// LE UM ARQUIVO INTEIRO E ARMAZENA NUM VETOR DE STRINGS
char** readArq(FILE* f){
	int n = lineNumberFile(f);
	int i = 0;
	char** content;
}
// MOSTRA O NUMERO DE LINHAS DE UM ARQUIVO
int lineNumberFile(FILE* f){
	int number = 0;
	int character;
	while((character=fgetc(f))!= EOF){
		if(character == '\n') number++;
	}
	return number;
}