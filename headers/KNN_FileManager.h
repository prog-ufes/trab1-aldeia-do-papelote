#ifndef _H_FILEMANAGER
#define _H_FILEMANAGER

// ABRE UM ARQUIVO DE ACORDO COM O MODO
FILE* openFile(char* name, char mode);
// FECHA UM ARQUIVO
void closeFile(FILE* f);

#endif