/*
    Depends of: geometry.hpp and text_edit.h

    Cell has information of a single cell of the map.

    Map is a matrix of cells and contains map functions. 
    Map must contain a vector characters type, which is a char vector and each index+1 means an 
    type. For example, for char '#' at index 0, all cells with this character will be type 1.
    Type 0 must be exclusive for empty spaces.
    In a similar way, map must contain a vector of type colors. For example, for int 3 at index 0,
    all cells with type 1 will be color 3.

    Item has information of a single item

    Slot has an item and its amount

    Inventory is an vector of slots and constains inventory functions

    Mob has an inventory and mob functions. It has an string vector (the mob "drawing") and a int
    vector, which designs the color for each drawing cell
*/

#include "geometry.hpp"
#include "text_edit.h"
#include <iostream>
#include <vector>
#include <cstring>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

struct Cell {
    char c;
    int type;
    int color;

    void define(char new_c, int new_type, int new_color){
        c = new_c;
        type = new_type;
        color = new_color;
    }

    void clear(){
        c = ' ';
        type = 0;
        color = 0;
    }

    void print(){
        std::cout << colors[color] << c << RESET;
    }

};

struct Map {
    std::vector<std::vector<Cell>> cell;
    rectangle r;

    //type of a character is vector index plus 1
    std::vector<char> characters_type;
    int else_type;

    //color of a type is vector index plus 1
    std::vector<int> type_colors;
    int else_color;
    
    //set which types are barriers/walls
    std::vector<int> barriers;

    void set_characters(Matrix<std::string> _new){
        r = _new.r;
        for(int i=0; i<r.hgt; i++){
            for(int j=0; j<r.len; j++){
                cell[i][j].c = _new.m[i][j];
            }
        }
    }

    //cells with same character will have same type
    void set_type(){
        for(int i=0; i<r.hgt; i++){
            for(int j=0; j<r.len; j++){

                if(cell[i][j].c == ' ') {
                    cell[i][j].type = 0;
                    continue;
                }

                int stop=0;
                for(int k=0; k<characters_type.size(); k++){
                    if(cell[i][j].c == characters_type[k]){
                        cell[i][j].type = k+1;
                        stop=1;
                        break;
                    }
                }

                if(!stop) cell[i][j].type = else_type;
            }
        }
    }

    //same type cells will have the same color
    void set_color(){
        for(int i=0; i<r.hgt; i++){
            for(int j=0; j<r.len; j++){

                int stop=0;
                for(int k=0; k<type_colors.size(); k++){
                    if(cell[i][j].type == k+1){
                        cell[i][j].color = type_colors[k];
                        stop=1;
                        break;
                    }
                }

                if(!stop) cell[i][j].type = else_color;
            }
        }
    }

    //set characters, type and color
    void define_all(Matrix<std::string> m){
        set_characters(m);
        set_type();
        set_color();
    }

    bool contains_point(coordinate p){
        if(p.y < 0 || p.y >= r.hgt || p.x < 0 || p.x >= r.len) return 0;
        return 1;
    }

    Cell return_cell(coordinate p){
        if(!contains_point(p)) return {-1, -1, -1};
        return cell[p.y][p.x];
    }

    int count_type(int type){
        int counter=0;
        for(int i=0; i<r.hgt; i++)
            for(int j=0; j<r.len; j++)
                if(cell[i][j].type == type) counter++;
        return counter;
    }

    bool is_barrier(coordinate p){
        for(int type : barriers){
            if(return_cell(p).type == type) return 1;
        }
        return 0;
    }

    void print(){
        for(int i=0; i<r.hgt; i++){
            for(int j=0; j<r.len; j++){
                cell[i][j].print();
            }
            std::cout << '\n';
        }
    }

};

struct Item {
    std::string name;
    std::string description;
    int max_amount;
    int type;
    /*
        other useful information
    */
};

struct Slot {
    Item item;
    int amount;
};

