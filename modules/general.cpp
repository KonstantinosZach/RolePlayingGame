#include "general.h"

//Συνάρτηση που ελέγχει το input του χρήστη
template <class T, int length>
void Input<T, length>::get_choice(T& choice){

    std::cout << "Enter choice" << std::endl;
    std::cin >> std::setw(length) >> choice;

    //Error check
    while(!std::cin.good()){

        //Report problem 
        std::cout << "Wrong input... Try again" << std::endl;

        //Clear stream
        std::cin.clear();
        std::cin.ignore(INTMAX_MAX, '\n');

        //Get new input
        std::cout << "Enter choice" << std::endl;
        std::cin >> std::setw(length) >> choice;      
    }

    //Clear stream
    std::cin.clear();
    std::cin.ignore(INTMAX_MAX, '\n');

}

template class Input<int, 1>;
template class Input<int, 2>;
template class Input<std::string, 1>;

// Define the names for our game

std::array<std::string,10> Names::armor = {"Genji Shield", "Crystal Shield", 
"Aegis Shield", "Golem Shield", "Revive Shield", "Rescue Shield", "Paladin Shield",
"Diamond Shield", "Ruby Shield", "Dynasty Shield"};

std::array<std::string,10> Names::potions = {"Red Potion", "Hi-Potion", "X-Potion", "Ether",
"Elixir", "Holy Water", "Healing Water", "Soul Spring", "Candle of Life", "Dream Powder"};

std::array<std::string,10> Names::weapons = {"Caladbolg", "Crystal Sword", "Excalibur", "Ragnarok",
"Durandal", "Ascalon", "Astral Sword", "Slowstrike", "Arc Sword", "Vendetta"};

std::array<std::string,10> Names::firespell = {"Flame", "Torch", "Eruption", "Flare",
"Blaze", "Inferno", "Fireball", "Explosion", "Meteor", "Dragon Fire"};

std::array<std::string,10> Names::icespell = {"Avalanche", "Chill", "Freeze", "Frostbite",
"Douse", "Deluge", "Flood", "Wave", "Tsunami", "Sinkhole"};

std::array<std::string,10> Names::lightingspell = {"Shock", "Bolt", "Plasma", "Spite",
"Channel Energy", "Yakamo’s Wrath", "Thunder", "Paralyzed", "Thor's hammer", "Zeus"};

std::array<std::string,10> Names::heroes = {"Abbathor", "Brandobaris", "Charmalaine", "Fharlanghn",
"Gadhelyn", "Moradin", "Roykyn", "Yondalla", "Eadro", "Orcus"};

std::array<std::string,10> Names::monsters = {"Zinzerena", "Pelor", "Yeenoghu", "Tiamat",
"Vaprak", "Bahgtru", "Falazure", "Hiatea", "Raxivort", "Squerrik"};