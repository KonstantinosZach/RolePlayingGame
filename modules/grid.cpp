#include <array>
#include <stdlib.h>
#include <time.h>

#include "grid.h"
#include "general.h"
#include "living.h"
#include "usuables.h"
#include "items.h"
#include "spells.h"

// Square functions ///

Square::Square(std::string state):state(state){}

Square::~Square(){}

void Square::set_empty(bool empty) { this->empty = empty;}

bool Square::is_empty()const{ return empty; }

void Square::print()const{

    if(get_state().compare("common") == 0){
        if(is_empty())
            std::cout << "\033[1;42m        \033[0m";
        else
            std::cout << "\033[1;42m  \033[0m" << " X " <<"\033[1;42m   \033[0m";
    }
    else if(get_state().compare("unaccessible") == 0){
        if(is_empty())
            std::cout << "\033[1;41m        \033[0m";
    }
    else{
        if(is_empty())
            std::cout << "\033[1;44m        \033[0m";
        else
            std::cout << "\033[1;44m  \033[0m" << " X " <<"\033[1;44m   \033[0m";
    }

}

std::string Square::get_state()const{ return state; }

// Common functions ///

Common::Common():Square("common"){}

//Επιστρέφει την κατάσταση της μάχης [0->continue / 1->heroes won / -1->monsters won]
int Common::end_of_the_fight(std::vector<Monster*> monsters, std::vector<Hero*> heroes){
    bool monsters_fainted = true;
    for(uint i=0; i<monsters.size(); i++){
        if(monsters[i]->get_health_power() > 0)
            monsters_fainted = false;
    }

    bool heroes_fainted = true;
    for(uint i=0; i<heroes.size(); i++){
        if(heroes[i]->get_health_power() > 0)
            heroes_fainted = false;
    }

    if(heroes_fainted)
        return -1;
    else if(monsters_fainted)
        return 1;
    else
        return 0;
}

//Δημιουργεί τα τέρατα που θα αντιμετοπίσουν οι ήρωες
std::vector<Monster*> Common::make_group_of_monsters(int level, int max_group_size){
    std::vector<Monster*> monsters;
    int size = rand() % max_group_size + 1;
    for(int i=0; i<size; i++){
        int category = rand() % 3;
        switch (category){
        case 0:
            monsters.push_back(new Dragon(Names::monsters[rand() % Names::monsters.size()], level));
            break;
        case 1:
            monsters.push_back(new Exosceleton(Names::monsters[rand() % Names::monsters.size()], level));
            break;
        default:
            monsters.push_back(new Spirit(Names::monsters[rand() % Names::monsters.size()], level));
            break;
        }
    }
    return monsters;
}

// Unaccessible functions ///

Unaccessible::Unaccessible():Square("unaccessible"){}

// Shop functions ///
//Γενικά υπάρχει η εξής λογική στις συναρτήσεις του Shop 
//Οταν βλέπουμε main_category εννοούμε [1.armor 2.weapon 3.potion 4.spell]
//Τα sub_category του weapon [1 -> one handed / 2 -> two handed]
//Τα sub_category του potion [1 -> health potion / 2 -> magic potion / 3 -> strength potion]
                            //[4 -> agility potion / 5 -> dexterity potion]
//Τα sub_category του spell [1 -> fire spell / 2 -> ice spell / 3 -> lighting spell]
Shop::Shop():Square("shop"){
    for(uint i=0, j=1; i<Names::armor.size() && j <= (uint)Living::get_max_level(); i++, j++){
        products.push_back(new Armor(Names::armor[i], j));
    }
    for(uint i=0, j=1; i<Names::potions.size() && j <= (uint)Living::get_max_level(); i++, j++){
        products.push_back(new HealthPotion(Names::potions[i], j));
        products.push_back(new MagicPotion(Names::potions[i], j));
        products.push_back(new StrengthPotion(Names::potions[i], j));
        products.push_back(new AgilityPotion(Names::potions[i], j));
        products.push_back(new DexterityPotion(Names::potions[i], j));
    }
    for(uint i=0, j=1; i<Names::weapons.size() && j <= (uint)Living::get_max_level(); i++, j++){
        products.push_back(new OneHanded(Names::weapons[i], j));
        products.push_back(new TwoHanded(Names::weapons[i], j));
    }
    for(uint i=0, j=1; i<Names::firespell.size() && j <= (uint)Living::get_max_level(); i++, j++){
        products.push_back(new FireSpell(Names::firespell[i], j));
    }
    for(uint i=0, j=1; i<Names::icespell.size() && j <= (uint)Living::get_max_level(); i++, j++){
        products.push_back(new IceSpell(Names::icespell[i], j));
    }
    for(uint i=0, j=1; i<Names::lightingspell.size() && j <= (uint)Living::get_max_level(); i++, j++){
        products.push_back(new LightingSpell(Names::lightingspell[i], j));
    }
}

Shop::~Shop(){
    for(uint i=0; i<products.size(); i++)
        delete products[i];
    products.clear();
}

//Επιστρέφει όλα τα αντικείμενα "armor" από το shop
std::vector<Useables*> Shop::find_armors(){
    std::vector<Useables*> wanted_items;
    for(uint i=0; i<products.size(); i++){
        if(products[i]->get_type().compare("armor") == 0)
            wanted_items.push_back(products[i]);
    }
    return wanted_items;
}

//Επιστρέφει όλα τα αντικείμενα μιας υποκατηγορίας "potion"
//[1 -> health potion / 2 -> magic potion / 3 -> strength potion]
//[4 -> agility potion / 5 -> dexterity potion]
std::vector<Useables*> Shop::find_potions(int sub_category){
    std::vector<Useables*> wanted_items;
    switch (sub_category){
        case 1:
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("health_potion") == 0)
                    wanted_items.push_back(products[i]);
            }
            return wanted_items;
        case 2 :
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("magic_potion") == 0)
                    wanted_items.push_back(products[i]);
            }
            return wanted_items;
        case 3 :
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("strength_potion") == 0)
                    wanted_items.push_back(products[i]);
            }
            return wanted_items;
        case 4 :
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("agility_potion") == 0)
                    wanted_items.push_back(products[i]);
            }
            return wanted_items;
        default :
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("dexterity_potion") == 0)
                    wanted_items.push_back(products[i]);
            }
            return wanted_items;
    }
}