class Inventory {
    private:
        void quick_sort(Inventory *inventory, std::string key, int beggining, int end){
            if(beggining >= end) return;

            int p = beggining, i = end;
            int cmp=0;
            int way = -1;
            
            do{
                Slot *sp = &inventory->slot[p];
                Slot *si = &inventory->slot[i];

                if(key == "amount") cmp = sp->amount - si->amount;
                else if(key == "type") cmp = sp->item.type - si->item.type;
                else cmp = sp->item.name < si->item.name; //by default, vector is sorted by name

                if(way == -1){
                    if(cmp > 0){
                        Slot temp = *sp;
                        *sp = *si;
                        *si = temp;

                        int aux = p;
                        p = i;
                        i = aux;

                        way = 1;
                    }
                }
                else{
                    if(cmp < 0){
                        Slot temp = *sp;
                        *sp = *si;
                        *si = temp;

                        int aux = p;
                        p = i;
                        i = aux;

                        way = -1;
                    }
                }
                i += way;
            }while(i != p);

            quick_sort(inventory, key, beggining, p-1);
            quick_sort(inventory, key, p+1, end);
        }
    
    public:
        std::vector<Slot> slot;
        void append(Item new_item){
            
            for(Slot s : slot){
                if(s.item.name == new_item.name && s.amount < new_item.max_amount){
                    s.amount++;
                    return;
                }
            }
            Slot new_slot = {new_item, 1};
            slot.push_back(new_slot);
        }

        int total_slots(){
            return slot.size();
        }

        int is_valid(int i){
            if(i < 0 || i >= total_slots()) return 0;
            return 1;
        }

        void discount(int amount, int i){
            if(!is_valid(i)) return;
            slot[i].amount -= amount;
            if(slot[i].amount < 0) slot[i].amount = 0;
        }
        
        void clear_slot(int i){
            if(!is_valid(i)) return;
            slot[i].item = {0};
            slot[i].amount = 0;
        }

        void swap_slots(int a, int b){
            if(!is_valid(a) || !(is_valid(b))) return;
            Slot aux = slot[a];
            slot[a] = slot[b];
            slot[b] = aux;
        }

        Inventory sorted_inventory(std::string key){
            Inventory new_inventory = *this;
            quick_sort(&new_inventory, key, 0, total_slots()-1);
            return new_inventory;
        }
};

class Mob {
    private:
        Matrix<std::string> form;
        std::vector<std::vector<int>> color; //each number is a color for its coordinate
        int health;
        int max_health;
        int mode;
        int walk_cd;
        int walk_cd_original;
        int d;
        /*
            other cooldowns and other things
        */

        coordinate next_move(){
            switch(d){
                case UP: return form.r.p + (coordinate){-1,0}; 
                case RIGHT: return form.r.p + (coordinate){0,1}; 
                case DOWN: return form.r.p + (coordinate){1,0};
                case LEFT: return form.r.p + (coordinate){0,-1};
            }
        }

        int cooldown_walk(){
            return !(walk_cd-- > 0);
        }

    public:
        Inventory inventory;

        bool set_colors(std::vector<std::vector<int>> c){
            if(c.size() != form.r.hgt) return 0;
            for(int i=0; i<c.size(); i++){
                if(c[i].size() != form.r.len) return 0;

                for(int j=0; j<c[i].size(); j++)
                    color[i][j] = c[i][j];
            }
            return 1;
        }

        bool can_move(Map m){
            if(m.is_barrier(next_move())) return 0;
            return 1;
        }

        void try_move(Map m){
            if(!cooldown_walk() && can_move(m)){ 
                form.r.p = next_move();
                walk_cd = walk_cd_original;
            }
        }

        bool collide_with(Mob other){
            if(form.collide_with(other.form)) return 1;
            return 0;
        }

        void set_direction(int i){
            d = i;
        }

        void heal(int hp){
            health += hp;
            if(health > max_health) health = max_health;
        }

        void take_damage(int hp){
            health -= hp;
            if(health < 0) health = 0;
        }

        void print(){
            for(int i=0; i<form.r.hgt; i++){
                for(int j=0; j<form.r.len; j++){
                    std::cout << colors[color[i][j]] << form.m[i][j] << RESET;
                }
                std::cout << '\n';
            }
        }

        int show_health(){
            return health;
        }

};
