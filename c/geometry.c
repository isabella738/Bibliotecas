/*
    coordinate is a single 2D dot (y, x)
    rectangle is a coordinate with lenght and height
    matrix is a rectangle with "a drawing"
    all matrix must be pointers to pointers.

    depends of "alloc.h"
*/

#include "geometry.h"
#include "useful_allocs.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

coordinate direction[] = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
};

//mathematics

int average(int a, int b){
    return (a+b)/2;
}

//coordinates

coordinate sum(coordinate a, coordinate b){
    return (coordinate){a.y + b.y, a.x + b.x};
}

coordinate sub(coordinate a, coordinate b){
    return (coordinate){a.y - b.y, a.x - b.x};
}

coordinate mltp(int n, coordinate a){
    return (coordinate){n*a.y, n*a.x};
}

coordinate negate(coordinate p){
    return (coordinate){-p.y, -p.x};
}

coordinate midpoint(coordinate a, coordinate b){
    return ((coordinate){average(a.y, b.y), average(a.x, b.x)});
}

int dot(coordinate a, coordinate b){ 
    return a.y*b.y + a.x*b.x;
}

int manhattan_distance(coordinate a, coordinate b){
    return abs(a.y - b.y) + abs(a.x - b.x);
}

double distance(coordinate a, coordinate b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

int equal(coordinate a, coordinate b){
    if(a.y == b.y && a.x == b.x) return 1;
    return 0;
}

//rectangles

coordinate abs_coordinate(rectangle r, coordinate p){
    return sum(p, r.p);
}

coordinate rectangle_center(rectangle r){
    return (coordinate){r.hgt/2, r.len/2};
}

coordinate vertex(rectangle r, int vertex){
    switch(vertex){
        case 0://upper left
            return (coordinate){0, 0};
        case 1://upper right
            return (coordinate){0, r.len};
        case 2://lower left
            return (coordinate){r.hgt, 0};
        case 3://lower right
            return (coordinate){r.hgt, r.len};
    }
    return (coordinate){-1, -1};
}

int point_in_rectangle(coordinate p, rectangle r){
    return (
        p.x >= r.p.x && p.x < r.p.x + r.len &&
        p.y >= r.p.y && p.y < r.p.y + r.hgt
    );
}

int rectangle_collision(rectangle a, rectangle b){
    return (!(
        a.p.x >= b.p.x + b.len ||
        a.p.x + a.len <= b.p.x ||
        a.p.y >= b.p.y + b.hgt ||
        a.p.y + a.hgt <= b.p.y
    ));
}

int rectangle_area(rectangle r){
    return r.hgt*r.len;
}

int rectangle_contains(rectangle a, rectangle b){
    return (!(
        b.p.x < a.p.x ||
        b.p.y < a.p.y ||
        b.p.x + b.len > a.p.x + a.len ||
        b.p.y + b.hgt > a.p.y + a.hgt
    ));
}

int interssection(rectangle a, rectangle b, coordinate *p1, coordinate *p2){
    if(!rectangle_collision(a, b)) return 0;
    coordinate minA = a.p, maxA = sum(a.p, vertex(a, 3));
    coordinate minB = b.p, maxB = sum(b.p, vertex(b, 3));

    (*p1).x = max(minA.x, minB.x);
    (*p1).y = max(minA.y, minB.y);
    (*p2).x = min(maxA.x, maxB.x);
    (*p2).y = min(maxA.y, maxB.y);
    return 1;
}

int sobreposition(rectangle a, rectangle b, rectangle *new){

    coordinate p1, p2;
    if(interssection(a, b, &p1, &p2)){
        int hgt = abs(p2.y - p1.y);
        int len = abs(p2.x - p1.x);
        coordinate p;
        p.y = p1.y;
        p.x = min(p1.x, p2.x);
        (*new) = (rectangle){p, hgt, len};
        return 1;
    }

    return 0;
}

//matrix

int point_in_matrix_int(coordinate p, rectangle m, int **matrix){
    coordinate local = sub(p, m.p);

    if(local.y < 0 || local.y >= m.hgt ||
       local.x < 0 || local.x >= m.len)
        return 0;

    return matrix[local.y][local.x] != 0;
}

int point_in_matrix_char(coordinate p, rectangle m, char **matrix){
    coordinate local = sub(p, m.p);

    if(local.y < 0 || local.y >= m.hgt ||
       local.x < 0 || local.x >= m.len)
        return 0;

    return matrix[local.y][local.x] != ' ';
}

int matrix_collision_int(rectangle a, rectangle b, int **mA, int **mB){
    rectangle sp;
    if(sobreposition(a, b, &sp)){
        for(int i=0; i<sp.hgt; i++){
            for(int j=0; j<sp.len; j++){
                coordinate p = sum(sp.p,(coordinate){i, j});
                coordinate ap, bp;//relatives coordinates of p for a and b rectangles

                ap = sub(p, a.p);
                bp = sub(p, b.p);

                if(mA[ap.y][ap.x] != 0 && mB[bp.y][bp.x] != 0) return 1;
            }
        }
    }
    return 0;
}

int matrix_collision_char(rectangle a, rectangle b, char **mA, char **mB){
    rectangle sp;
    if(sobreposition(a, b, &sp)){
        for(int i=0; i<sp.hgt; i++){
            for(int j=0; j<sp.len; j++){
                coordinate p = sum(sp.p,(coordinate){i, j});
                coordinate ap, bp;//relatives coordinates of p for a and b rectangles

                ap = sub(p, a.p);
                bp = sub(p, b.p);

                if(mA[ap.y][ap.x] != ' ' && mB[bp.y][bp.x] != ' ') return 1;
            }
        }
    }
    return 0;
}

int **rotate_matrix_int(int *hgt, int *len, int **m){ 
    int **new_matrix = alloc_matrix(*len, *hgt);
    if(new_matrix == NULL) return NULL;

    int ax = (*hgt);
    (*hgt) = (*len);
    (*len) = ax;

    for(int y=0; y<(*hgt); y++){
        for(int x=0; x<(*len); x++)
            new_matrix[y][x] = m[x][(*hgt)-y-1];
        new_matrix[y][(*len)] = '\0';
    }

    return new_matrix;
}

char **rotate_matrix_char(int *hgt, int *len, char **m){ 
    char **new_matrix = alloc_matrix(*len, *hgt);
    if(new_matrix == NULL) return NULL;

    int ax = (*hgt);
    (*hgt) = (*len);
    (*len) = ax;

    for(int y=0; y<(*hgt); y++){
        for(int x=0; x<(*len); x++)
            new_matrix[y][x] = m[x][(*hgt)-y-1];
        new_matrix[y][(*len)] = '\0';
    }

    return new_matrix; 
}

void flip_matrix_int(int hgt, int len, int **m){
    int new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[y][len-1-x];

    memcpy(m, new_matrix, sizeof(new_matrix));
}

void flip_matrix_char(int hgt, int len, char **m){
    char new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[y][len-1-x];

    memcpy(m, new_matrix, sizeof(new_matrix));
}

void upside_down_matrix_int(int hgt, int len, int **m){
    int new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[hgt-1-y][x];

    copy_matrix_int(hgt, len, m, new_matrix);
}

void upside_down_matrix_char(int hgt, int len, char **m){
    char new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[hgt-1-y][x];

    copy_matrix_char(hgt, len, m, new_matrix);
}

