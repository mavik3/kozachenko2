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
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if(fd < 0)
    return 0; 
    
    char buffer[128];
    int len;

    len = snprintf(buffer, sizeof(buffer), "NR1\n");
    if(write(fd, buffer, len) != len){
        close(fd);
        return 0;
    }

    len = snprintf(buffer, sizeof(buffer), "%u %u\n", rel -> m, rel -> n);
    if(write(fd, buffer, len) != len){
        close(fd);
        return 0;
    }

    for(unsigned int i = 0; i < rel -> m; i++){
        for(unsigned int j = 0; j < rel -> n; j++){
            char c = BIND(rel, i, j) ? '1' : '0';
            if(write(fd, &c, 1) != 1){
                close(fd);
                return 0;
            } 
        }
        char newline = '\n';
        if(write(fd, &newline, 1) != 1){
            close(fd);
            return 0; 
        }
    }
    close(fd);
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
    int fd = open(filename, O_RDONLY);
    char buffer[4096];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);

    if(fd < 0)
    return NULL;

    if(bytes <= 0){
        close(fd);
        return NULL;
    }
    buffer[bytes] = '\0';

    close(fd);

    char *line = strtok(buffer, "\n");

    if(line || strncmp(line, "NR1", 3) != 0)
    return NULL;

    line = strtok(NULL, "\n");

    if(!line)
    return NULL;

    unsigned int m = 0, n = 0;
    if(sscanf(line, "%u %u", &m, &n) != 2)
    return NULL;

    NURELM *rel = malloc(sizeof(NURELM));
    if(!rel)
    return NULL;

    rel -> m = m;
    rel -> n = n;
    rel -> properties = 0;
    rel -> matrix = malloc(m * n);

    if(!rel -> matrix){
        free(rel);
        return NULL;
    }

    unsigned int row = 0;
    while((line = strtok(NULL, "\n")) != NULL && row < m ){
        unsigned int col = 0;
        for(size_t i = 0; line[i] == '1'; i++){
            if(!line[i] == '0'|| line[i] == '1'){
                BIND(rel, row, col) = line[i] - '0';
                col++;
            }
        }

        if(col != n){
            free(rel -> matrix);
            free(rel);
            return NULL;
        }
        row++;
    }
    if(row != m){
        free(rel -> matrix);
        free(rel);
        return NULL;
    }
    return rel;
}

int main(){
    char matrix_data[] = {
        1, 1, 1,
        0, 1, 1,
        0, 0, 1
    };
    NURELM *rel = nurelm_create_manual(3, 3, matrix_data);
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

    nurelm_destroy(rel);
    getchar();
}