//Επιστρέφει όλα τα αντικείμενα μιας υποκατηγορίας "weapon"
//[1 -> one handed / 2 -> two handed]
std::vector<Useables*> Shop::find_weapons(int sub_category){
    std::vector<Useables*> wanted_items;
    switch (sub_category){
        case 1:
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("one_handed") == 0)
                    wanted_items.push_back(products[i]);
            }
            return wanted_items;
        default :
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("two_handed") == 0)
                    wanted_items.push_back(products[i]);
            }
            return wanted_items;
    }
}

//Επιστρέφει όλα τα αντικείμενα μιας υποκατηγορίας "spell"
//[1 -> fire spell / 2 -> ice spell / 3 -> lighting spell]
std::vector<Useables*> Shop::find_spells(int sub_category){
    std::vector<Useables*> wanted_spells;
    switch (sub_category){
        case 1:
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("fire_spell") == 0)
                    wanted_spells.push_back(products[i]);
            }
            return wanted_spells;
        case 2:
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("ice_spell") == 0)
                    wanted_spells.push_back(products[i]);
            }
            return wanted_spells;            
        default :
            for(uint i=0; i<products.size(); i++){
                if(products[i]->get_type().compare("lighting_spell") == 0)
                    wanted_spells.push_back(products[i]);
            }
            return wanted_spells;
    }    
}

//Eκτυπώνει αριθμημένα τα αντικείνα του vector
void Shop::print(std::vector<Useables*> products)const{
    if(!products.empty()){
        for(uint i=0; i<products.size(); i++){
        std::cout << i+1 << ".";
        products[i]->info();
        std::cout << std::endl;
        }
    }
}

//Πραγματοποιεί αγορά αντικειμένων μέσα από το vector του ορίσματος
void Shop::buy(std::vector<Useables*> products, std::vector<Hero*> group_of_heroes){

    int number;
    do{
        //Στην αρχή επιλέγουμε τον ήρωα για τον οποίο θέλουμε να γίνει η αγορά
        system("clear");
        std::cout << "Avaible Heroes: " << group_of_heroes.size() << std::endl;
        for(uint i=0; i<group_of_heroes.size(); i++){
            std::cout << i+1 << "." << group_of_heroes[i]->get_name();
            std::cout << " money: " << group_of_heroes[i]->get_money() << std::endl;
        }
        std::cout << "0.Exit" << std::endl;

        std::cout << "\n" << "For which hero you want to buy items: " << std::endl;
        std::cout << "You can choose a hero using the numbers " << 1 << " to "
        << group_of_heroes.size() << "\n" << "or 0 to exit" << std::endl;  

        Input<int, 1>::get_choice(number);
        while(number < 0 || number > (int)group_of_heroes.size()){
            std::cout << "Wrong input..." << std::endl;
            std::cout << "Try again..." << std::endl;
            Input<int, 1>::get_choice(number);      
        }
        
        //Αμα πάρουμε την τιμή 0 τότε φεύγουμε από την συνάρτηση γιατί ο χρήστης δεν θέλει να
        //αγοράσει για καποιον ήρωα κάτι άλλο
        if(number != 0){
        
            int id_of_product;
            do{
                system("clear");
                print(products);

                //Εδω ο χρήστης επιλέγει ποιο αντικείμενο θέλει να αγοράσει
                std::cout << "\n" << "What would you like to buy:" << std::endl;
                std::cout << "You can choose an item/spell using the numbers " << 1 << " to "
                << products.size()<< "\n" << "or 0 to exit" << std::endl;
                std::cout << "Hero's money: " << group_of_heroes[number - 1]->get_money() << std::endl;

                Input<int, 2>::get_choice(id_of_product);
                while(id_of_product < 0 || id_of_product > (int)products.size()){
                    std::cout << "Wrong input..." << std::endl;
                    std::cout << "Try again..." << std::endl;
                    Input<int, 2>::get_choice(id_of_product);      
                }

                //Αν πάρουμε την τιμή 0 τότε το πρόγραμμα ρωτάει τον χρήστη αν θέλει
                //να αποχωρήσει από την λίστα των αντικειμένων
                if(id_of_product != 0){

                    //Ελέχγουμε αν το level του ήρωα αρκεί για την αγορά
                    if(products[id_of_product - 1]->get_required_level() > group_of_heroes[number - 1]->get_level()){
                            std::cout << "Can't buy the item/spell" << "\n" << "Level of hero to low" << std::endl;
                    }
                    else{
                        //Το πρόγραμμα ρωτάει τον χρήστη για την επαλήθευση της αγοράς
                        std::cout << "You want the hero: " << group_of_heroes[number - 1]->get_name() << "\n"
                        << "to acquire this item: "<< products[id_of_product-1]->get_name() << "\n"
                        << "for the price of: " << products[id_of_product -1]->get_price() <<"?(y/n)" << std::endl;

                        std::string choice;
                        Input<std::string, 1>::get_choice(choice);
                        while(choice.compare("y") != 0 && choice.compare("n") != 0){
                            std::cout << "Wrong input..." << std::endl;
                            std::cout << "Try again..." << std::endl;
                            Input<std::string, 1>::get_choice(choice);
                        }
                        if(choice.compare("y") == 0){
                            int money = group_of_heroes[number -1]->get_money();
                            //Ελέγχουμε αν ο ήρωας έχει τα απαραίτητα χρήματα για την αγορά
                            if(money >= products[id_of_product -1]->get_price()){
                                group_of_heroes[number - 1]->set_money(-products[id_of_product -1]->get_price());
                                group_of_heroes[number - 1]->add_on_inventory(products[id_of_product -1]);
                            }
                            else{
                                std::cout << "Sorry but this hero cant afford the item" << std::endl;
                            }

                        }
                    }
                }
            }while(id_of_product != 0);// Η λούπα πραγματοποιείται μέχρι να φύγει ο χρήστης από την λίστα προιόντων
        }
    }while(number != 0);//Η λούπα πραγματοποιείται μέχρι να μην επιλέξει καποιον ήρωα ο χρήστης
}

