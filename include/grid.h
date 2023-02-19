#pragma once
#include <iostream>
#include <array>
#include <vector>
#include "living.h"
#include "usuables.h"

//Κλάση που περιγράφει τα τετράγωνα 
//από τα οποία αποτελείται ο χάρτης
class Square{

    protected:
        std::string state;      // "common" / "unaccessible" / "shop"
        bool empty = true;      // αν βρίσκονται οι ηρωές σε αυτό το τετράγωνο ή όχι

    public:
        Square(std::string state);
        virtual ~Square() = 0;
        std::string get_state()const;
        void set_empty(bool empty);
        bool is_empty()const;
        void print()const;
};

class Common : public Square{

    public:
        Common();
        std::vector<Monster*> make_group_of_monsters(int level, int max_group_size);
        int end_of_the_fight(std::vector<Monster*> monsters, std::vector<Hero*> heroes);
};

class Unaccessible : public Square{

    public:
        Unaccessible();
};

class Shop : public Square{ 

    private:
        std::vector<Useables*> products;        //Τα αντικείμενα που περιέχει το Shop για πώληση
    
    public:
        Shop();
        ~Shop();
        std::vector<Useables*> find_armors();
        std::vector<Useables*> find_potions(int sub_category);
        std::vector<Useables*> find_weapons(int sub_category);
        std::vector<Useables*> find_spells(int sub_category);

        void print(std::vector<Useables*> products)const;
        void buy(std::vector<Useables*> products, std::vector<Hero*> group_of_heroes);
        void sell(std::vector<Hero*> group_of_heroes, int category, int sub_category = 0);
};

//Κλάση που περιγράφει τον χάρτη του παιχνιδιού
class Grid{

    private:
        int static const map_rows = 10;
        int static const map_colums = 10;
        std::array<std::array<Square*, map_rows>, map_colums> map;
        std::vector<Hero*> group_of_heroes;         //το vector που περιέχει τους ήρωες
        std::array<int,2> group_position = {{8,4}}; //η αρχική τοποθεσία των ηρώων πάνω στον χάρτη
        

        void add_hero(std::string type, int& number);
        void make_group_of_heroes();
    public:
        Grid();
        ~Grid();
        void print_map()const;
        bool move(std::string direction);
        void explore_shop(Shop* shop);
        void fight(Common* common);
        void equip();
        void play();
        void use();
        void heroes_stats();
        void check_inventory();
        std::vector<Hero*> get_group_of_heroes()const;
        std::array<int,2> get_group_position()const;
        void set_group_position(int x, int y);
        
};