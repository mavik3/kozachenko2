#include<stdio.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0

typedef struct {
    int **matica;
    int size;
    char transit;
}NURELM;
int **create_matica(int n){
    int **matica = malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++){
        matica[i] = calloc(n, sizeof(int));
    }
    return matica;
}

char nurelm_test_transitivity(NURELM *to_test){

}

void free_matica(int **matica, int n){
    for(int i = 0; i < n; i++){
        free(matica[i]);
    }
    free(matica);
}