//Πραγματοποιεί πώληση αντικειμένων μέσα από inventory των ηρώων
void Shop::sell(std::vector<Hero*> group_of_heroes, int main_category, int sub_category){
    int number;
    do{
        //Στην αρχή επιλέγουμε τον ήρωα για τον οποίο θέλουμε να πουλήσουμε κάτι
        system("clear");
        std::cout << "Avaible Heroes: " << group_of_heroes.size() << std::endl;
        for(uint i=0; i<group_of_heroes.size(); i++){
            std::cout << i+1 << "." << group_of_heroes[i]->get_name();
            std::cout << " money: " << group_of_heroes[i]->get_money() << std::endl;
        }
        std::cout << "0.Exit" << std::endl;

        std::cout << "\n" << "From which hero you want to sell items: " << std::endl;
        std::cout << "You can choose a hero using the numbers " << 1 << " to "
        << group_of_heroes.size() << "\n" << "or 0 to exit" << std::endl;  

        Input<int, 1>::get_choice(number);
        while(number < 0 || number > (int)group_of_heroes.size()){
            std::cout << "Wrong input..." << std::endl;
            std::cout << "Try again..." << std::endl;
            Input<int, 1>::get_choice(number);      
        }

        //Αμα πάρουμε την τιμή 0 τότε φεύγουμε από την συνάρτηση γιατί ο χρήστης δεν θέλει να
        //πουλήσει από καποιον ήρωα κάτι άλλο    
        if(number != 0){

            int place = 1;  // 1->un-equiped items 0->equiped items
            do{
                std::vector<Useables*> products;
                //Μόνο τα armors και weapons έχουν την δυνατότητα να είναι equiped ή όχι
                //Ο χρήστης θα πρέπει να διαλέξει από που θέλει να κάνει sell
                if(main_category == 1 || main_category == 2){
                    system("clear");
                    std::cout << "1.Inventory" << "\n" << "2.Equiped Items" << "\n" << "0.Exit" << std::endl;

                    Input<int, 1>::get_choice(place);
                    while(place < 0 || place > 2){
                        std::cout << "Wrong input..." << std::endl;
                        std::cout << "Try again..." << std::endl;
                        Input<int, 1>::get_choice(place);      
                    }
                }

                //Αν πάρουμε 0 τότε ο χρήστης επιστρέφει στην επιλογή ήρωα
                if(place != 0){

                    int id_of_product;
                    std::string choice;
                    do{
                        system("clear");
                        if(place == 1){
                            products = group_of_heroes[number - 1]->find_in_inventory(main_category, sub_category);
                        }
                        else if(place == 2){
                            products = group_of_heroes[number - 1]->get_equiped_items(main_category, sub_category);
                        }
                        //Αμα δεν υπάρχει κάτι στο inventory τοτε περιμένουμε την τιμη 0 
                        if(products.empty()){
                            std::cout << "Empty" << "\n" << "Press 0 to exit" << std::endl;
                        }
                        //Αλλιως ο χρήστης μπορεί να επιλέξει τι θέλει να κάνει sell
                        else{
                            print(products);
                            std::cout << "\n" << "What would you like to sell:" << std::endl;
                            std::cout << "You can choose an item using the numbers " << 1 << " to "
                            << products.size()<< "\n" << "or 0 to exit" << std::endl;
                            std::cout << "Hero's money: " << group_of_heroes[number - 1]->get_money() << std::endl;            
                        }

                        Input<int, 2>::get_choice(id_of_product);
                        while(id_of_product < 0 || id_of_product > (int)products.size()){
                            std::cout << "Wrong input..." << std::endl;
                            std::cout << "Try again..." << std::endl;
                            Input<int, 2>::get_choice(id_of_product);      
                        }

                        //Αν πάρουμε την τιμή 0 τοτε ο χρήστης δεν μπορεί ή δεν θέλει να κάνει κάτι άλλο sell
                        if(id_of_product != 0){
                            //Aλλιως τον ρωτάμε αν είναι σίγουρος για την πώληση
                            std::cout << "You want the hero: " << group_of_heroes[number - 1]->get_name() << "\n"
                            << "to sell this item: "<< products[id_of_product-1]->get_name() << "\n"
                            << "for the price of: " << products[id_of_product -1]->get_price()/2 <<"?(y/n)" << std::endl;

                            Input<std::string, 1>::get_choice(choice);
                            while(choice.compare("y") != 0 && choice.compare("n") != 0){
                                std::cout << "Wrong input..." << std::endl;
                                std::cout << "Try again..." << std::endl;
                                Input<std::string, 1>::get_choice(choice);
                            }
                            if(choice.compare("y") == 0){
                                if(place == 1){
                                    group_of_heroes[number - 1]->remove_from_inventory(products[id_of_product - 1]->get_id());
                                }
                                else{
                                    group_of_heroes[number - 1]->remove_equiped_item(products[id_of_product - 1]->get_id());
                                }
                                group_of_heroes[number - 1]->set_money(products[id_of_product -1]->get_price()/2);
                            }
                        }
                    }while(id_of_product != 0);//Η λούπα γίνεται μεχρι ο χρήστης να φύγει από την λίστα των αντικειμένων προς πώληση
                    if(main_category > 2){
                        place = 0;
                    }
                }
            }while(place != 0);//Αυτο το βήμα αφορά μόνο τα weapons/armors
        }
    }while(number != 0);//Η λούπα γίνεται μεχρι ο χρήστης να μην επιλέξει κάποιον ήρωα
}

