#include "headers/KNN_Classifiers.h"
#include "headers/KNN_Dimension.h"
#include "headers/KNN_Distance.h"
#include "headers/KNN_FileManager.h"
#include "headers/KNN_Matrix.h"
#include "headers/KNN_Vector.h"
#include <stdlib.h>
#include <stdio.h>

void runCommands(Tcommand_data* commands, Tcsv_data* training_content, Tcsv_data* test_content){
    int k = 0;
	int i = 0;
	int j = 0;

	int vet_len = 0;

	int correct = 0;

	int test_rotule = 0;
	int *knn_rotule = create_I_Vector(training_content->map.lines);

	double acuracia = 0;

	double **m_test = splitNumbers(test_content, &vet_len);
	double **m_train = splitNumbers(training_content, &vet_len);

	double *dist = create_F_Vector(training_content->map.lines);

    for( k = 0 ; k < commands->map.lines ; k++ ){
		correct = 0;
        switch(commands->data[k].distance){
            case 'E':
                for( i = 0 ; i < test_content->map.lines ; i++ ){
					// SALVA O ROTULO DE TESTE REAL
					test_rotule = m_test[i][vet_len - 1];
					// PERCORRE TODAS AS LINHAS DO TREINO COMPARANDO
					// A DISTANCIA DO TESTE COM A DO TREINO
					for( j = 0 ; j < training_content->map.lines ; j++ ){
						dist[j] = euclidianDistance(m_test[i], m_train[j], vet_len - 1);
					}
					//kMinors(dist, training_content->map.lines, commands->data[k].k, knn_rotule);
					//if( recorrence(knn_rotule, commands->data[k].k) == test_rotule ) correct++;
				}
				//acuracia = Accuracy(correct, test_content->map.lines);
				printf("Acuracia %f\n", acuracia);
                break;
            case 'M':
                for( i = 0 ; i < test_content->map.lines ; i++ ){
					for( j = 0 ; j < training_content->map.lines ; j++ ){
						dist[j] = minkowskyDistance(m_test[i], m_train[j], vet_len -1, commands->data[k].r);
					}
					//kMinors(dist, training_content->map.lines, commands->data[k].k, knn_rotule);
					//if( recorrence(knn_rotule, commands->data[k].k) == test_rotule ) correct++;
				}
				//acuracia = Accuracy(correct, test_content->map.lines);
				printf("Acuracia %f\n", acuracia);
                break;
            case 'C':
                for( i = 0 ; i < test_content->map.lines ; i++ ){
					for( j = 0 ; j < training_content->map.lines ; j++ ){
						dist[j] = chebyshevDistance(m_test[i], m_train[j], vet_len -1);
					}
					//kMinors(dist, training_content->map.lines, commands->data[k].k, knn_rotule);
					//if( recorrence(knn_rotule, commands->data[k].k) == test_rotule ) correct++;
				}
				//acuracia = Accuracy(correct, test_content->map.lines);
				printf("Acuracia %f\n", acuracia);
                break;
        }
		
    }

	freedoubleMatrix(m_test, test_content->map.lines);
	freedoubleMatrix(m_train, training_content->map.lines);
	free(dist);
	free(knn_rotule);
	
    printf("\n");

}

int main(void){

    FILE* config = openFile("config.txt", 'r');
    Tcommand_data* commands;

    char* training_path;
    char* test_path;
    char* predicts_path;

    Tcsv_data* training_content;
    Tcsv_data* test_content;

    training_path = readLineFile(config);
    test_path = readLineFile(config);
    predicts_path = readLineFile(config);

    closeFile(config);

    commands = readInstructions("config.txt");

    training_content = readFileToMatrix(training_path);
    test_content = readFileToMatrix(test_path);

    runCommands(commands, training_content, test_content);

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
