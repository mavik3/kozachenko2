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

char nurelm_test_transitivity(NURELM *to_cheak)
{
int n = to_cheak -> size;
int **R = to_cheak -> matica;

for (int i = 0; i < n; i++)
{
    for(int j = 0; j < n; j++)
    {
        if(R[i][j])
        {
            for(int k = 0; k < n; k++)
        {
            if(R[i][k] && R[j][k] && R[i][k])
            {
                to_cheak ->transit = FALSE;
                return FALSE;
            }
        }
    }
}
}
to_cheak -> transit = TRUE;
return TRUE;
}
void free_matica(int **matica_vytv, int n){
    for(int i = 0; i < n; i++){
        free(matica_vytv[i]);
    }
    free(matica_vytv);
}
int main()
{
    NURELM test;
    test.size = 3;
    test.matica = create_matica(test.size);
    test.matica[0][1] = 1;
    test.matica[1][2] = 1;
    test.matica[0][2] = 1;

    char result = nurelm_test_transitivity(&test);
    printf("relacia matici\n");
    for(int i = 0; i < test.size; i++)
    {
        for(int j = 0; j <test.size; j++)
        {
            printf("%d ", test.matica[i][j]);
        }
        printf("\n");
    }
    printf("matica je antisymetricka? %s\n", result ? "TRUE" : "FALSE");
    free_matica(test.matica,test.size);
    
}