// Grid functions ///
Grid::Grid(){

    //Βάση αυτού του array θα δημιουργηθεί το map του παιχνιδιού
    //[x -> unaccessible / c -> common / s -> shop]
    std::array<std::array<std::string, map_rows>, map_colums> blueprint_of_map =
        {"x", "x", "x", "x", "x", "x", "x", "x", "x", "x",
        "x", "c", "c", "c", "c", "c", "c", "c", "c", "x",
        "x", "x", "c", "c", "x", "c", "c", "c", "x", "x",
        "x", "x", "s", "c", "s", "c", "c", "c", "s", "x",
        "x", "c", "c", "x", "c", "c", "c", "x", "c", "x",
        "x", "c", "x", "c", "s", "c", "x", "c", "c", "x",
        "x", "c", "c", "c", "x", "c", "c", "c", "x", "x",
        "x", "c", "s", "x", "c", "c", "x", "s", "c", "x",
        "x", "c", "c", "c", "c", "s", "c", "c", "c", "x",
        "x", "x", "x", "x", "x", "x", "x", "x", "x", "x",};

    //Δημιουργία του map
    for(int i=0; i<map_rows; i++){
        for(int j=0; j<map_colums; j++){

            if(blueprint_of_map[i][j].compare("c") == 0)
                map[i][j] = new Common();
            else if(blueprint_of_map[i][j].compare("x") == 0)
                map[i][j] = new Unaccessible();
            else
                map[i][j] = new Shop();

        }
    }
    srand (time(NULL));
    make_group_of_heroes();
}

Grid::~Grid(){

    for(int i=0; i<map_rows; i++){
        for(int j=0; j<map_colums; j++){
            delete map[i][j];
        }
    }

    for(uint i=0; i<group_of_heroes.size(); i++){
        delete group_of_heroes[i];
    }
    group_of_heroes.clear();
}

//Προσθέτει ήρωες (έως number) συγκεκριμένου τύπου (type) στο vector ηρώων
void Grid::add_hero(std::string type , int& number){

    std::cout << "How many " << type << "s do you want?" << std::endl;
    std::cout << "Avaible positions " << number << std::endl;

    int choice;
    Input<int,1>::get_choice(choice);
    while(choice > number  || choice < 0){
        std::cout << "Wrong Input" << std::endl;
        std::cout << "Try again..." << std::endl;
        std::cout << "Avaible positions " << number << std::endl;
        Input<int,1>::get_choice(choice);
    }

    for(int i=0; i<choice; i++){
        if(type.compare("Warrior") == 0){
            group_of_heroes.push_back(new Warrior(Names::heroes[rand() % Names::heroes.size()]));
        }
        else if(type.compare("Sorcerer") == 0){
            group_of_heroes.push_back(new Sorcerer(Names::heroes[rand() % Names::heroes.size()]));
        }
        else if(type.compare("Paladin") == 0){
            group_of_heroes.push_back(new Paladin(Names::heroes[rand() % Names::heroes.size()]));
        }
        else{
            std::cout << "Wrong argument" << std::endl;
            std::cout << "Expected Warrior/Sorcerer/Paladin" << std::endl;
        }
    }

    number -= choice;
    if(number == 0){
        std::cout << "Selection over" << std::endl;
    }   
}

//Καθορίζει πόσους και τι είδους ήρωες θα χειρίζεται ο χρήστης
void Grid::make_group_of_heroes(){

    system("clear");
    std::cout << "You can control between 1-3 heroes" << std::endl;
    std::cout << "How many heroes do you want?" << std::endl;

    int choice;
    Input<int, 1>::get_choice(choice);
    while (choice <= 0 || choice > 3){
        std::cout << "Wrong input... Try again" << std::endl;
        Input<int, 1>::get_choice(choice);
    }

    std::cout << "You selected: " << choice << " heroes" << std::endl;
    std::cout << "\n" << "Different types of heroes" << std::endl;
    std::cout << "Warrior -> Damage and Agility boost" << std::endl;
    std::cout << "Sorcerer -> Dexterity and Agility boost" << std::endl;
    std::cout << "Paladin -> Damage and Dexterity boost" << std::endl;
    std::cout << std::endl;

    //Η λούπα γίνεται οσο ο χρήστης έχει έλευθερη θέση για να βάλει κάποιον ήρωα
    while(choice > 0){
        add_hero("Warrior", choice);
        if(choice == 0)
            break;

        add_hero("Sorcerer", choice);
        if(choice == 0)
            break;

        add_hero("Paladin", choice);
        if(choice == 0)
            break; 

        std::cout << "You didnt select enough Heroes" << std::endl;
    }

    //ενημερώνουμε το square ότι οι ήρωες βρίσκονται εκεί
    map[group_position[0]][group_position[1]]->set_empty(false);

}

//Εκτυπώνεται το map του παιχνιδιου
void Grid::print_map()const{

    std::cout << "\033[1;42m        \033[0m";
    std::cout << " for common square" << "\n" <<std::endl;
    std::cout << "\033[1;41m        \033[0m";
    std::cout << " for unaccessible square" << "\n" << std::endl;
    std::cout << "\033[1;44m        \033[0m";
    std::cout << " for shop square" << "\n" << std::endl;
    std::cout << " X -> Where the hero is currently" << "\n" << std::endl;

    for(int i=0; i<map_rows; i++){
        for(int j=0; j<map_colums; j++){
            map[i][j]->print();
            std::cout << " ";
        }
        std::cout << "\n" << std::endl;
    }
}

std::array<int,2> Grid::get_group_position()const{ return group_position; }

void Grid::set_group_position(int x, int y){
    group_position[0] = x;
    group_position[1] = y;
    map[x][y]->set_empty(false);
}

std::vector<Hero*> Grid::get_group_of_heroes()const{ return group_of_heroes; }

