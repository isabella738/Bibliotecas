#ifndef USEFUL_ALLOCS
#define USEFUL_ALLOCS

char **alloc_matrix_char(int hgt, int len);
int **alloc_matrix_int(int hgt, int len);
void free_matrix(int hgt, char **m);
void copy_matrix_char(int hgt, int len, char **dest, char origin[hgt][len]);
void copy_matrix_int(int hgt, int len, char **dest, int origin[hgt][len]);

#define copy_matrix(hgt, len, dest, origin) \
    _Generic((dest), \
        int **: copy_matrix_int, \
        char **: copy_matrix_char, \
    )(hgt, len, dest, origin)

#endif