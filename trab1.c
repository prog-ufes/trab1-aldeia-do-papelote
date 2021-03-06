#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "headers/KNN_Classifiers.h"
#include "headers/KNN_Dimension.h"
#include "headers/KNN_Distance.h"
#include "headers/KNN_FileManager.h"
#include "headers/KNN_Matrix.h"
#include "headers/KNN_Vector.h"

void runCommands(Tcommand_data* commands, Tcsv_data* training_content, Tcsv_data* test_content, char *path_result){
    // Variaveis de controle de loops
	int i = 0;
	int j = 0;
	int k = 0;

	int recorrencia = 0;

	int vet_len = 0; // tamanho horizontal do vetor
	int correct = 0; // corretas da acuracia

	float acuracia = 0;

	int test_rotule = 0; // auxiliar para guardar o rotulo do teste atual
	int *knn_rotule; // vetor de k rotulos mais proximos
	int *index; // vetor de controle de indices

	// Variaveis que guardam quantos tipos de rotulos existem
	// para o teste e para o treino

	double **m_test = splitNumbers(test_content, &vet_len); // matriz de teste
	double **m_train = splitNumbers(training_content, &vet_len); // matriz de treino

	int distinct_train = distinctRotules(m_train, training_content->map.lines, vet_len);

	double *dist = create_F_Vector(training_content->map.lines); // vetor de distancias

	// Variveis para escrever as predicoes apos rodar o algoritmo
	Tcsv_map content_map;
	content_map.lines = distinct_train + 3 + test_content->map.lines;
	content_map.length_line = malloc( sizeof(int) * content_map.lines );
	content_map.length_line[0] = 5;
	for( i = 2 ; i < distinct_train ; i++ ){
		content_map.length_line[i] = (distinct_train * 10) + 1;
	}
	char **content = create_R_CharacterMatrix(content_map);
	for( i = 0 ; i < content_map.lines ; i++){
		strcpy(content[i], "\0");
	}
	char* path_result_f;

	TDimension confusion_dim;
	confusion_dim.x = distinct_train;
	confusion_dim.y = distinct_train;

	int** confusion = createMatrix(confusion_dim);


	// Algoritmo de KNN
	for( k = 0 ; k < commands->map.lines - 1 ; k++ ){
		index = create_I_Vector(commands->data[k].k);
		knn_rotule = create_I_Vector(commands->data[k].k);

		printf("Command: %d, %c, %.2f\n", commands->data[k].k, commands->data[k].distance, commands->data[k].r);
		correct = 0;

        for( i = 0 ; i < test_content->map.lines ; i++ ){
			// SALVA O ROTULO DE TESTE REAL
			test_rotule = m_test[i][vet_len - 1];

			// PERCORRE TODAS AS LINHAS DO TREINO COMPARANDO
			// A DISTANCIA DO TESTE COM A DO TREINO
			switch(commands->data[k].distance){
				case 'E':
					for( j = 0 ; j < training_content->map.lines ; j++ ){
						dist[j] = euclidianDistance(m_test[i], m_train[j], vet_len - 1);
					}
					break;
				case 'M':
					for( j = 0 ; j < training_content->map.lines ; j++){
						dist[j] = minkowskyDistance(m_test[i], m_train[j], vet_len - 1, commands->data[k].r);
					}
					break;
				case 'C':
					for( j = 0 ; j < training_content->map.lines ; j++){
						dist[j] = chebyshevDistance(m_test[i], m_train[j], vet_len - 1);
					}
					break;
			}

			kMinors(dist, training_content->map.lines, commands->data[k].k, index);

			for( j = 0 ; j < commands->data[k].k ; j++){
				knn_rotule[j] = m_train[index[j]][vet_len - 1];
			}

			recorrencia = recorrence(knn_rotule, commands->data[k].k);

			if( recorrencia == test_rotule )
				correct++;

			content[3 + distinct_train + i][0] = recorrencia + 47;
			confusion[test_rotule - 1][recorrencia - 1] += 1;
		}

		path_result_f = (char*) calloc(sizeof(char), strlen(path_result) + 50);

		acuracia = Accuracy(correct, test_content->map.lines);

		int ac = acuracia*100;
		// COLOCANDO O RESULTADO DA ACURACIA
		content[0][0] = '0';
		content[0][1] = '.';
		content[0][3] = (ac%10) + 48;
		ac /= 10;
		content[0][2] = (ac%10) + 48;
		content[0][4] = '\0';

		strcpy(path_result_f, path_result);
		strcat(path_result_f, "predicao_");
		
		char *num = (char*) calloc(sizeof(char), 5);
		sprintf(num, "%d", k + 1);
		strcat(path_result_f, num);
		strcat(path_result_f, ".txt");

		printMatrix(confusion, confusion_dim);

		free(num);

		//zerando a matriz de confusao
		for( i = 0 ; i < confusion_dim.x ; i++){
			strcpy(content[i + 2], "\0");
			for( j = 0 ; j < confusion_dim.y ; j++){
				num = (char*) calloc(sizeof(char), 5);
				sprintf(num, "%d", confusion[i][j]);
				strcat(content[i + 2], num);
				if( j != confusion_dim.y - 1 ) strcat(content[i + 2]," ");
				confusion[i][j] = 0;
				free(num);
			}
		}

		writeInFile(path_result_f, content, content_map.lines);

		
		free(path_result_f);
    	free(index);
		free(knn_rotule);
	}

	freedoubleMatrix(m_test, test_content->map.lines);
	freedoubleMatrix(m_train, training_content->map.lines);

	freeCharacterMatrix(content, content_map.lines);

	freeMatrix(confusion, confusion_dim.x);

	free(dist);
	free(content_map.length_line);

    printf("\n");
}

int main(void){

    FILE* config = openFile("config.txt", 'r');
    Tcommand_data* commands;

    char* training_path = NULL;
    char* test_path = NULL;
    char* predicts_path = NULL;

    Tcsv_data* training_content;
    Tcsv_data* test_content;

    training_path = readLineFile(config);
    test_path = readLineFile(config);
    predicts_path = readLineFile(config);

    closeFile(config);

    commands = readInstructions("config.txt");

    training_content = readFileToMatrix(training_path);
    test_content = readFileToMatrix(test_path);

    if(training_path != NULL && test_path != NULL && predicts_path != NULL)
    	runCommands(commands, training_content, test_content, predicts_path);
    else return 1;

    free(training_path);
    free(test_path);
    free(predicts_path);

    freeCharacterMatrix(training_content->data, training_content->map.lines);
    freeCharacterMatrix(test_content->data, test_content->map.lines);

    free(commands->map.length_line);
    freeCommands(commands);

    free(training_content->map.length_line);
    free(test_content->map.length_line);

    free(training_content);
    free(test_content);

    return 0;
}