//Προκαλεί την έναρξη του παιχνιδιού
void Grid::play(){

    std::string choice;
    std::cout << "\n" << "Do you want to start the game?(y/n)" << std::endl;
    Input<std::string, 1>::get_choice(choice);

    while(choice.compare("n") != 0 && choice.compare("y")){
        std::cout << "Wrong input..." << std::endl;
        std::cout << "Do you want to start the game?(y/n)" << std::endl;
        Input<std::string, 1>::get_choice(choice);
    }

    if(choice.compare("n") == 0)    //τερματισμός προγράμματος
        return;

    //Μερικές πληροφορίες για τις επιλογές που έχει ο χρήστης
    system("clear");
    std::cout << "\n" << "The game begins!" << std::endl;
    std::cout << "You can:" << std::endl;
    std::cout << "-move using w-a-s-d" << std::endl;
    std::cout << "-quit using q" << std::endl;
    std::cout << "-equip items using e" << std::endl;
    std::cout << "-check invetory using i" << std::endl;
    std::cout << "-consume potions using u" << std::endl;
    std::cout << "-check hero's stats using o" << "\n" << std::endl;
    print_map();

    Input<std::string, 1>::get_choice(choice);
    system("clear");
    while(choice.compare("q") != 0){
        if(choice.compare("i") == 0){
            check_inventory();    
            print_map();
            Input<std::string, 1>::get_choice(choice);
        }
        else if(choice.compare("e") == 0){
            equip();
            print_map();
            Input<std::string, 1>::get_choice(choice);
        }
        else if(choice.compare("u") == 0){
            use();
            print_map();
            Input<std::string, 1>::get_choice(choice);
        }
        else if(choice.compare("o") == 0){
            heroes_stats();
            print_map();
            Input<std::string, 1>::get_choice(choice);         
        }
        else{
            if(move(choice)){
                print_map();
            }
            Input<std::string, 1>::get_choice(choice);
        }
        system("clear");
    }
}

