/*
    coordinate is a single 2D dot (y, x)
    rectangle is a coordinate with lenght and height
    matrix is a rectangle with "a drawing"
*/

#include "geometry.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

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
            return (coordinate){0, r.len-1};
        case 2://lower left
            return (coordinate){r.hgt-1, 0};
        case 3://lower right
            return (coordinate){r.hgt-1, r.len-1};
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
    return (
        a.p.x < b.p.x + b.len &&
        a.p.x + a.len > b.p.x &&
        a.p.y < b.p.y + b.hgt &&
        a.p.y + a.hgt > b.p.y
    );
}

int rectangle_area(rectangle r){
    return r.hgt*r.len;
}

int rectangle_contains(rectangle a, rectangle b){
    if(
        b.p.x >= a.p.x &&
        b.p.y >= a.p.y &&
        b.p.x + b.len <= a.p.x + a.len &&
        b.p.y + b.hgt <= a.p.y + a.hgt
    )
    return 1;

    return 0;
}

//matrix

int point_in_matrix_int(coordinate p, rectangle m, int matrix[][m.len]){
    for(int i=0; i<m.hgt; i++)
        for(int j=0; j<m.len; j++)
            if(
                equal(p, (coordinate){m.p.y + i, m.p.x + j}) && matrix[i][j] != 0
            )
                return 1;
    return 0;
}

int point_in_matrix_char(coordinate p, rectangle m, char matrix[][m.len]){
    for(int i=0; i<m.hgt; i++)
        for(int j=0; j<m.len; j++)
            if(
                equal(p, (coordinate){m.p.y + i, m.p.x + j}) && matrix[i][j] != ' '
            )
                return 1;
    return 0;
}

int matrix_collision_int(rectangle a, rectangle b, int mA[][a.len], int mB[][b.len]){
    for(int i=0; i<a.hgt; i++)
        for(int j=0; j<a.len; j++)
            for(int k=0; k<b.hgt; k++)
                for(int l=0; l<b.len; l++)
                    if(
                        equal(
                            abs_coordinate(a, (coordinate){i, j}), 
                            abs_coordinate(b, (coordinate){k, l})
                        )
                        && mA[i][j] != 0 && mB[k][l] != 0
                    )
                    return 1;

    return 0;
}

int matrix_collision_char(rectangle a, rectangle b, char mA[][a.len], char mB[][b.len]){
    for(int i=0; i<a.hgt; i++)
        for(int j=0; j<a.len; j++)
            for(int k=0; k<b.hgt; k++)
                for(int l=0; l<b.len; l++)
                    if(
                        equal(
                            abs_coordinate(a, (coordinate){i, j}), 
                            abs_coordinate(b, (coordinate){k, l})
                        )
                        && mA[i][j] != ' ' && mB[k][l] != ' '
                    )
                    return 1;

    return 0;
}

void rotate_matrix_int(int hgt, int len, int m[][len]){ 
    int new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[hgt-1-x][y];

    memcpy(m, new_matrix, sizeof(new_matrix));
}

void rotate_matrix_char(int hgt, int len, char m[][len]){ 
    char new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[hgt-1-x][y];

    memcpy(m, new_matrix, sizeof(new_matrix));
}

void flip_matrix_int(int hgt, int len, int m[][len]){
    int new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[y][len-1-x];

    memcpy(m, new_matrix, sizeof(new_matrix));
}

void flip_matrix_char(int hgt, int len, char m[][len]){
    char new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[y][len-1-x];

    memcpy(m, new_matrix, sizeof(new_matrix));
}

void upside_down_matrix_int(int hgt, int len, int m[][len]){
    int new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[hgt-1-y][x];

    memcpy(m, new_matrix, sizeof(new_matrix));
}

void upside_down_matrix_char(int hgt, int len, char m[][len]){
    char new_matrix[hgt][len];

    for(int y=0; y<hgt; y++)
        for(int x=0; x<len; x++)
            new_matrix[y][x] = m[hgt-1-y][x];

    memcpy(m, new_matrix, sizeof(new_matrix));
}