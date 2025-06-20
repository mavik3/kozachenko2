#include<stdio.h>
#include<stdlib.h>
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

NURELM *nurelm_create_Znot_to_Znot(void) {
    NURELM *rel = malloc(sizeof(NURELM));
    if(!rel) return NULL;
    rel -> m = 0;
    rel -> n = 0;
    rel -> properties = 0;
    rel -> matrix = NULL; 
    return rel;
}