//Με βάση αυτή τη συνάρτηση γίνεται η διάσχιση του shop
//είτε για να αγοράσει ο χρήστης είτε για να πουλήσει
void Grid::explore_shop(Shop* shop){

    std::cout << "Do you want to use the shop?(y/n)" << std::endl;
    std::string choice;
    Input<std::string, 1>::get_choice(choice);
    while(choice.compare("n") != 0 && choice.compare("y") != 0){
        std::cout << "Wrong input..." << std::endl;
        std::cout << "Do you want to use the shop?(y/n)" << std::endl;
        Input<std::string, 1>::get_choice(choice);
    }

    if(choice.compare("y") == 0){

        //Ο χρήστης επιλέγει άμα θέλει να αγοράσει, να πουλήσει ή να φύγει
        system("clear");
        int main_category;
        std::cout << "\033[1;44m Welcome to the shop \033[0m" << std::endl;
        std::cout << "1.Buy" << "\n" << "2.Sell" << "\n" << "0.Exit" << std::endl;
        Input<int, 1>::get_choice(main_category);
        while(main_category < 0 || main_category > 2){
            std::cout << "Wrong input..." << std::endl;
            std::cout << "Try again..." << std::endl;
            Input<int, 1>::get_choice(main_category);      
        }

        //Όσο ο χρήστης δεν κάνει έξοδο από το shop
        while(main_category != 0){

                //Ο χρήστης επιλέγει μια κατηγορία ή κάνει έξοδο
                system("clear");
                std::cout << "Items categories:" << std::endl;
                std::cout << "1.Armor" << "\n" << "2.Weapons" << "\n"<< "3.Potions"<< "\n"<< "4.Spells"
                << "\n" << "0.Exit" << std::endl;
                std::cout << "\n" << "You can choose a category using the numbers 1-4 or 0 to exit" << std::endl;
                int product_category;
                Input<int, 1>::get_choice(product_category);
                while(product_category < 0 || product_category > 4){
                    std::cout << "Wrong input..." << std::endl;
                    std::cout << "Try again..." << std::endl;
                    Input<int, 1>::get_choice(product_category);
                }

                //Όσο δεν κάνει έξοδο
                std::vector<Useables*> products;
                while(product_category != 0){

                    //Επέλεξε τα armors
                    if(product_category == 1){
                        products = shop->find_armors();
                        //και ανάλογα αγοράζει ή πουλάει
                        if(main_category == 1)
                            shop->buy(products, get_group_of_heroes());
                        else
                            shop->sell(get_group_of_heroes(),product_category);
                    }

                    //Επέλεξε τα weapons
                    else if(product_category == 2){

                        //Kαι τώρα επιλέγει ποια υποκατηγορία θέλει ή κάνει έξοδο
                        system("clear");
                        std::cout << "Weapons categories:" << std::endl;
                        std::cout << "1.One Handed" << "\n" << "2.Two Handed"<< "\n" << "0.Exit" << std::endl;
                        std::cout << "\n" << "You can choose a category using the numbers 1-2 or 0 to exit" << std::endl;

                        int weapon_category;
                        Input<int, 1>::get_choice(weapon_category);
                        while(weapon_category < 0 || weapon_category > 2){
                            std::cout << "Wrong input..." << std::endl;
                            std::cout << "Try again..." << std::endl;
                            Input<int, 1>::get_choice(weapon_category);
                        }

                        //Όσο επιλέγει κάποια υποκατηγορία όπλου
                        while(weapon_category != 0){
                            products = shop->find_weapons(weapon_category);
                            system("clear");
                            if(main_category == 1)
                                shop->buy(products, get_group_of_heroes());
                            else
                                shop->sell(get_group_of_heroes(), product_category, weapon_category);

                            //Ο χρήστης ρωτάται άμα θέλει να συνεχίσει τις αγορές ή κάνει έξοδο από τις κατηγορίες όπλων
                            system("clear");
                            std::cout << "Weapons categories:" << std::endl;
                            std::cout << "1.One Handed" << "\n" << "2.Two Handed"<< "\n" << "0.Exit" << std::endl;
                            std::cout << "\n" << "You can choose a category using the numbers 1-2 or 0 to exit" << std::endl;
                            Input<int, 1>::get_choice(weapon_category);

                            while(weapon_category < 0 || weapon_category > 2){
                                std::cout << "Wrong input..." << std::endl;
                                std::cout << "Try again..." << std::endl;
                                Input<int, 1>::get_choice(weapon_category);
                            }
                        }
                    }

                    //Επέλεξε potions
                    else if(product_category == 3){

                        system("clear");
                        std::cout << "Potion categories:" << std::endl;
                        std::cout << "1.Health Potions" << "\n" << "2.Magic Potions"<< "\n" << "3.Strength Potions" 
                        << "\n" << "4.Agility Potions"<< "\n" << "5.Dexterity Potions" << "\n" << "0.Exit" << std::endl;
                        std::cout << "\n" << "You can choose a category using the numbers 1-5 or 0 to exit" << std::endl;  

                        int potion_category;
                        Input<int, 1>::get_choice(potion_category);
                        while(potion_category < 0 || potion_category > 5){
                            std::cout << "Wrong input..." << std::endl;
                            std::cout << "Try again..." << std::endl;
                            Input<int, 1>::get_choice(potion_category);
                        }

                        while(potion_category != 0){
                            products = shop->find_potions(potion_category);
                            system("clear");
                            if(main_category == 1)
                                shop->buy(products, get_group_of_heroes());
                            else
                                shop->sell(get_group_of_heroes(), product_category, potion_category);

                            system("clear");
                            std::cout << "Potion categories:" << std::endl;
                            std::cout << "1.Health Potions" << "\n" << "2.Magic Potions"<< "\n" << "3.Strength Potions" 
                            << "\n" << "4.Agility Potions"<< "\n" << "5.Dexterity Potions" << "\n" << "0.Exit" << std::endl;
                            std::cout << "\n" << "You can choose a category using the numbers 1-5 or 0 to exit" << std::endl; 
 
                            Input<int, 1>::get_choice(potion_category);
                            while(potion_category < 0 || potion_category > 5){
                                std::cout << "Wrong input..." << std::endl;
                                std::cout << "Try again..." << std::endl;
                                Input<int, 1>::get_choice(potion_category);
                            }
                        }

                    }

                    //Aλλιώς επέλεξε spells
                    else{

                        system("clear");
                        std::cout << "Spell categories:" << std::endl;
                        std::cout << "1.Fire Spell" << "\n" << "2.Ice Spell"<< "\n" << "3.Lighting Spell"<< "\n" << "0.Exit" << std::endl;
                        std::cout << "\n" << "You can choose a category using the numbers 1-3 or 0 to exit" << std::endl;  

                        int spell_category;
                        Input<int, 1>::get_choice(spell_category);
                        while(spell_category < 0 || spell_category > 3){
                            std::cout << "Wrong input..." << std::endl;
                            std::cout << "Try again..." << std::endl;
                            Input<int, 1>::get_choice(spell_category);
                        }

                        while(spell_category != 0){
                            products = shop->find_spells(spell_category);
                            system("clear");
                            if(main_category == 1)
                                shop->buy(products, get_group_of_heroes());
                            else
                                shop->sell(get_group_of_heroes(), product_category, spell_category);

                            system("clear");
                            std::cout << "Spell categories:" << std::endl;
                            std::cout << "1.Fire Spell" << "\n" << "2.Ice Spell"<< "\n" << "3.Lighting Spell"<< "\n" << "0.Exit" << std::endl;
                            std::cout << "\n" << "You can choose a category using the numbers 1-3 or 0 to exit" << std::endl;
 
                            Input<int, 1>::get_choice(spell_category);
                            while(spell_category < 0 || spell_category > 3){
                                std::cout << "Wrong input..." << std::endl;
                                std::cout << "Try again..." << std::endl;
                                Input<int, 1>::get_choice(spell_category);
                            }
                        }                   
                    }

                    //Ο χρήστης επιλέγει άμα θέλει να συνεχίσει τις αγορές ή κάνει έξοδο από τις γενικές κατηγορίες
                    system("clear");
                    std::cout << "Items categories:" << std::endl;
                    std::cout << "1.Armor" << "\n" << "2.Weapons" << "\n"<< "3.Potions"<< "\n"<< "4.Spells"
                    << "\n" << "0.Exit" << std::endl;

                    std::cout << "\n" << "You can choose a category using the numbers 1-4 or 0 to exit" << std::endl;
                    Input<int, 1>::get_choice(product_category);  
                    while(product_category < 0 || product_category > 4){
                        std::cout << "Wrong input..." << std::endl;
                        std::cout << "Try again..." << std::endl;
                        Input<int, 1>::get_choice(product_category);
                    }

                }

                //Ο χρήστης ρωτάται άμα θέλει να συνεχίσει τις αγορές ή κάνει έξοδο από το shop
                products.clear();
                system("clear");
                std::cout << "\033[1;44m Welcome to the shop \033[0m" << std::endl;
                std::cout << "1.Buy" << "\n" << "2.Sell" << "\n" << "0.Exit" << std::endl;
                Input<int, 1>::get_choice(main_category);
                while(main_category < 0 || main_category > 2){
                    std::cout << "Wrong input..." << std::endl;
                    std::cout << "Try again..." << std::endl;
                    Input<int, 1>::get_choice(main_category);      
                }

            }
    }
    system("clear");
}

void Grid::equip(){

    int choice;
    do{

        system("clear");
        std::cout << "Which hero you want to equip with items?" << std::endl;
        for(uint i=0; i<group_of_heroes.size(); i++){
            std::cout << i+1 << "." << group_of_heroes[i]->get_name() << std::endl;
        }
        std::cout << "0.Exit" << std::endl;
        
        Input<int,1>::get_choice(choice);
        while(choice < 0 || (int)group_of_heroes.size() < choice){
            std::cout << "Wrong input..." << std::endl;
            std::cout << "Try again" << std::endl;
            Input<int,1>::get_choice(choice);
        }

        if(choice != 0)
            group_of_heroes[choice - 1]->explore_inventory("equip");

    }while(choice != 0);//H λούπα γίνεται μέχρι ο χρήστης να μην θέλει καποιος άλλος ήρωας να κάνει equip items

}

