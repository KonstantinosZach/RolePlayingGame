#include "items.h"

// Item  functions///
Item::Item(std::string name, int required_level, std::string type)
:Useables(name, "item"), required_level(required_level), type(type){
    for(int i=0; i<required_level; i++)
        price += price_modifier;
}

Item::~Item(){}

std::string Item::get_type()const{ return type;}

int Item::get_required_level()const{ return required_level; }

void Item::info()const{
    std::cout << "Item information" << std::endl;
    std::cout << "Type: " << type << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Price: " << price << std::endl;
    std::cout << "Required level: " << required_level << std::endl;
}

// Armor functions ///
Armor::Armor(std::string name, int required_level):Item(name, required_level, "armor"){
    for(int i=0; i<required_level; i++)
        damage_reflection += damage_reflection_modifier;
}

int Armor::get_damage_reflection()const{ return damage_reflection; }

void Armor::info()const{
    Item::info();
    std::cout << "Damage reflection: " << damage_reflection << std::endl;
}

// Potion functions ///
Potion::Potion(std::string name, int required_level, std::string type)
:Item(name, required_level, "potion"), type(type){}

Potion::~Potion(){}

void Potion::info()const{
    Item::info();
    std::cout << "Attribute: " << type << std::endl;
}

std::string Potion::get_type()const{ return Potion::type; }

// Health Potion functions
HealthPotion::HealthPotion(std::string name, int required_level)
:Potion(name, required_level, "health_potion"){
    for(int i=0; i<required_level; i++)
        increase += health_increase_modifier;
}

int HealthPotion::get_increase()const{ return increase; }

void HealthPotion::info()const{
    Potion::info();
    std::cout << "Increase: "  << increase << std::endl;
}

// Magic Potion functions
MagicPotion::MagicPotion(std::string name, int required_level)
:Potion(name, required_level, "magic_potion"){
    for(int i=0; i<required_level; i++)
        increase += magic_increase_modifier;
}

int MagicPotion::get_increase()const { return increase; }

void MagicPotion::info()const{
    Potion::info();
    std::cout << "Increase: " << increase << std::endl;
}


// Strength Potion functions
StrengthPotion::StrengthPotion(std::string name, int required_level)
:Potion(name, required_level, "strength_potion"){
    for(int i=0; i<required_level; i++)
        increase += strength_increase_modifier;
}

int StrengthPotion::get_increase()const { return increase; }

void StrengthPotion::info()const{
    Potion::info();
    std::cout << "Increase: " << increase << std::endl;
}

// Dexterity Potion functions
DexterityPotion::DexterityPotion(std::string name, int required_level)
:Potion(name, required_level, "dexterity_potion"){
    for(int i=0; i<required_level; i++)
        increase += dexterity_increase_modifier;
}

int DexterityPotion::get_increase()const { return increase; }

void DexterityPotion::info()const{
    Potion::info();
    std::cout << "Increase: " << increase << std::endl;
}

// Agility Potion functions
AgilityPotion::AgilityPotion(std::string name, int required_level)
:Potion(name, required_level, "agility_potion"){
    for(int i=0; i<required_level; i++)
        increase += agility_increase_modifier;
}

float AgilityPotion::get_increase()const { return increase; }

void AgilityPotion::info()const{
    Potion::info();
    std::cout << "Increase: " << increase << std::endl;
}

// Weapon functions ///
Weapon::Weapon(std::string name, int required_level, std::string type)
:Item(name, required_level, "weapon"), type(type){
    for(int i=0; i<required_level; i++)
        damage += damage_modifier;
}

std::string Weapon::get_type()const{ return Weapon::type; }

int Weapon::get_damage()const{ return damage; }

Weapon::~Weapon(){}

void Weapon::info()const{
    Item::info();
    std::cout << "damage: " << damage << std::endl;   
}

/// One-handed Weapon functions ///
OneHanded::OneHanded(std::string name, int required_level):Weapon(name, required_level, "one_handed"){}

void OneHanded::info()const{
    Weapon::info();
    std::cout << "type: One Handed" << std::endl;
}

// Two-handed Weapon functions ///
TwoHanded::TwoHanded(std::string name, int required_level):Weapon(name, required_level, "two_handed"){
    damage += damage_boost;
}

void TwoHanded::info()const{
    Weapon::info();
    std::cout << "type: Two Handed" << std::endl;
}