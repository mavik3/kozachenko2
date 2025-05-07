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
    int **matica_vytv = malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++){
        matica_vytv[i] = calloc(n, sizeof(int));
    }
    return matica_vytv;
}

char nurelm_test_transitivity(NURELM *to_test)
{
int n = to_test -> size;
int **R = to_test -> matica;

for (int i = 0; i < n; i++)
{
    for(int j = 0; j < n; j++)
    {
        for(int k = 0; k < n; k++)
        {
            if(R[i][k] && R[j][k])
            {
                to_test ->transit = FALSE;
                return FALSE;
            }
        }
    }
}

to_test -> transit = TRUE;
return TRUE;
}
void free_matica(int **matica_vytv, int n){
    for(int i = 0; i < n; i++){
        free(matica_vytv[i]);
    }
    free(matica_vytv);
}