void Grid::use(){

    int choice;
    do{

        system("clear");
        std::cout << "Which hero you want to use potions?" << std::endl;
        for(uint i=0; i<group_of_heroes.size(); i++){
            std::cout << i+1 << "." << group_of_heroes[i]->get_name() << std::endl;
        }
        std::cout << "0.Exit" << std::endl;
        
        Input<int,1>::get_choice(choice);
        while(choice < 0 || (int)group_of_heroes.size() < choice){
            std::cout << "Wrong input..." << std::endl;
            std::cout << "Try again" << std::endl;
            Input<int,1>::get_choice(choice);
        }

        if(choice != 0)
            group_of_heroes[choice - 1]->explore_inventory("use");

    }while(choice != 0);//H λούπα γίνεται μέχρι ο χρήστης να μην θέλει καποιος άλλος ήρωας να κάνει use potions
}

void Grid::heroes_stats(){

    int choice;
    do{

        system("clear");
        std::cout << "For which hero you want to show stats?" << std::endl;
        for(uint i=0; i<group_of_heroes.size(); i++){
            std::cout << i+1 << "." << group_of_heroes[i]->get_name() << std::endl;
        }
        std::cout << "0.Exit" << std::endl;
        
        Input<int,1>::get_choice(choice);
        while(choice < 0 || (int)group_of_heroes.size() < choice){
            std::cout << "Wrong input..." << std::endl;
            std::cout << "Try again" << std::endl;
            Input<int,1>::get_choice(choice);
        }

        if(choice != 0){
            std::string escape;
            do{
                system("clear");
                group_of_heroes[choice - 1]->info();

                //Ο χρήστης ρωταται άμα θ΄λει να φύγει από τα stats του ήρωα
                std::cout << "\n" << "Do you want to leave?(y/n)" << std::endl;
                Input<std::string,1>::get_choice(escape);
                while(escape.compare("y") != 0 && escape.compare("n") != 0){
                    std::cout << "Wrong input..." << std::endl;
                    std::cout << "Try again" << std::endl;
                    Input<std::string,1>::get_choice(escape);
                }
            }while(escape.compare("n") == 0);
        }

    }while(choice != 0);//H λούπα γίνεται μέχρι ο χρήστης να μην θέλει να δει τα stats καποιου άλλου ήρωα
}

//Διαχειρίζεται τις διαφορετικές επιθέσεις των ηρώων προς τα τέρατα
void Grid::fight(Common* common){

    //Κάθε φορά που οι ήρωες μπαίνουν σε common square
    //έχουν 50% πιθανότητα να ξεκινήσουν μια μάχη
    int fight_possibility = (rand() % 2);

    //άμα δεν κάνουν μάχη τότε απλά προχωράνε στο επόμενο square
    if(fight_possibility == 0)
        return;

    int level = group_of_heroes[0]->get_level();
    int size = group_of_heroes.size();
    std::vector<Monster*> monsters = common->make_group_of_monsters(level, size);

    std::cout << "\033[1;41m A fight is about to start \033[0m" << std::endl;
    std::cout << "You will encounter the following monsters:" << std::endl;
    for(int i=0; i<(int)monsters.size(); i++){
        std::cout << i+1 << ".";
        monsters[i]->info();
    }

    int end = 0;
    do{

        //O κάθε ήρωας του group έχει από ένα γύρο
        for(int i=0; i<(int)group_of_heroes.size(); i++){

            bool turn_finished = false;
            //Οσο είναι ζωντανός ο ήρωας και όσο δεν εχει πραγματοποιηθει ακόμα ο γύρος του
            //και όσο κάποιο από τα τέρατα ζει θα συνεχίζεται η λούπα
            while(!turn_finished && common->end_of_the_fight(monsters, get_group_of_heroes()) != 1
                && !group_of_heroes[i]->get_status()){

                std::cout << "Hero's turn: ";
                std::cout << group_of_heroes[i]->get_name();
                std::cout << std::endl;

                //Επιλέγει την κίνησή του
                std::cout << "Select your next move" << std::endl;
                std::cout <<"1.Attack" << "\n" << "2.Cast Spell" << "\n"
                << "3.Use Potion"<< "\n" << "4.Inventory" << "\n" << "5.Check Hero Stats" << std::endl;

                int choice;
                Input<int,1>::get_choice(choice);
                while(choice < 1 || choice > 5){
                    std::cout << "Wrong Input..." << std::endl;
                    std::cout << "Try again..." << std::endl;
                    Input<int,1>::get_choice(choice);
                }

                //Απλή επίθεση
                if(choice == 1){
                    turn_finished = group_of_heroes[i]->attack(monsters);
                }
                //Cast κάποιου spell
                else if(choice == 2){
                    turn_finished = group_of_heroes[i]->explore_inventory("cast");
                    if(turn_finished)
                        turn_finished = group_of_heroes[i]->cast_spell(monsters);
                }
                //Γίνεται χρήση κάποιου potion
                else if(choice == 3){
                    turn_finished = group_of_heroes[i]->explore_inventory("use+");
                }
                //Γίνονται equip κάποια items
                else if(choice == 4){
                    turn_finished = group_of_heroes[i]->explore_inventory("equip");
                }
                //Εμφανίζονται τα stats του ήρωα
                else if(choice == 5){
                    std::string choice;
                    do{
                        system("clear");
                        group_of_heroes[i]->info();
                        std::cout << "Do you want to return to leave?(y/n)" << std::endl;
                        Input<std::string,1>::get_choice(choice);
                        while(choice.compare("y") != 0 && choice.compare("n") != 0){
                            std::cout << "Wrong Input..." << std::endl;
                            std::cout << "Try again..." << std::endl;
                            Input<std::string,1>::get_choice(choice);
                        }
                    }while(choice.compare("n") == 0);
                }
            }

            //Aμα έχει γίνει κάποιο cast spell
            //τοτε εδώ θα εφαρμοστουν και οι επιδράσεις τους στα κατάλληλα τέρατα
            for(int i=0; i<(int)monsters.size(); i++){
                monsters[i]->apply_effects();
            }
        }

        //Τα τέρατα με την σειρά τους κάνουν επίθεση στους ήρωες
        for(int i=0; i<(int)monsters.size(); i++){
            monsters[i]->attack(get_group_of_heroes());
        }

        //Γίνεται restore στο health power των τεράτων
        for(int i=0; i<(int)monsters.size(); i++){
            monsters[i]->restore();
        }

        //Γίνεται restore στο health/magic power των ηρώων
        for(int i=0; i<(int)group_of_heroes.size(); i++){
            group_of_heroes[i]->restore();
        }

        //Ελέγχουμε άμα ε΄χει τελειώσει η μάχη
        end = common->end_of_the_fight(monsters, get_group_of_heroes());
    }while(end == 0);

    //Αμα νίκησαν οι ήρωες
    if(end == 1){

        std::cout << "\n" << "\033[1;42m The heroes won the fight \033[0m"<< std::endl;
        int extra_money = 25;
        extra_money = extra_money * monsters.size() + extra_money * monsters[0]->get_level();

        int extra_experience = 15;
        extra_experience = extra_experience * monsters.size() + extra_experience * monsters[0]->get_level();

        for(int i=0; i<(int)group_of_heroes.size(); i++){
            group_of_heroes[i]->set_money(extra_money);
            group_of_heroes[i]->update_experience(extra_experience);
            group_of_heroes[i]->revive();
        }
    }
    //Αμα έχασαν οι ήρωες
    else{

        std::cout << "\n" << "\033[1;41m The monsters won the fight \033[0m"<< std::endl;
        for(int i=0; i<(int)group_of_heroes.size(); i++){
            group_of_heroes[i]->set_money(-group_of_heroes[i]->get_money()/2);
            group_of_heroes[i]->revive();
        } 
    }
    
    //Διαγραφή των τεράτων
    for(int i=0; i<(int)monsters.size(); i++){
        delete monsters[i];
    }
    
}

