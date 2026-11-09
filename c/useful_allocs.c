#include <stdlib.h>
#include <string.h>
#include "useful_allocs.h"

char **alloc_matrix_char(int hgt, int len){
    char **new = malloc(sizeof(char*)*hgt);
    if(new == NULL) return NULL;

    for(int i=0; i<hgt; i++){
        new[i] = malloc(sizeof(char)*len);
        if(new[i] == NULL){
            for(int j=0; j<i; j++) free(new[j]);
            free(new);
            return NULL;
        }
    }
    return new;
}

int **alloc_matrix_int(int hgt, int len){
    char **new = malloc(sizeof(int*)*hgt);
    if(new == NULL) return NULL;

    for(int i=0; i<hgt; i++){
        new[i] = malloc(sizeof(char)*len);
        if(new[i] == NULL){
            for(int j=0; j<i; j++) free(new[j]);
            free(new);
            return NULL;
        }
    }
    return new;
}

void free_matrix(int hgt, char **m){
    for(int i=0; i<hgt; i++)
        free(m[i]);
    free(m);
}

void copy_matrix_char(int hgt, int len, char **dest, char origin[hgt][len]){
    for(int i=0; i<hgt; i++){
        memcpy(dest[i], origin[i], len);
    }
}

void copy_matrix_int(int hgt, int len, char **dest, int origin[hgt][len]){
    for(int i=0; i<hgt; i++){
        memcpy(dest[i], origin[i], len);
    }
}
