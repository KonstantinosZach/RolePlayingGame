#pragma once
#include <iostream>
#include <iomanip>

template <typename T, int length>
class Input{
    public:
        static void get_choice(T& choice);
};

class Names{
    public:
        static std::array<std::string,10> armor;
        static std::array<std::string,10> potions;
        static std::array<std::string,10> weapons;
        static std::array<std::string,10> firespell;
        static std::array<std::string,10> icespell;
        static std::array<std::string,10> lightingspell;
        static std::array<std::string,10> heroes;
        static std::array<std::string,10> monsters;

};