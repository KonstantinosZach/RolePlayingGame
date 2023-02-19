#include "spells.h"

// Spell functions ///
Spell::Spell(std::string name, int required_level, std::string type):
    Useables(name, "spell"), required_level(required_level), type(type){
        range_of_damage[0] = 20;
        range_of_damage[1] = 35;
        for(int i=0; i<required_level; i++){
            price += price_modifier;
            range_of_damage[0] += range_of_damage[0]  * stats_modifier;
            range_of_damage[1] += range_of_damage[1]  * stats_modifier;
            mana_cost += mana_cost * stats_modifier;
        }

    }

Spell::~Spell(){}

int Spell::get_required_level()const{ return required_level; }

std::string Spell::get_type()const{ return type;}

int Spell::get_min_damage()const{ return range_of_damage[0]; }

int Spell::get_max_damage()const{ return range_of_damage[1]; }

int Spell::get_mana_cost()const{ return mana_cost; }

void Spell::info()const{
    std::cout << "name: " << name << std::endl;
    std::cout << "price: " << price << std::endl;
    std::cout << "range of damage: " << range_of_damage[0] << "-" << range_of_damage[1] << std::endl;
    std::cout << "mana cost: " << mana_cost << std::endl;
    std::cout << "required level: "  << required_level << std::endl;
}

// FireSpell functions ///
FireSpell::FireSpell(std::string name,int required_level):Spell(name,required_level, "fire_spell") {
    for(int i = 0; i<required_level; i++){
        reduced_armor += reduced_armor_modifier;
        //Για κάθε 2 leves θα αυξάνονταιι κατά 1 τα turns των effect
        if(i%2 == 0){
            turns++;
        }
    }
}

int FireSpell::get_armor_reduction()const{ return reduced_armor; }

int FireSpell::get_turns()const{ return turns; }

void FireSpell::info()const{
    std::cout << "Fire Spell information" << std::endl;
    Spell::info();
    std::cout << "armor reduction: " << reduced_armor << std::endl;
    std::cout << "duration: " << turns << " turns" << std::endl;    
}

// IceSpell functions ///
IceSpell::IceSpell(std::string name,int required_level):Spell(name,required_level, "ice_spell") {
    for(int i = 0; i<required_level; i++){
        reduced_damage += reduced_damage_modifier;
        if(i%2 == 0){
            turns++;
        }
    }
}

int IceSpell::get_damage_reduction()const{ return reduced_damage; }

int IceSpell::get_turns()const{ return turns; }

void IceSpell::info()const{
    std::cout << "Ice Spell information" << std::endl;
    Spell::info();
    std::cout << "damage reduction: " << reduced_damage << std::endl;
    std::cout << "duration: " << turns << " turns" << std::endl;   
}

// LightingSpell functions ///
LightingSpell::LightingSpell(std::string name,int required_level):Spell(name,required_level, "lighting_spell") {
    for(int i = 0; i<required_level; i++){
        reduced_agility += reduced_agility_modifier;
        if(i%2 == 0){
            turns++;
        }
    }
}
float LightingSpell::get_agility_reduction()const{ return reduced_agility; }

int LightingSpell::get_turns()const{ return turns; }

void LightingSpell::info()const{
    std::cout << "Lighting Spell information" << std::endl;
    Spell::info();
    std::cout << "agility reduction: " << reduced_agility << std::endl;
    std::cout << "duration: " << turns << " turns" << std::endl;    
}