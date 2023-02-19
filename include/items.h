#pragma once
#include <iostream>
#include "usuables.h"

class Item : public Useables{

    private:
        static const int price_modifier = 10;   //βαση αυτού θα αλλάζει η τιμή των αντικειμένων σε κάθε level

    protected:
        int required_level = 0;
        std::string type;       // "armor" / "potion"

        Item(std::string name, int required_level, std::string type);
    
    public:
        virtual ~Item() = 0;
        int get_required_level()const;
        virtual std::string get_type()const;
        virtual void info()const;
};

class Armor : public Item{
    
    private:
        static const int damage_reflection_modifier = 8;
        int damage_reflection = 0;
    
    public:
        Armor(std::string name, int required_level);
        int get_damage_reflection()const;
        void info()const;
};

class Potion : public Item{

    protected:
        std::string type;   // "health potion" / "magic potion" / "strength potion"
                            // "dexterity potion" / "agility potion"

        Potion(std::string name, int required_level, std::string type);

    public:
        virtual ~Potion() = 0;
        std::string get_type()const;
        virtual void info()const;
};

class HealthPotion : public Potion{

    private:
        int increase = 0;
        static const int health_increase_modifier = 45;
    
    public:
        HealthPotion(std::string name, int required_level);
        int get_increase()const;
        void info()const;
};

class MagicPotion : public Potion{

    private:
        int increase = 0;
        static const int magic_increase_modifier = 35;
    
    public:
        MagicPotion(std::string name, int required_level);
        int get_increase()const;
        void info()const;
};


class StrengthPotion : public Potion{

    private:
        int increase = 0;
        static const int strength_increase_modifier = 10;
    
    public:
        StrengthPotion(std::string name, int required_level);
        int get_increase()const;
        void info()const;
};

class DexterityPotion : public Potion{

    private:
        int increase = 0;
        static const int dexterity_increase_modifier = 25;
    
    public:
        DexterityPotion(std::string name, int required_level);
        int get_increase()const;
        void info()const;
};

class AgilityPotion : public Potion{

    private:
        float increase = 0.0;
        static constexpr float agility_increase_modifier = 0.03;
    
    public:
        AgilityPotion(std::string name, int required_level);
        float get_increase()const;
        void info()const;
};

class Weapon : public Item{

    protected:
        std::string type;   //"one handed" / "two handed"
        int damage = 0;
        static const int damage_modifier = 25;

        Weapon(std::string name, int required_level, std::string type);

    public:
        virtual ~Weapon() = 0;
        std::string get_type()const;
        virtual int get_damage()const;
        virtual void info()const;
};

class OneHanded : public Weapon{

    public:
        OneHanded(std::string name, int required_level);
        void info()const;
};

class   TwoHanded : public Weapon{

    private:
        int damage_boost = 10;

    public:
        TwoHanded(std::string name, int required_level);
        void info()const;
};