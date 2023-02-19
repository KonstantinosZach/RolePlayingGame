#pragma once
#include <iostream>
#include <vector>
#include <array>

#include "items.h"
#include "spells.h"
#include "usuables.h"

class Living{

    protected:
        std::string name;
        std::string type;               //"hero" / "monster"
        int health_power = 150;
        int max_health_power = 150;     //Το μεγιστo hp σε κάθε level
        static const int max_level = 5; //To μεγιστo level που μπορεί να φτάσει ο κάθε ήρωας
        int level = 1;
        bool fainted = false;
        static constexpr float stats_modifier = 0.25f;  //Με βάση αυτό αλλάζουν τα stats του ήρωα σε κάθε level up
        Living(std::string name, std::string type);
    
    public:
        virtual ~Living() = 0;
        std::string get_name()const;
        virtual std::string get_type()const;
        static int get_max_level();
        int get_level()const;
        int get_health_power()const;
        void update_health_power(int plus);
        bool get_status()const;
        void set_status(bool is_fainted);
        virtual void info()const = 0;
};

class Monster;

class Hero: public Living{

    protected:
        std::string type;           //"warrior" / "sorcerer" / "paladin"
        int magic_power = 100;
        int max_magic_power = 100;  //Το μεγιστo magic power σε κάθε level
        int strength = 35;
        int dexterity = 15;
        float agility = 0.1;
        int money = 100;
        int experience = 0;
        int max_experience = 100;   //Το exp που χρειάζεται για level up
        std::vector<Useables*> inventory;   //περιέχει όλα τα items και spells του ηρωα
        std::vector<Weapon*> weapons;       //περιέχει τα weapons που έχει κάνει equip
        Armor* armor = NULL;                //περιέχει τo armor που έχει κάνει equip
        Spell* spell = NULL;                //περιέχει τo spell που θα κάνει cast στην μάχη

        Hero(std::string name, std::string type);

    public:
        virtual ~Hero() = 0;
        std::string get_type()const;
        int get_magic_power()const;
        void update_magic_power(int plus);
        int get_strength()const;
        int get_dexterity()const;
        float get_agility()const;
        int get_money()const;
        void set_money(int balance);
        virtual void level_up(int levels = 1) = 0;
        int get_experience()const;
        void update_experience(int plus);
        int get_defense()const;
        virtual void info()const;

        bool explore_inventory(std::string type = "view");
        void print(std::vector<Useables*> products);
        void add_on_inventory(Useables* product);
        void remove_from_inventory(int id);
        std::vector<Useables*> find_in_inventory(int main_category, int sub_category);
        bool find_in_inventory(int useable_id);
        std::vector<Useables*> get_equiped_items(int main_category, int sub_category);
        void remove_equiped_item(int id);
        bool equip_item(std::vector<Useables*> products);
        bool equip_spell(std::vector<Useables*> products);
        bool use(std::vector<Useables*> products);
        bool attack(std::vector<Monster*> monsters);
        bool cast_spell(std::vector<Monster*> monsters);
        void restore();
        void revive();
};

class Warrior : public Hero{

    private:
        int strenght_boost = 5;
        float agility_boost = 0.01;

    public:
        Warrior(std::string name);
        void level_up(int levels = 1);
        void info()const;

};

class Sorcerer : public Hero{

    private:
        float agility_boost = 0.02;
        int dexterity_boost = 2;

    public:
        Sorcerer(std::string name);
        void level_up(int levels = 1);
        void info()const;

};

class Paladin : public Hero{

    private:
        int dexterity_boost = 5;
        int strenght_boost = 2;

    public:
        Paladin(std::string name);
        void level_up(int levels = 1);
        void info()const;

};

class Monster : public Living{

    protected:
        std::string type;       //"dragon" , "exosceleton", "spirit"
        std::array<int,2> range_of_damage{ {20, 40} };
        int defense = 5;
        float agility = 0.15;

        //Τα effects μετά από casting spells
        //και οι γύροι που δειαρκούν

        int damage_reduced = 0;
        int damage_reduced_turns = 0;

        int defense_reduced = 0;
        int defense_reduced_turs = 0;

        float agility_reduced = 0.0;
        int agility_reduced_turns = 0;

        Monster(std::string name, std::string type);
    
    public:

        virtual ~Monster() = 0;
        std::string get_type()const;
        int get_defense()const;
        float get_agility()const;
        int get_max_damage()const;
        int get_min_damage()const;
        virtual void info()const;

        void set_defense_reduction(int reduction, int turns);
        void set_damage_reduction(int reduction, int turns);
        void set_agility_reduction(float reduction, int turns);

        void apply_effects();
        void attack(std::vector<Hero*> group_of_heroes);
        void restore();
};

class Dragon : public Monster{

    private:
        int damage_boost = 5;
    
    public:
        Dragon(std::string name, int level = 1);
        void info()const;
};

class Exosceleton : public Monster{

    private:
        int defense_boost = 5;
    
    public:
        Exosceleton(std::string name, int level = 1);
        void info()const;
};

class Spirit : public Monster{

    private:
        float agility_boost = 0.015;
    
    public:
        Spirit(std::string name, int level = 1);
        void info()const;
};