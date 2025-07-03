#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

#define BIND(r, i, j) ((r) -> matrix[(i)*(r) -> n + (j)])

#define state_yes 0x01
#define state_no 0x02
#define mask_trans 0xC0
#define set_trans(p) ((p & mask_trans) >> 6)
#define put_trans(p, s) ((p & ~mask_trans) | ((s & 0x03) << 6)) 

typedef struct {
    unsigned int m;
    unsigned int n;
    char properties;
    char *matrix;
}NURELM;

void nurelm_destroy(NURELM *rel){
    if(!rel) return;
    free(rel -> matrix);
    free(rel);
}

char nurelm_save(NURELM *rel, char *filename){
    FILE *file = fopen(filename, "w");
    if(!file)
    return 0;

    if(fprintf(file, "NR1\n") < 0){
        fclose(file);
        return 0;
    }
    
    if(fprintf(file, "%u %u\n", rel -> m, rel -> n) <0){
        fclose(file);
        return 0;
    }

    for(unsigned int i = 0; i < rel -> m; i++){
        for(unsigned int j = 0; j < rel -> n; j++){
            char c = BIND(rel, i, j) ? '1' : '0';
            if(fputc(c, file) == EOF){
                fclose(file);
                return 0;
            }
        }
        if(fputc('\n', file) == EOF){
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

NURELM *nurelm_create_Znot_to_Znot(void){
    NURELM *rel = malloc(sizeof(NURELM));
    if(!rel)
    return NULL;

    rel -> m = 0;
    rel -> n = 0;
    rel -> properties = 0;
    rel -> matrix = NULL;

    return rel;
}

void nurelm_print(NURELM *rel){
    for(unsigned int i = 0; i < rel -> m; i++){
        for(unsigned int j = 0; j < rel -> n; j++){
            printf("%c ",BIND(rel,i ,j) ? '1' : '0');
        }
        printf("\n");
    }
}
char nurelm_test_transitivity(NURELM *rel){
    for(unsigned int i = 0; i < rel -> m; i++){
        for(unsigned int j = 0; j < rel -> n; j++){
            if(BIND(rel, i, j)){
                for(unsigned int k = 0; k < rel -> n; k++){
                    if(BIND(rel, j, k) && !BIND(rel, i, k)){
                        rel -> properties = put_trans(rel -> properties, state_no);
                        return FALSE;
                    }
                }
            }
        }
    }
    rel -> properties = put_trans(rel -> properties, state_yes);
    return TRUE;
}

NURELM *nurelm_create_manual(unsigned int m, unsigned int n, const char *data){

    NURELM *rel = malloc(sizeof(NURELM));
    if(!rel) return NULL;

    rel -> m = m;
    rel -> n = n;
    rel -> properties = 0;
    rel -> matrix = malloc(m * n);

    if(!rel -> matrix) {
        free(rel);
        return NULL;
    }
    for(unsigned int i = 0; i < m * n;  i++)
        rel -> matrix[i] = data[i];
    
    return rel;
}

NURELM *nurelm_create_by_file(char *filename){
    FILE *file = fopen(filename, "r");
    if(!file){
        printf("2");
        return NULL;
        
    }
    char line[1024];
    if(!fgets(line, sizeof(line), file)){
        fclose(file);
        printf("3");
        return NULL;
    }

    if(line[0] != 'N' || line[1] != 'R' || line[2] != '1'){
        fclose(file);
        printf("4");
        return NULL;
    }

    /*
    for(int i=0; i<1024; i++)
        printf("%c",line[i]);
    */
    
    unsigned int m = 0, n = 0;
    if(fscanf(file, "%u %u", &m, &n) != 2 || m == 0 || n == 0){
        fclose(file);
        printf("5 %u %u", m, n);
        return NULL;
    }

    printf("DIM: %u %u\n", m, n);

    NURELM *rel = malloc(sizeof(NURELM));
    if(!rel){
        fclose(file);
        printf("6");
        return NULL;
        
    }

    rel -> m = m;
    rel -> n = n;
    rel -> properties = 0;
    rel -> matrix = malloc(m * n);
    if(!rel -> matrix){
        free(rel);
        fclose(file);
        printf("7");
        return NULL;
    }

    for (unsigned int row = 0; row < m; row++) {
        if (!fscanf(file, "%u",&line[row])) {
            free(rel->matrix);
            free(rel);
            fclose(file);
            printf("8");
            return NULL;
        }
    
        for (unsigned int col = 0; col < n; col++){
            if (line[col] != '0' && line[col] != '1') {
                free(rel->matrix);
                free(rel);
                fclose(file);
                printf("9");
                return NULL;
            }
            BIND(rel, row, col) = line[col] -'0';
        }    
    }
    fclose(file);
    return rel;
}

int main(){
    char matrix_data[] = {
        1, 1, 1,
        0, 1, 1,
        0, 0, 1
    };
    NURELM *rel = nurelm_create_manual(3, 3, matrix_data);
    NURELM *newrel;

    if(!rel){
        printf("chyba vytvorenia relacie.\n");
        return 1;
    }

    printf("matica relacie:\n");
    nurelm_print(rel);

    if(nurelm_test_transitivity(rel)){
        printf("relacia je trasitivna\n");
    }

    else{
        printf("relacia nie je transitivna\n");
    }

    nurelm_save(rel, "test.rel");
    newrel = nurelm_create_by_file("test.rel");

    printf("%p", newrel);
//    nurelm_print(newrel);

    nurelm_destroy(rel);
//    getchar();
}
