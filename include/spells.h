#pragma once
#include <iostream>
#include <array>
#include "usuables.h"

class Spell : public Useables{

    private:
        static const int price_modifier = 25;
        static constexpr float stats_modifier = 0.35f;

    protected:
        int required_level;
        std::string type;       //"fire spell" / "ice spell" / "lighting spell"
        std::array<int,2> range_of_damage;
        int mana_cost = 25;

        Spell(std::string name,int required_level, std::string type);

    public:
        virtual ~Spell() = 0;
        int get_required_level()const;
        std::string get_type()const;
        int get_min_damage()const;
        int get_max_damage()const;
        int get_mana_cost()const;
        virtual int get_turns()const = 0;
        virtual void info()const;       
};

class FireSpell: public Spell{

    private:
        int reduced_armor = 0;
        static const int reduced_armor_modifier = 5;
        int turns = 1;      //για ποσους γύρους μάχης θα ισχύουν τα effects των spells
    
    public:
        FireSpell(std::string name, int required_level);
        int get_armor_reduction()const;
        int get_turns()const;
        void info()const;
};

class IceSpell: public Spell{

    private:
        int reduced_damage = 0;
        static const int reduced_damage_modifier = 15;
        int turns = 1;
    
    public:
        IceSpell(std::string name, int required_level);
        int get_damage_reduction()const;
        int get_turns()const;
        void info()const;
};

class LightingSpell: public Spell{

    private:
        float reduced_agility = 0.0;
        static constexpr float reduced_agility_modifier = 0.02;
        int turns = 1;
    
    public:
        LightingSpell(std::string name, int required_level);
        float get_agility_reduction()const;
        int get_turns()const;
        void info()const;
};

