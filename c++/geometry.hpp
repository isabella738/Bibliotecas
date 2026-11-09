#include <math.h>
#include <vector>
#include <algorithm>

struct coordinate {
    int y, x;

    bool operator==(const coordinate& outro) const {
        return y == outro.y && x == outro.x;
    }
    coordinate operator+(const coordinate& outro) const {
        return {y + outro.y, x + outro.x};
    }
    coordinate operator-(const coordinate& outro) const {
        return {y - outro.y, x - outro.x};
    }
    coordinate operator*(int n) const {
        return {y * n, x * n};
    }
    coordinate midpoint(coordinate other){
        return {(y+other.y/2), (x+other.x)/2};
    }
    double distance_from(coordinate other){
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }
    int manhattan_distance_from(coordinate other){
        return abs(y - other.y) + abs(x - other.x);
    }
    int dot(coordinate other){
        return y*other.y + x*other.x;
    }
};

struct rectangle {
    coordinate p;
    int hgt, len;

    int area(){
        return hgt*len;
    }
    
    coordinate center(){
        return {hgt/2, len/2};
    }
    
    coordinate abs_coordinate(coordinate other){
        return p+other;
    }
    
    coordinate vertex(int v){
        switch(v){
            case 0://upper left
                return {0, 0};
            case 1://upper right
                return {0, len};
            case 2://lower left
                return {hgt, 0};
            case 3://lower right
                return {hgt, len};
        }
        return {-1, -1};
    }
    
    bool contains_point(coordinate q){
        return (
            q.x >= p.x && q.x < p.x + len &&
            q.y >= p.y && q.y < p.y + hgt
        );
    }
    
    bool collide_with(rectangle other){
        return (!(
            p.x >= other.p.x + other.len ||
            p.x + len <= other.p.x ||
            p.y >= other.p.y + other.hgt ||
            p.y + hgt <= other.p.y
        ));
    }
    
    bool contains_rectangle(rectangle b){
        return (!(
            b.p.x < p.x ||
            b.p.y < p.y ||
            b.p.x + b.len > p.x + len ||
            b.p.y + b.hgt > p.y + hgt
        ));
    }
    
    bool interssects_with(rectangle other, coordinate *p1, coordinate *p2){

        if(!this->collide_with(other)) return 0;
        coordinate minA = p, maxA = this->abs_coordinate(this->vertex(3));
        coordinate minB = other.p, maxB = other.abs_coordinate(other.vertex(3));

        (*p1).x = std::max(minA.x, minB.x);
        (*p1).y = std::max(minA.y, minB.y);
        (*p2).x = std::min(maxA.x, maxB.x);
        (*p2).y = std::min(maxA.y, maxB.y);
        return 1;
    }
    
    bool overlaps_with(rectangle other, rectangle *_new){

        coordinate p1, p2;
        if(this->interssects_with(other, &p1, &p2)){
            int hgt = abs(p2.y - p1.y);
            int len = abs(p2.x - p1.x);
            coordinate p;
            p.y = p1.y;
            p.x = std::min(p1.x, p2.x);
            (*_new) = {p, hgt, len};
            return 1;
        }
        return 0;
    }
};

template <typename T>
using matrix = std::vector<T>;

template <typename T>
struct Matrix {
    matrix<T> m;
    rectangle r;

    void rotate(){
        int aux = r.hgt;
        r.hgt = r.len;
        r.len = aux;
        matrix<T> _new(r.hgt);
        for(int y=0; y<r.hgt; y++){
            for(int x=0; x<r.len; x++)
                _new[y] += m[r.len-x-1][y];
        }
        m = (_new);
    }

    void flip(){
        matrix<T> _new(r.hgt);

        for(int y=0; y<r.hgt; y++){
            for(int x=0; x<r.len; x++)
                _new[y] += m[y][r.len-1-x];
        }
        m = (_new);
    }

    void upside_down(){
        matrix<T> _new(r.hgt);

        for(int y=0; y<r.hgt; y++){
            for(int x=0; x<r.len; x++)
                _new[y] += m[r.hgt-1-y][x];
        }
        m = (_new);
    }
    
    bool contains_point(coordinate p){
        coordinate local = p - r.p;

        if(local.y < 0 || local.y >= r.hgt ||
        local.x < 0 || local.x >= r.len)
            return 0;

        return this->m[local.y][local.x] != ' ';
    }
    
    bool collide_with(Matrix<T> other){
        rectangle rec;
        if(this->r.overlaps_with(other.r, &rec)){
            for(int i=0; i<rec.hgt; i++){
                for(int j=0; j<rec.len; j++){
                    coordinate p = rec.p + (coordinate){i, j};
                    coordinate ap, bp;//relatives coordinates of p for a and b rectangles

                    ap = p - this->r.p;
                    bp = p - other.r.p;

                    if(this->m[ap.y][ap.x] != ' ' && other.m[bp.y][bp.x] != ' ') return 1;
                }
            }
        }

        return 0;
    }
};