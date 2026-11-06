#include "game_basics.h"
#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//others

void quick_sort(Inventory *vector, const char key[], int beggining, int end){
    if(beggining >= end) return;

    int p = beggining, i = end;
    int cmp=0;
    int way = -1;
    
    do{
        if(strcmp(key, "amount") == 0) cmp = vector[p].amount - vector[i].amount;
        else if(strcmp(key, "type") == 0) cmp = vector[p].type - vector[i].type;
        else cmp = strcmp(vector[p].name, vector[i].name); //by default, vector is sorted by name

        if(way == -1){
            if(cmp > 0){
                Inventory temp = vector[p];
                vector[p] = vector[i];
                vector[i] = temp;

                int aux = p;
                p = i;
                i = aux;

                way = 1;
            }
        }
        else{
            if(cmp < 0){
                Inventory temp = vector[p];
                vector[p] = vector[i];
                vector[i] = temp;

                int aux = p;
                p = i;
                i = aux;

                way = -1;
            }
        }
        i += way;
    }while(i != p);

    quick_sort(vector, key, beggining, p-1);
    quick_sort(vector, key, p+1, end);
}

// map functions

int is_inside_map(coordinate p){
    if(
        p.y >= 0 && p.y < HEIGHT &&
        p.x >= 0 && p.x < LENGHT
    ) return 1;
    return 0;
}

int is_barrier(coordinate p, Map m[][LENGHT]){
    if(!is_inside_map(p)) return 1;

    int v[] = BARRIERS;
    for(int i=0; i < sizeof(v)/sizeof(v[0]); i++){
        if(m[p.y][p.x].type == v[i])
            return 1;
    }
    return 0;
}

int count_map_type(Map m[][LENGHT], int type){
    int contador=0;
    for(int i=0; i<HEIGHT; i++)
        for(int j=0; j<LENGHT; j++)
            if(m[i][j].type == type) 
                contador++;
    return contador;
}

void set_cell(Map m[][LENGHT], coordinate p, const char c, int type, int color){
    if(!is_inside_map(p))
        return;

    m[p.y][p.x] = (Map){c, type, color};
}

void define_char_type(Map m[][LENGHT], const char c, int type){
    for(int i=0; i<HEIGHT; i++)
        for(int j=0; j<LENGHT; j++)
            if (m[i][j].c == c)
                m[i][j].type = type;
}

// mob functions

int invert(int direction){
    if(direction < 2) return direction += 2;
    return direction -= 2;
}

int cooldown_time(int *cd){
    return (*cd)-- <= 0;
}

void take_damage(int *health, int damage){
    if(damage < 0) return;

    if(damage > (*health)) (*health) = 0;
    else (*health) -= damage;
}

void heal(int *health, int max_hp, int amount){
    if(amount < 0) return;

    (*health) += amount;

    if((*health) > max_hp)
        (*health) = max_hp;
}

int is_alive(int health){
    return health > 0;
}

void add_score(int *score, int points){
    (*score) += points;
}

int can_move_point(coordinate p, Map m[][LENGHT]){
    return (is_inside_map(p) && !is_barrier(p, m));
}

int can_move_large(rectangle r, Map m[][LENGHT]){
    for(int i=0; i<r.hgt; i++){
        for(int j=0; j<r.len; j++){
            coordinate abs = sum(r.p, (coordinate){i, j});
            if(
                !is_inside_map(abs) || is_barrier(abs, m)
            )
            return 0;
        }
    }
    return 1;
}

int try_move_point(Point_Mob *mob, Map m[][LENGHT]){
    coordinate p = sum(mob->p, direction[mob->d]);
    if(can_move(p, m)){
        mob->p = p;
        return 1;
    }
    return 0;
}

int try_move_large(Large_Mob *mob, Map m[][LENGHT]){
    rectangle r = (rectangle){sum(mob->r.p, direction[mob->d]), mob->r.hgt, mob->r.len};
    if(can_move(r, m)){
        mob->r = r;
        return 1;
    }
    return 0;
}

int move_action_point(Point_Mob *mob, Map m[][LENGHT]){
    if(!cooldown_time(&mob->walk_cd)){
        return try_move_point(mob, m);
    }
    return 0;
}

int move_action_large(Large_Mob *mob, Map m[][LENGHT]){
    if(!cooldown_time(&mob->walk_cd)){
        return try_move_large(mob, m);
    }
    return 0;
}

int allocate_mob_point(Point_Mob **vector, int size){
    Point_Mob *new = realloc(*vector, sizeof(Point_Mob)*size);
    if(new != NULL){
        *vector = new;
        return 1;
    }
    return 0;
}

int allocate_mob_large(Large_Mob **vector, int size){
    Large_Mob *new = realloc(*vector, sizeof(Large_Mob)*size);
    if(new != NULL){
        *vector = new;
        return 1;
    }
    return 0;
}

void pop_mob_point(Point_Mob *mob, int x, int *max){
    if(x < 0 || x >= (*max)) return;
    for(int i=x; i<(*max); i++)
        mob[i] = mob[i+1];
    (*max)--;
}

void pop_mob_large(Large_Mob *mob, int x, int *max){
    if(x < 0 || x >= (*max)) return;
    for(int i=x; i<(*max); i++)
        mob[i] = mob[i+1];
    (*max)--;
}

int append_mob_point(Point_Mob **mob, Point_Mob new_mob, int *max){
    if(allocate_mob_point(mob, (*max)+1)){
        (*mob)[*max] = new_mob;
        (*max)++;
        return 1;
    }
    return 0;
}

int append_mob_large(Large_Mob **mob, Large_Mob new_mob, int *max){
    if(allocate_mob_large(mob, (*max)+1)){
        (*mob)[*max] = new_mob;
        (*max)++;
        return 1;
    }
    return 0;
}

//inventory

void pop_item(Inventory *item, int x, int *max){
    if(x < 0 || x >= (*max)) return;

    for(int i=x; i<(*max); i++)
        item[i] = item[i+1];
    (*max)--;
}

int allocate_inventory(Inventory **inventory, int size){
    Inventory *new_inventory = realloc(*inventory, sizeof(Inventory)*size);
    if(new_inventory != NULL){
        *inventory = new_inventory;
        return 1;
    }
    return 0;
}

int append_item(Inventory **inventory, Inventory new_item, int *max){
    for(int i=0; i<(*max); i++){
        if(strcmp(new_item.name, (*inventory)[i].name) == 0){
            (*inventory)[i].amount += new_item.amount;

            if((*inventory)[i].amount <= (*inventory)[i].max_amount) return 1;
            
            new_item.amount = (*inventory)[i].amount - (*inventory)[i].max_amount;
            (*inventory)[i].amount = (*inventory)[i].max_amount;
        }
    }

    int allocate=1;
    while((*max)+1 <= MAX_ITEMS && new_item.amount > 0 && allocate){
        allocate = allocate_inventory(inventory, (*max)+1);
        new_item.amount -= new_item.max_amount;
    }
    if(!allocate) return 0;

    (*max)++;
    return 1;
}

Inventory *sorted_inventory(Inventory *inventory, const char key[], int max){
    if(max <= 0) return NULL;

    Inventory *new_inventory = malloc(sizeof(Inventory)*max);

    if(new_inventory == NULL) return NULL;

    memcpy(new_inventory, inventory, sizeof(new_inventory));
    quick_sort(new_inventory, key, 0, max-1);
    return new_inventory;
}
