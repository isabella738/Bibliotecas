#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct{
    int y, x;
}coordinate;

typedef struct{
    coordinate p;
    int hgt, len;
}rectangle;

coordinate direction[] = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
};

// mathematics

int average(int a, int b);

// coordinates

//sum each coordinate element
coordinate sum(coordinate a, coordinate b);

//subtracts each coordinate element
coordinate sub(coordinate a, coordinate b);

//multiply a coordinate with an integer
coordinate mltp(int n, coordinate a);

//invert signals of coordinate elements
coordinate negate(coordinate p);

//midpoint between a and b dots
coordinate midpoint(coordinate a, coordinate b);

//dot product between a and b dots
int dot(coordinate a, coordinate b);

//returns 1 if a equals to b
int equal(coordinate a, coordinate b);

int manhattan_distance(coordinate a, coordinate b);
double distance(coordinate a, coordinate b);

//rectangles

//returns absolute coordinate of p regarding the rectangle position
coordinate abs_coordinate(rectangle r, coordinate p);

//returns the upper coordinate if one of rectangle measures is even
coordinate rectangle_center(rectangle r);

/*
    returns coordinate of:
    vertex=0: upper left
    vertex=1: upper right
    vertex=2: lower left
    vertex=3: lower right
*/ coordinate vertex(rectangle r, int vertex);

//returns 1 if r contains p
int point_in_rectangle(coordinate p, rectangle r);

//returns 1 if there is intersection between a and b
int rectangle_collision(rectangle a, rectangle b);

//area of r
int rectangle_area(rectangle r);

//returns 1 if a contains b
int rectangle_contains(rectangle a, rectangle b);

// matrix

int point_in_matrix_int(coordinate p, rectangle m, int matrix[][m.len]);
int point_in_matrix_char(coordinate p, rectangle m, char matrix[][m.len]);
int matrix_collision_int(rectangle a, rectangle b, int mA[][a.len], int mB[][b.len]);
int matrix_collision_char(rectangle a, rectangle b, char mA[][a.len], char mB[][b.len]);
void rotate_matrix_int(int hgt, int len, int m[][len]);
void rotate_matrix_char(int hgt, int len, char m[][len]);
void flip_matrix_int(int hgt, int len, int m[][len]);
void flip_matrix_char(int hgt, int len, char m[][len]);
void upside_down_matrix_int(int hgt, int len, int m[][len]);
void upside_down_matrix_char(int hgt, int len, char m[][len]);

//returns 1 if p is a significant matrix element
#define point_in_matrix(p, m, matrix) \
    _Generic((matrix), \
        int (*)[(m).len]: point_in_matrix_int, \
        char (*)[(m).len]: point_in_matrix_char \
    )(p, m, matrix)

//returns 1 if any significant element of mA overlap any significant element of mB
#define matrix_collision(a, b, mA, mB) \
    _Generic((mA), \
        int (*)[(a).len]: matrix_collision_int, \
        char (*)[(a).len]: matrix_collision_char \
    )(a, b, mA, mB)

//rotates matrix 90 degrees clockwise
#define rotate_matrix(hgt, len, m) \
    _Generic((m), \
        int (*)[len]: rotate_matrix_int, \
        char (*)[len]: rotate_matrix_char \
    )(hgt, len, m)

//flips m horizontally
#define flip_matrix(hgt, len, m) \
    _Generic((m), \
        int (*)[len]: flip_matrix_int, \
        char (*)[len]: flip_matrix_char \
    )(hgt, len, m)

//flips m vertically
#define upside_down_matrix(hgt, len, m) \
    _Generic((m), \
        int (*)[len]: upside_down_matrix_int, \
        char (*)[len]: upside_down_matrix_char \
    )(hgt, len, m)

#endif