//Οι ήρωες μετακινούνται στα map
bool Grid::move(std::string direction){

    std::array<int,2> current_position = get_group_position();
    int x = current_position[0];
    int y = current_position[1];

    //"w" -> στο πάνω square
    if(direction.compare("w") == 0){

        //έλεγχουμε τι τύπου είναι το πάνω square
        //αν ειναι διαφορο το unaccessible καλούμε την κατάλληλη συνάρτηση
        if(map[x - 1][y]->get_state().compare("unaccessible") != 0){
            
            //και ενημερώνουμε την θέση των ηρώων πάνω στον χάρτη
            set_group_position(x - 1, y);
            map[x][y]->set_empty(true);

            if(map[x - 1][y]->get_state().compare("shop") == 0){
                explore_shop((Shop*)map[x - 1][y]);
            }
            else{
                fight((Common*)map[x - 1][y]);
            }

            return true;
        }
        //αλλιως ενημερώνουμε τον χρήστη ότι δεν μπορεί να πάει εκεί
        else{
            std::cout << "Cant move up..." << std::endl;
            std::cout << "Try a different direction" << "\n" << std::endl;
            print_map();
            return false;
        }
    }
    else if(direction.compare("s") == 0){

        if(map[x + 1][y]->get_state().compare("unaccessible") != 0){

            set_group_position(x + 1, y);
            map[x][y]->set_empty(true);

            if(map[x + 1][y]->get_state().compare("shop") == 0){
                explore_shop((Shop*)map[x + 1][y]);
            }
            else{
                fight((Common*)map[x + 1][y]);
            }

            return true;
        }
        else{
            std::cout << "Cant move down" << std::endl;
            std::cout << "Try a different direction" << "\n" << std::endl;
            print_map();
            return false;
        }
    }
    else if(direction.compare("a") == 0){

        if(map[x][y - 1]->get_state().compare("unaccessible") != 0){

            set_group_position(x, y - 1);
            map[x][y]->set_empty(true);

            if(map[x][y - 1]->get_state().compare("shop") == 0){
                explore_shop((Shop*)map[x][y - 1]);
            }
            else{
                fight((Common*)map[x][y - 1]);
            }

            return true;
        }
        else{
            std::cout << "Cant move left" << std::endl;
            std::cout << "Try a different direction" << "\n" << std::endl;
            print_map();
            return false;
        }
    }
    else if(direction.compare("d") == 0){

        if(map[x][y + 1]->get_state().compare("unaccessible") != 0){

            set_group_position(x, y + 1);
            map[x][y]->set_empty(true);

            if(map[x][y + 1]->get_state().compare("shop") == 0){
                explore_shop((Shop*)map[x][y + 1]);
            }
            else{
                fight((Common*)map[x][y + 1]);
            }

            return true;
        }
        else{
            std::cout << "Cant move right" << std::endl;
            std::cout << "Try a different direction" << "\n" << std::endl;
            print_map();
            return false;
        }
    }
    else{
        std::cout << "Wrong input..." << std::endl;
        std::cout << "Try again" << "\n" << std::endl;
        print_map();
        return false;
    }
}

void Grid::check_inventory(){

    int choice;
    do{

        system("clear");
        std::cout << "For which hero you want to check inventory?" << std::endl;
        for(uint i=0; i<group_of_heroes.size(); i++){
            std::cout << i+1 << "." << group_of_heroes[i]->get_name() << std::endl;
        }
        std::cout << "0.Exit" << std::endl;
        
        Input<int,1>::get_choice(choice);
        while(choice < 0 || (int)group_of_heroes.size() < choice){
            std::cout << "Wrong input..." << std::endl;
            std::cout << "Try again" << std::endl;
            Input<int,1>::get_choice(choice);
        }

        if(choice != 0)
            group_of_heroes[choice - 1]->explore_inventory();

    }while(choice != 0);

}