/*
    Depends of "geometry.h"

    Map elements type 0 are empty spaces and type 1 are barrier spaces by default. You 
    can change which numbers means barrier spaces, but 0 must always mean empty.
    Map measurements must be defined in macros LENGHT and HEIGHT

    Item elements type and description are optional.

    Mobs walk based on their direction. 0 = up, 1 = right, 2 = down, 3 = left. 
    Mode element is optional.

    You can customize the structs as your game demand, but keep non-optional elements for
    correct functioning, unless you modify functions.
*/

#ifndef GAME_BASICS
#define GAME_BASICS
#include "geometry.h"

typedef struct{
    char c;
    int type;
    int color;
}Map;

typedef struct{
    char *name;
    char *description;
    int type;
    int amount;
    int max_amount;
}Item;

typedef struct{
    Item *inventory;
    coordinate p;
    char c;
    int d; //direction

    int color;
    int health;
    int mode;
    int walk_cd;
}Point_Mob;

typedef struct{
    Item *inventory;
    rectangle r;
    char **matrix;
    int d;

    int color;
    int health;
    int mode;    
    int walk_cd;
}Large_Mob;

#define LENGHT 0
#define HEIGHT 0

#define BARRIERS {1}
#define MAX_ITEMS 0

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

/*
    map functions
*/

int is_inside_map(coordinate p);

//returns 1 if m[y][x] is a barrier
int is_barrier(coordinate p, Map m[][LENGHT]);

int count_map_type(Map m[][LENGHT], int type);

void set_cell(Map m[][LENGHT], coordinate p, char c, int type, int color);

void define_char_type(Map m[][LENGHT], char c, int type);

/*
    mob functions
*/

/*
    0 -> 2
    1 -> 3
    2 -> 0
    3 -> 1
*/ 
int invert(int direction);

//returns 1 if cooldown is 0
int cooldown_time(int *cd);

void take_damage(int *health, int damage);

void heal(int *health, int max_hp, int amount);

int is_alive(int health);

//add mob to mob vector
#define append_mob(mob, new_mob, max) \
    _Generic((mob), \
        Point_Mob **: append_mob_point, \
        Large_Mob **: append_mob_large \
    )(mob, new_mob, max)

//removes mob x from vector
#define pop_mob(mob, x, max) \
    _Generic((mob), \
        Point_Mob *: pop_mob_point, \
        Large_Mob *: pop_mob_large \
    )(mob, x, max)

//realloc mob vector size
#define allocate_mob(vector, size) \
    _Generic((vector), \
        Point_Mob **: allocate_mob_point, \
        Large_Mob **: allocate_mob_large \
    )(vector, size)

//verifies if mob new position is inside map and is not barrier
#define can_move(p, m) \
    _Generic((p), \
        coordinate: can_move_point, \
        rectangle: can_move_large \
    )(p, m)

//changes mob position if it is possible
#define try_move(mob, m) \
    _Generic((mob), \
        Point_Mob *: try_move_point, \
        Large_Mob *: try_move_large \
    )(mob, m)

//decrements cooldown walk and change mob's position if cd is 0
#define move_action(mob, m) \
    _Generic((mob), \
        Point_Mob *: move_action_point, \
        Large_Mob *: move_action_large \
    )(mob, m)

int can_move_point(coordinate p, Map m[][LENGHT]);
int can_move_large(rectangle r, Map m[][LENGHT]);
int try_move_point(Point_Mob *mob, Map m[][LENGHT]);
int try_move_large(Large_Mob *mob, Map m[][LENGHT]);
int move_action_point(Point_Mob *mob, Map m[][LENGHT]);
int move_action_large(Large_Mob *mob, Map m[][LENGHT]);
int allocate_mob_point(Point_Mob **vector, int size);
int allocate_mob_large(Large_Mob **vector, int size);
void pop_mob_point(Point_Mob *mob, int x, int *max);
void pop_mob_large(Large_Mob *mob, int x, int *max);
int append_mob_point(Point_Mob **mob, Point_Mob new_mob, int *max);
int append_mob_large(Large_Mob **mob, Large_Mob new_mob, int *max);

/*
    Item
*/

void pop_item(Item *item, int x, int *max);
int allocate_inventory(Item **inventory, int size);
int append_item(Item **inventory, Item new_item, int *max);
Item *sorted_inventory(Item *inventory, const char key[], int max);

#endif