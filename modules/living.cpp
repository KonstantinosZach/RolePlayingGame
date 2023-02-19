#include <vector>
#include "living.h"
#include "general.h"

// Living functions ///
Living::Living(std::string name, std::string type):name(name), type(type){}

Living::~Living(){}

std::string Living::get_name()const{ return name; }

std::string Living::get_type()const{ return type; }

int Living::get_max_level(){ return max_level; }

int Living::get_level()const{ return level; }

int Living::get_health_power()const{ return health_power; }

//Άμα πέσει η ζωή κάτω του 0 τότε ο ηρωας γίνεται fainted
void Living::update_health_power(int plus){ 
    health_power += plus;
    if(health_power <=0)
        set_status(true);
}

//Επιστρέφει false αν ειναι fainted και true αν είναι ζωντανο
bool Living::get_status()const{ return fainted; }

void Living::set_status(bool is_fainted){  fainted = is_fainted; }

// Hero functions ///
Hero::Hero(std::string name, std::string type):Living(name, "hero"), type(type){}

Hero::~Hero(){
    inventory.clear();
    weapons.clear();
}

int Hero::get_magic_power()const{ return magic_power; }

std::string Hero::get_type()const { return type; }

void Hero::update_magic_power(int plus){ magic_power += plus; }

int Hero::get_strength()const{ return strength; }

int Hero::get_dexterity()const{ return dexterity; }

float Hero::get_agility()const{ return agility; }

int Hero::get_money()const{ return money; }

void Hero::set_money(int balance){ 
    money += balance; 
    std::cout << "New currency: " << money << std::endl ;
}

int Hero::get_experience()const{ return experience; }

void Hero::update_experience(int plus){
    //αν έχω φτάσει στο max level τότε ο ήρωας δεν κερδίζει άλλο exp
    if(level == max_level){
        experience = 0;
        std::cout << "Name: " << name << " Max level reached: " << level << std::endl;
    }
    else{
        experience += plus;
        //άμα φτάνει στο εκάστοτε max_experience τότε γίνεται level up
        if(experience >= max_experience){
            experience -= max_experience;
            max_experience *= 2;
            level_up();
        }
        std::cout << "Name: " << name << " current experience: " << experience << " out of: " << max_experience
        << " level:" << level << std::endl;
    }
}

//Επαναφέρει ενα μικρό ποσοστό του συνολικού health power και magic power
//όταν ο ήρωας δεν έχει πεθάνει
void Hero::restore(){

    int health_increase = max_health_power/8;
    int magic_increase = max_magic_power/8;

    if(health_power > 0){
        if((health_power + health_increase) < max_health_power){
            health_power += health_increase;
        }
        else{
            health_increase = max_health_power - health_power;
            health_power = max_health_power;
        }

        if((magic_power + magic_increase) < max_magic_power){
            magic_power +=  magic_increase;
        }
        else{
            magic_increase = max_magic_power - magic_power;
            magic_power = max_magic_power;
        }

        std::cout << "Hero's name: " << name << "\n"
        << "Health power restored: " << health_increase << " current health power: " << health_power<< "\n"
        << "Magic power restored: " << magic_increase << " current health: " << magic_power << "\n" << std::endl;
    }

}

//Αναγενεί τον ήρωα όταν γίνεται fainted(hp <= 0) στη μάχη
void Hero::revive(){
    if(health_power <= 0){
        health_power = 0;
        std::cout << "Hero's name: " << name << "\n"
        << "Hero restored health power after getting fainted in battle" << std::endl;
        health_power += max_health_power/2;
        std::cout << "Health power restored: " << max_health_power/2 << " current health power: " << health_power << "\n" << std::endl;
        set_status(false);
    }
}

int Hero::get_defense()const{
    int defense = 0;
    if(armor != NULL){
        defense = armor->get_damage_reflection();
    }
    return defense;
}

//Εκτυπώνει τα products
//μαζί και με τα αντικείμενα που έχει κάνει equip ο χρήστης
void Hero::print(std::vector<Useables*> products){
    system("clear");
    if(!products.empty()){
        for(uint i=0; i<products.size(); i++){
            std::cout << i+1 << ".";
            products[i]->info();
            std::cout << std::endl;
        }
    }   
    else{
        std::cout << "Empty" << "\n" << std::endl;
    }
    std::cout << "The hero has currently equiped:" << "\n" << std::endl;
    if(!weapons.empty()){
        for(uint i=0; i<weapons.size(); i++){
            weapons[i]->info();
            std::cout << std::endl;
        }
    }
    if(armor != NULL){
        armor->info();
        std::cout << std::endl;
    }
    if(spell != NULL){
        spell->info();
        std::cout << std::endl;
    }
}

//Κάνει equip κάτι από τα products
//true -> έγινε equip / false -> δεν έγινε equip
bool Hero::equip_item(std::vector<Useables*> products){

    int id;
    bool inventory_edited = false;

    //Aρχικά ο ηρωας επιλεγει πιο από τα items θέλει να κάνει equip
    std::cout << "What you want to equip?" << "\n";
    std::cout << "You can choose an item using the numbers 1-" << products.size()
    << " or 0 to exit" << "\n";
    Input<int, 1>::get_choice(id);
    while(id < 0 || id > (int)products.size()){
        std::cout << "Wrong input..." << std::endl;
        std::cout << "Try again..." << std::endl;
        Input<int, 1>::get_choice(id);
    }

    //αμα παρουμε 0 τότε φεύγουμε γιατι ο χρήστης δεν θέλει να γίνει κατι equip
    if(id == 0)
        return inventory_edited;

    std::string choice;

    //Τα products που γίνονται equip είναι armor ή weapons
    //Oπότε ελέγχουμε να δούμε σε ποια περίπτωση βρισκόμαστε

    //Αν είναι armor τότε
    if(products[0]->get_type().compare("armor") == 0){
        //Αν δεν έχει κάτι άλλο equiped τότε το βάζουμε στη θέση του equip
        //και το βγαζουμε απο το inventory
        if(armor == NULL){
            armor = (Armor*)products[id - 1];
            remove_from_inventory(products[id - 1]->get_id());
            inventory_edited = true;
        }
        //Aλλιως αν έχει ήδη κατι equiped
        else{
            std::cout << "\n" << "Current armor:" << std::endl;
            armor->info();
            //Τον ρωτάμε αν θέλει να το αλλάξει
            std::cout << "\n" << "Do you want to replace your current armor?(y/n)" << std::endl;
            Input<std::string, 1>::get_choice(choice);
            while(choice.compare("y") != 0 && choice.compare("n") != 0){
                std::cout << "Wrong input..." << std::endl;
                std::cout << "Do you want to replace your current armor?(y/n)" << std::endl;
                Input<std::string, 1>::get_choice(choice);         
            }
            //και πραγματοποιούμε την αντικατάσταση
            if(choice.compare("y") == 0){
                add_on_inventory(armor);
                armor = (Armor*)products[id - 1];
                remove_from_inventory(products[id - 1]->get_id());
                inventory_edited = true;
            }
        }
    }
    //Αλλιως θα είναι weapon
    else{
        //Η περίπτωση που δεν έχει τιποτα equiped
        if(weapons.empty()){
            remove_from_inventory(products[id - 1]->get_id());
            weapons.push_back((Weapon*)products[id - 1]);
            inventory_edited = true;
        }
        //Η περίπτωση που έχουμε ήδη equiped 1 one-handed και θελουμε να βάλουμε αλλο 1 one-handed
        else if((int)weapons.size() == 1 && weapons[0]->get_type().compare("one_handed") == 0
        && products[id - 1]->get_type().compare("one_handed") == 0){
            remove_from_inventory(products[id - 1]->get_id());
            weapons.push_back((Weapon*)products[id - 1]);
            inventory_edited = true;
        }
        //Σε κάθε αλλη περίπτωση θα πρέπει να γίνει κάποια αντικατάσταση
        else{
            std::cout << "The hero cant carry more Weapons" << std::endl;
            std::cout << "Current Weapons:" << "\n" << std::endl;
            for(uint i=0; i<weapons.size(); i++){
                std::cout << i+1 << ".";
                weapons[i]->info();
                std::cout << std::endl;
            }
            //Η περίπτωση που έχουμε ήδη equiped 1 two-handed και θελουμε να βάλουμε αλλο 1 two-handed
            if(products[id - 1]->get_type().compare("two_handed") == 0
            || weapons[0]->get_type().compare("two_handed") == 0){          
                std::cout << "Do you want to replace your current weapons?(y/n)" << std::endl;
                Input<std::string, 1>::get_choice(choice);
                while(choice.compare("y") != 0 && choice.compare("n") != 0){
                    std::cout << "Wrong input..." << std::endl;
                    std::cout << "Do you want to replace your current weapons?(y/n)" << std::endl;
                    Input<std::string, 1>::get_choice(choice);         
                }
                if(choice.compare("y") == 0){
                    for(int i=0; i<(int)weapons.size(); i++){
                        add_on_inventory(weapons[i]);
                    }
                    weapons.clear();
                    remove_from_inventory(products[id - 1]->get_id());
                    weapons.push_back((Weapon*)products[id - 1]);
                    inventory_edited = true;
                }
            }
            //Η περίπτωση που έχουμε ήδη equiped 2 όπλα και θέλει να βάλει 1 one-handed
            else if((int)weapons.size() == 2 && products[id - 1]->get_type().compare("one_handed") == 0){
                std::cout << "\n" << "Do you want to replace a current weapon?(y/n)" << std::endl;
                Input<std::string, 1>::get_choice(choice);
                while(choice.compare("y") != 0 && choice.compare("n") != 0){
                    std::cout << "Wrong input..." << std::endl;
                    std::cout << "Do you want to replace a current weapon?(y/n)" << std::endl;
                    Input<std::string, 1>::get_choice(choice);         
                }
                //και επιλέγει πιο από τα δύο θέλει να αντικαταστήσει
                if(choice.compare("y") == 0){
                    std::cout << "Which one?" << std::endl;
                    int replace;
                    Input<int, 1>::get_choice(replace);
                    while(replace < 0 || replace > 2){
                        std::cout << "Wrong input..." << std::endl;
                        std::cout << "Try again..." << std::endl;
                        Input<int, 1>::get_choice(replace);
                    }
                    add_on_inventory(weapons[replace - 1]);
                    weapons.erase(weapons.begin() + (replace-1));
                    remove_from_inventory(products[id - 1]->get_id());
                    weapons.push_back((Weapon*)products[id - 1]);
                    inventory_edited = true;
                }
            }       
        }
    }
    return inventory_edited;
}

//Κανει equip ενα spell προσωρινά το οποίο θα κάνει cast ο ηρωας σε κάποιο τέρας
//true -> έγινε equip / false -> δεν έγινε equip
bool Hero::equip_spell(std::vector<Useables*> products){

    int id;
    bool inventory_edited = false;

    //Aρχικά ο ηρωας επιλεγει πιο από τα spell θέλει να κάνει cast
    std::cout << "What spell you want to cast?" << "\n";
    std::cout << "You can choose a spell using the numbers 1-" << products.size()
    << " or 0 to exit" << "\n";
    Input<int, 1>::get_choice(id);
    while(id < 0 || id > (int)products.size()){
        std::cout << "Wrong input..." << std::endl;
        std::cout << "Try again..." << std::endl;
        Input<int, 1>::get_choice(id);
    }

    //αμα παρουμε 0 τότε φεύγουμε γιατι ο χρήστης δεν θέλει να γίνει κατι cast
    if(id == 0)
        return inventory_edited;
    
    //αλλιως κάνουμε equip το spell
    if(spell == NULL){
        spell = (Spell*)products[id - 1];
        remove_from_inventory(products[id - 1]->get_id());
        inventory_edited = true;
    }
    return inventory_edited;
}

//Κάνει use ένα απο τα potions που περιέχουν τα products
//Tα αντικείμενα του vector πρέπει να είναι τύπου "potion"
//true -> έγινε use καποιου potion /false -> δεν έγινε use καποιου potion
bool Hero::use(std::vector<Useables*> products){

    int id;
    //Aρχικά ο ηρωας επιλέγει πιο από τα spell θέλει να κάνει cast
    std::cout << "Which potion you want to use?" << "\n";
    std::cout << "You can choose a potion using the numbers 1-" << products.size()
    << " or 0 to exit" << "\n";
    Input<int, 1>::get_choice(id);
    while(id < 0 || id > (int)products.size()){
        std::cout << "Wrong input..." << std::endl;
        std::cout << "Try again..." << std::endl;
        Input<int, 1>::get_choice(id);
    }

    //αμα παρουμε 0 τότε φεύγουμε γιατι ο χρήστης δεν θέλει να γίνει κατι use
    if(id == 0)
        return false;

    int increase;

    //και ανάλογα με τον τύπο του potion βελτιώνεται και το αντιστοιχο χαρακτηριστικό του ήρωα
    //πχ αν είναι health_power θα αυξηθεί η ζωη του ήρωα
    if(products[id - 1]->get_type().compare("health_potion") == 0){

        increase = dynamic_cast<HealthPotion*>(products[id - 1])->get_increase();

        //προσέχουμε να μην ξεπεράσουμε τη μέγιστη ζωή
        if(health_power == max_health_power){
            std::cout << "Yours health is full" << "\n";
            return false;
        }
        else{
            if(health_power + increase <= max_health_power){
                std::cout << "Your health power increased: " <<  increase << std::endl;
                health_power += increase;
            }
            else{
                std::cout << "Your health power increased: " <<  max_health_power - health_power << std::endl;
                health_power = max_health_power;
            }
            std::cout << "Current health power: " << health_power << " name:"  << name << "\n" << std::endl;
        }
    }
    else if (products[id - 1]->get_type().compare("magic_potion") == 0){

        increase = dynamic_cast<MagicPotion*>(products[id - 1])->get_increase();

        //προσέχουμε να μην ξεπεράσουμε τη μέγιστη magic power
        if(magic_power == max_magic_power){
            std::cout << "Your magic power is full" << "\n";
            return false;
        }
        else{
            if(magic_power + increase <= max_magic_power){
                std::cout << "Your magic power increased: " <<  increase << std::endl;
                magic_power += increase;
            }
            else{
                std::cout << "Your magic power increased: " <<  max_magic_power - magic_power << std::endl;
                magic_power = max_magic_power;
            }
            std::cout << "Current magic power: " << magic_power << " name:"  << name << "\n" << std::endl;
        }
    }
    else if (products[id - 1]->get_type().compare("strength_potion") == 0){
        increase = dynamic_cast<StrengthPotion*>(products[id - 1])->get_increase();
        strength += increase;
        std::cout << "Your strength increased: " <<  increase
        << "\n" << "Current strength: " << strength << " name:"  << name << "\n" << std::endl;
    }
    else if (products[id - 1]->get_type().compare("dexterity_potion") == 0){
        increase = dynamic_cast<DexterityPotion*>(products[id - 1])->get_increase();
        dexterity += increase;
        std::cout << "Your dexterity increased: " <<  dexterity
        << "\n" << "Current dexterity: " << dexterity << " name:"  << name << "\n" << std::endl;
    }
    else if(products[id - 1]->get_type().compare("agility_potion") == 0){
        float increase = dynamic_cast<AgilityPotion*>(products[id - 1])->get_increase();
        agility += increase;
        std::cout << "Your agility increased: " <<  agility
        << "\n" << "Current agility: " << agility << " name:"  << name << "\n" << std::endl;
    }    
    remove_from_inventory(products[id - 1]->get_id());
    return true;
}

//Κανει attack σε κάποιο από τα τέρατα του vector
//true -> έγινε attack / false -> δεν έγινε attack
bool Hero::attack(std::vector<Monster*> monsters){

    bool turn_finished = false;
    system("clear");

    do{
        //Aρχικά ο ηρωας επιλέγει πιο από τα τέρατα θέλει να κάνει attack
        std::cout << "Which monster you want to attack?" << std::endl;
        for(int i=0; i<(int)monsters.size(); i++){
            std::cout << i+1 << "." << monsters[i]->get_name()<< " Hp:" << monsters[i]->get_health_power()
            << " Damage: " << monsters[i]->get_min_damage() << "-" << monsters[i]->get_max_damage() << " Agility:"
            << monsters[i]->get_agility() << " Defense:" << monsters[i]->get_defense() << " Type: " << monsters[i]->get_type() << std::endl;
        }
        std::cout << "0.Exit" << std::endl;

        int choice;
        Input<int,1>::get_choice(choice);
        while(choice < 0 || choice >(int)monsters.size()){
            std::cout << "Wrong Input..." << std::endl;
            std::cout << "Try again..." << std::endl;
            Input<int,1>::get_choice(choice);
        }

        if(choice == 0)
            return turn_finished;

        //Με την προυπόθεση ότι το τέρας είναι ζωντανό
        if(!monsters[choice - 1]->get_status()){
            float agility = monsters[choice - 1]->get_agility();

            int evade = agility * 100;
            //Βλέπουμε άμα το τέρας καταφέρει κα κάνει evade το attack
            if( evade >= (rand() % 100 ) + 1 ){
                std::cout << "Hero's attack has been evaded" << "\n" << std::endl;
            }
            else{
                int damage = strength;
                //και ελέγχουμε άμα ο ήρωας έχει equiped weapons για να αυξήσουμε το damage
                if(!weapons.empty()){
                    for(int i=0; i<(int)weapons.size(); i++){
                        damage += weapons[i]->get_damage();
                    }
                }
                //αφαιρούμε από το damage το armor του τέρατος
                damage -= monsters[choice - 1]->get_defense();
                monsters[choice - 1]->update_health_power(- damage);

                std::cout << "Damage dealt: " << damage << std::endl;
                std::cout << "Monster's current health: " << monsters[choice - 1]->get_health_power() << "\n" << std::endl;
            }
            turn_finished = true;
        }
        else{
            std::cout << "This monster is already fainted" << std::endl;
            std::cout << "Select a different one" << std::endl;
        }
    }while(!turn_finished);
    return turn_finished;
}

//Kάνει cast το spell που έχει γίνει equip από πριν
//Aπαραίτητη προυπόθεση για να δουλέψει η cast_spell να έχει κληθεί με επιτυχία η equip_spell
//true -> έγινε cast καποιου spell /false -> δεν έγινε cast καποιου spell
bool Hero::cast_spell(std::vector<Monster*> monsters){

    system("clear");
    bool turn_finished = false;

    if(spell == NULL){
        std::cout << "No spell found!" << std::endl;
        return turn_finished;
    }

    do{
        //Αρχικά ο ήρωας επιλέγει σε ποιο τέρας θέλει να κάνει cast το spell
        std::cout << "Cast the spell at which monster?" << std::endl;
        for(int i=0; i<(int)monsters.size(); i++){
            std::cout << i+1 << ".";
            monsters[i]->info();
        }
        std::cout << "0.Exit" << std::endl;

        int choice;
        Input<int,1>::get_choice(choice);
        while(choice < 0 || choice >(int)monsters.size()){
            std::cout << "Wrong Input..." << std::endl;
            std::cout << "Try again..." << std::endl;
            Input<int,1>::get_choice(choice);
        }

        //Aμα δεν θελήσει να γίνει το cast βάζουμε το spell πίσω στο inventory
        if(choice == 0){
            add_on_inventory(spell);
            spell = NULL;
            return turn_finished;
        }
        
        //Αλλιως με την προυπόθεση το τέρας που επέλεξε να είναι ζωντανό
        if(!monsters[choice - 1]->get_status()){
            float agility = monsters[choice - 1]->get_agility();

            //Βλέπουμε άμα το τέρας καταφέρει κα κάνει evade το spell
            int evade = agility * 100;
            if( evade >= (rand() % 100 ) + 1 ){
                std::cout << "Hero's attack has been evaded" << "\n" << std::endl;
                spell = NULL;
                turn_finished = true;
            }
            else{
                //ελέγχεται αμα ο ήρωας έχει το απαραίτητο magic_power
                if(magic_power < spell->get_mana_cost()){
                    std::cout << "You need more magic power to cast this spell" << "\n" <<
                    "Select a different one or try using basic attack" << std::endl;
                }
                //και ανάλογα με τον τύπο του spell επηρεάζεται και το τέρας
                else{
                    magic_power -= spell->get_mana_cost();
                    int spell_damage = rand() % (spell->get_max_damage() + 1 - (spell->get_min_damage() + dexterity)) + spell->get_min_damage();
                    monsters[choice - 1]->update_health_power(- spell_damage);
                    
                    int turns = spell->get_turns();
                    if(spell->get_type().compare("fire_spell") == 0){
                        int armor_reduction = dynamic_cast<FireSpell*>(spell)->get_armor_reduction();
                        monsters[choice - 1]->set_defense_reduction(armor_reduction, turns);
                    }
                    else if(spell->get_type().compare("ice_spell") == 0){
                        int damage_reduction = dynamic_cast<IceSpell*>(spell)->get_damage_reduction();
                        monsters[choice - 1]->set_damage_reduction(damage_reduction, turns);
                    }
                    else{
                        float agility_reduction = dynamic_cast<LightingSpell*>(spell)->get_agility_reduction();
                        monsters[choice - 1]->set_agility_reduction(agility_reduction, turns);
                    }

                    std::cout << "Spell damage dealt: " << spell_damage << std::endl;
                    std::cout << "Monster's current health: " << monsters[choice - 1]->get_health_power() << "\n" << std::endl;

                    //και αφαιρούμε το spell από τον ήρωα
                    spell = NULL;
                    turn_finished = true;
                }
            }
        }
        else{
            std::cout << "This monster is already fainted" << std::endl;
            std::cout << "Select a different one" << std::endl;
        }
    }while(!turn_finished);
    return turn_finished;
}

//Συνάρτηση διασχισης του inventοry του ήρωα με διαφοριτικές λειτουργίες ανάλογα το όρισμα
//default = "view" -> εμφανίζεται το inventory του ηρωα
//"use" -> μπορούν να γίνουν πολλαπλά use potion από το inventory(έξω από μαχη)
//"use+" -> μπορεί να γίνει μόνο 1 use potion(κατα τη διαρκεια της μάχης)
//"equip" -> μπορεί να γίνουν equip weapons/armors
//"cast" -> μπορει να γίνει cast καποιο spell
bool Hero::explore_inventory(std::string type){

    bool inventory_edited = false;
    int product_category;
    do{
        system("clear");
        //αν έχουμε "view" ο χρήστης επιλέγει οποια κατηγορία θέλει να προβάλει
        if(type.compare("view") == 0){
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
        //αν εχουμε "equip" τότε ο χρήστης μπορεί να επιλέξει μονο weapons ή armors
        else if(type.compare("equip") == 0){
            std::cout << "Items categories:" << std::endl;
            std::cout << "1.Armor" << "\n" << "2.Weapons" << "\n" << "0.Exit" << std::endl;
            Input<int, 1>::get_choice(product_category);
            while(product_category < 0 || product_category > 2){
                std::cout << "Wrong input..." << std::endl;
                std::cout << "Try again..." << std::endl;
                Input<int, 1>::get_choice(product_category);
            }
        }
        //ενω αν έχουμε "use" ή "use+" τότε αυτόματα επιλέγονται τα potions
        else if(type.compare("use") == 0 || type.compare("use+") == 0){
            product_category = 3;
        }
        //ενω αν έχουμε "cast" τότε αυτόματα επιλέγονται τα spells
        else{
            product_category = 4;
        }
        while(product_category != 0){

            std::vector<Useables*> products;
            //αν έχουμε armors
            if(product_category == 1){

                std::string choice("n");
                do{
                    //παίρνουμε όλα τα armors από το inventory
                    products = find_in_inventory(product_category, 0);
                    print(products);

                    if(type.compare("equip") == 0 && !products.empty()){
                        inventory_edited = equip_item(products);
                    }

                    //οταν δεν γίνει κάποιο equip ή οταν αδειάσει το sub inventory(armors-weapons)
                    //ο χρηστης επιλέγει άμα θέλει να φύγει ή οχι
                    if(!inventory_edited || products.empty()){
                        std::cout <<"\n" << "Do you want to leave the list?(y/n)" << std::endl;
                        Input<std::string, 1>::get_choice(choice);
                        while(choice.compare("n") != 0 && choice.compare("y") != 0){
                            std::cout << "Wrong input..." << std::endl;
                            std::cout << "Try again..." << std::endl;
                            Input<std::string, 1>::get_choice(choice);
                        }
                    }
                }while(choice.compare("n") == 0);
            }
            //αν έχουμε weapons
            else if(product_category == 2){

                int weapon_category;
                do{
                    //o χρήστης επιλέγει ποια κατηγορία όπλων θέλει
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

                    while(weapon_category != 0){

                        std::string choice("n");
                        //οσο θέλει να βλέπει την λίστα των όπλων
                        do{
                            products = find_in_inventory(product_category, weapon_category);
                            print(products);

                            if(type.compare("equip") == 0 && !products.empty()){
                                inventory_edited = equip_item(products);
                            }

                            if(!inventory_edited || products.empty()){
                                std::cout <<"\n" << "Do you want to leave the list?(y/n)" << std::endl;
                                Input<std::string, 1>::get_choice(choice);
                                while(choice.compare("n") != 0 && choice.compare("y") != 0){
                                    std::cout << "Wrong input..." << std::endl;
                                    std::cout << "Try again..." << std::endl;
                                    Input<std::string, 1>::get_choice(choice);
                                }                                
                            }
                        }while(choice.compare("n") == 0);

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
                }while(weapon_category != 0);
            }
            //αν έχουμε potions
            else if(product_category == 3){

                int potion_category;
                do{
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

                    while(potion_category != 0){

                        std::string choice("n");
                        do{
                            products = find_in_inventory(product_category, potion_category);
                            print(products);
                            //Στο use+ σημαίνει ότι βρισκόμαστε σε μάχη και επειδή επιτρέπεται μόνο 1 potion
                            //να γίνει use από τον ήρωα μολις γίνει κάνουμε return
                            if(type.compare("use+") == 0 && !products.empty()){
                                inventory_edited = use(products);

                                if(inventory_edited){
                                    return inventory_edited;
                                }

                                else{
                                    std::cout <<"\n" << "Do you want to leave the list?(y/n)" << std::endl;
                                    Input<std::string, 1>::get_choice(choice);
                                    while(choice.compare("n") != 0 && choice.compare("y") != 0){
                                        std::cout << "Wrong input..." << std::endl;
                                        std::cout << "Try again..." << std::endl;
                                        Input<std::string, 1>::get_choice(choice);
                                    }                                      
                                }

                            }
                            //αλλιως σημαινει οτι βρισκόμαστε εκτος μάχης οπότε ο χρήστης επιλέγει το πότε
                            //θέλει να φύγει από τη λίστα
                            else{
                                if(type.compare("use") == 0 && !products.empty()){
                                    inventory_edited = use(products);
                                }
                                if(!inventory_edited || products.empty()){
                                    std::cout <<"\n" << "Do you want to leave the list?(y/n)" << std::endl;
                                    Input<std::string, 1>::get_choice(choice);
                                    while(choice.compare("n") != 0 && choice.compare("y") != 0){
                                        std::cout << "Wrong input..." << std::endl;
                                        std::cout << "Try again..." << std::endl;
                                        Input<std::string, 1>::get_choice(choice);
                                    }                                    
                                }
                            }
                        }while(choice.compare("n") == 0);

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
                }while(potion_category != 0);
            }
            //αν έχουμε spells
            else{

                int spell_category;
                do{

                    system("clear");
                    std::cout << "Spells categories:" << std::endl;
                    std::cout << "1.Fire Spell" << "\n" << "2.Ice Spell"<< "\n" << "3.Lighting Spell" << "\n" << "0.Exit" << std::endl;
                    std::cout << "\n" << "You can choose a category using the numbers 1-3 or 0 to exit" << std::endl;

                    Input<int, 1>::get_choice(spell_category);
                    while(spell_category < 0 || spell_category > 3){
                        std::cout << "Wrong input..." << std::endl;
                        std::cout << "Try again..." << std::endl;
                        Input<int, 1>::get_choice(spell_category);
                    }

                    while(spell_category != 0){

                        std::string choice("n");
                        do{
                            products = find_in_inventory(product_category, spell_category);
                            print(products);

                            //Στο cast σημαίνει ότι βρισκόμαστε σε μάχη και επειδή επιτρέπεται μόνο 1 spell cast
                            //να γίνει από τον ήρωα μολις γίνει κάνουμε return
                            if(type.compare("cast") == 0 && !products.empty()){
                                inventory_edited = equip_spell(products);

                                if(inventory_edited){
                                    return inventory_edited;
                                }

                            }

                            std::cout <<"\n" << "Do you want to leave the list?(y/n)" << std::endl;
                            Input<std::string, 1>::get_choice(choice);
                            while(choice.compare("n") != 0 && choice.compare("y") != 0){
                                std::cout << "Wrong input..." << std::endl;
                                std::cout << "Try again..." << std::endl;
                                Input<std::string, 1>::get_choice(choice);
                            }
                        }while(choice.compare("n") == 0);

                        system("clear");
                        std::cout << "Spells categories:" << std::endl;
                        std::cout << "1.Fire Spell" << "\n" << "2.Ice Spell"<< "\n" << "3.Lighting Spell" << "\n" << "0.Exit" << std::endl;
                        std::cout << "\n" << "You can choose a category using the numbers 1-3 or 0 to exit" << std::endl;

                        Input<int, 1>::get_choice(spell_category);
                        while(spell_category < 0 || spell_category > 3){
                            std::cout << "Wrong input..." << std::endl;
                            std::cout << "Try again..." << std::endl;
                            Input<int, 1>::get_choice(spell_category);
                        }
                    }
                }while(spell_category != 0);
            }

            system("clear");
            if(type.compare("view") == 0){
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

            else if(type.compare("equip") == 0){
                std::cout << "Items categories:" << std::endl;
                std::cout << "1.Armor" << "\n" << "2.Weapons" << "\n" << "0.Exit" << std::endl;
                Input<int, 1>::get_choice(product_category);
                while(product_category < 0 || product_category > 2){
                    std::cout << "Wrong input..." << std::endl;
                    std::cout << "Try again..." << std::endl;
                    Input<int, 1>::get_choice(product_category);
                }
            }

            else{
                product_category = 0;
            } 
   
        }
    }while(product_category != 0);
    return inventory_edited;
}


/* Οταν βλέπουμε main_category εννοούμε [1.armor 2.weapon 3.potion 4.spell]
Τα sub_category του weapon [1 -> one handed / 2 -> two handed]
Τα sub_category του potion [1 -> health potion / 2 -> magic potion / 3 -> strength potion]
                            [4 -> agility potion / 5 -> dexterity potion]
Τα sub_category του spell [1 -> fire spell / 2 -> ice spell / 3 -> lighting spell] */

//Επιστρέφει όλα τα αντικείμενα τύπου (main_category, sub_category) πχ (2, 1) -> weapons(one handed)
//που βρίσκονται στο inventory του ήρωα
std::vector<Useables*> Hero::find_in_inventory(int main_category, int sub_category){

    std::vector<Useables*> products;
    if(main_category == 1){
        for(uint i=0; i<inventory.size(); i++){
            if(inventory[i]->get_type().compare("armor") == 0)
                products.push_back(inventory[i]);
        }
        return products;
    }
    else if(main_category == 2){
        switch (sub_category){
            case 1:
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("one_handed") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
            default :
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("two_handed") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
        }      
    }
    else if(main_category == 3){
        switch (sub_category){
            case 1:
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("health_potion") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
            case 2 :
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("magic_potion") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
            case 3 :
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("strength_potion") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
            case 4 :
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("agility_potion") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
            default :
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("dexterity_potion") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
        }  
    }
    else{
        switch (sub_category){
            case 1:
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("fire_spell") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
            case 2 :
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("ice_spell") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
            default :
                for(uint i=0; i<inventory.size(); i++){
                    if(inventory[i]->get_type().compare("lighting_spell") == 0)
                        products.push_back(inventory[i]);
                }
                return products;
        }
    }
}

//True -> βρέθηκε αντικείμενο με id = useable_id στο inventory του ηρωα
//False -> δεν βρέθηκε
bool Hero::find_in_inventory(int useable_id){
    for(int i=0; i<(int)inventory.size(); i++){
        if(inventory[0]->get_id() == useable_id)
            return true;
    }
    return false;
}

//επιστρέφει τα αντικείμενα που έχουν γίνει equip από τον ήρωα τύπου (main cateogry, sub category)
//(1, 0) -> armors / (2,1) -> weapons(one handed) / (2,2)->weapons(two handed)
std::vector<Useables*> Hero::get_equiped_items(int main_category, int sub_category){
    std::vector<Useables*> products;
    if(main_category == 1){
        if(armor != NULL){
            products.push_back(armor);
        }  
    }
    else{
        for(int i=0; i<(int)weapons.size(); i++){
            if(sub_category == 1){
                if(weapons[i]->get_type().compare("one_handed") == 0){
                    products.push_back(weapons[i]);
                }
            }
            else{
                if(weapons[i]->get_type().compare("two_handed") == 0){
                    products.push_back(weapons[i]);
                }
            }
        }     
    }
    return products;
}

//αφαιρεί equiped item με το συγκεκριμένο id
void Hero::remove_equiped_item(int id){
    if(armor != NULL && armor->get_id() == id){
        armor = NULL;
        return;
    }
    else if(!weapons.empty()){
        for(int i=0; i<(int)weapons.size(); i++){
            if(weapons[i]->get_id() == id){
                weapons.erase(weapons.begin() + i);
                return;
            }
        }
    }
}

//αφαιρεί αντικείμενο από το inventory με το συγκεκριμένο id
void Hero::remove_from_inventory(int id){

    for(uint i=0; i<inventory.size(); i++){
        int product_id = inventory[i]->get_id();
        if(product_id == id){
            inventory.erase(inventory.begin() + ((int)i));
            return;
        }
    }

}

void Hero::add_on_inventory(Useables* product){
    inventory.push_back(product);
}

void Hero::info()const{

    std::cout << "Type: " << type << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Level: " << level << std::endl;
    std::cout << "Health power: " << health_power << std::endl;
    std::cout << "Magic power: " << magic_power << std::endl;
    std::cout << "Strength: " << strength << std::endl;
    std::cout << "Agility: " << agility << std::endl;
    std::cout << "Dexterity: " << dexterity << std::endl;
    std::cout << "Current money: " << money << std::endl;
    std::cout << "Experience: " << experience << std::endl; 
    if(fainted)
        std::cout << "Status: fainted" << std::endl;
    else
        std::cout << "Status: alive" << std::endl;
}

// Warrior functions ///
Warrior::Warrior(std::string name):Hero(name, "warrior"){
    strength += strenght_boost;
    agility += agility_boost;
}

void Warrior::level_up(int levels){

    if(level >= max_level)
        return;

    for(int i=0; i<levels; i++){
        strenght_boost += strenght_boost * stats_modifier;
        agility_boost += agility_boost * stats_modifier;

        strength += strength * stats_modifier + strenght_boost;
        agility +=  agility * stats_modifier + agility_boost;
        dexterity += dexterity * stats_modifier;
        max_health_power += max_health_power * stats_modifier;
        max_magic_power += max_magic_power * stats_modifier;

        health_power = max_health_power;
        magic_power = max_magic_power;

        level++;
        if(level >= max_level)
            return;
    }
}

void Warrior::info()const{
    std::cout << "Info for the Warrior "<< std::endl;
    Hero::info();
}

// Sorcerer functions ///
Sorcerer::Sorcerer(std::string name):Hero(name, "sorcerer"){
    agility += agility_boost;
    dexterity += dexterity_boost;
}

void Sorcerer::level_up(int levels){

    if(level >= max_level)
        return;

    for(int i=0; i<levels; i++){
        dexterity_boost += dexterity_boost * stats_modifier;
        agility_boost += agility_boost * stats_modifier;

        dexterity += dexterity * stats_modifier + dexterity_boost;
        agility +=  agility * stats_modifier + agility_boost;
        strength += strength * stats_modifier;
        max_health_power += max_health_power * stats_modifier;
        max_magic_power += max_magic_power * stats_modifier;

        level++;
    }
}

void Sorcerer::info()const{
    std::cout << "Info for the Sorcerer "<< std::endl;
    Hero::info();
}

// Paladin functions ///
Paladin::Paladin(std::string name):Hero(name, "paladin"){
    dexterity += dexterity_boost;
    strength += strenght_boost;
}

void Paladin::level_up(int levels){
    if(level >= max_level)
        return;
    
    for(int i=0; i<levels; i++){
        dexterity_boost += dexterity_boost * stats_modifier;
        strength += strenght_boost * stats_modifier;

        dexterity += dexterity * stats_modifier + dexterity_boost;
        strength +=  strength * stats_modifier + strenght_boost;
        agility += agility * stats_modifier;
        max_health_power += max_health_power * stats_modifier;
        max_magic_power += max_magic_power * stats_modifier;

        level++;
    }
}

void Paladin::info()const{
    std::cout << "Info for the Paladin "<< std::endl;
    Hero::info();
}

//Monster functions ///
Monster::Monster(std::string name,std::string type):Living(name, "monster"), type(type){}

Monster::~Monster(){}

int Monster::get_defense()const{ return (defense - defense_reduced); }

std::string Monster::get_type()const { return type; }

float Monster::get_agility()const{ return (agility - agility_reduced) ; }

void Monster::info()const{
    std::cout << get_name()<< " Hp:" << get_health_power() << " Damage:" << get_min_damage() 
    << "-" << get_max_damage() << " Agility:" << get_agility() << " Defense:" << get_defense() 
    << " Type:" << get_type() << " Level:" << get_level() << "\n" << std::endl;
}

//εφαρμόζει στο τέρας τα effects που προκλήθηκαν μετά από το cast spell
//για όσους γύρους αυτό όριζε
void Monster::apply_effects(){

    if(agility_reduced_turns <= 0){
        agility_reduced = 0;
    }
    else{
        std::cout << "Monster is hurt by the spells effect" << std::endl;
        std::cout << "Agility reduction: " << agility_reduced << std::endl;
        std::cout << "Remaining turns: " << agility_reduced_turns << "\n" << std::endl;
        agility_reduced_turns--;
    }

    if(damage_reduced_turns <= 0){
        damage_reduced = 0;
    }
    else{
        std::cout << "Monster is hurt by the spells effect" << std::endl;
        std::cout << "Damage reduction: " << damage_reduced << std::endl;
        std::cout << "Remaining turns: " << damage_reduced_turns << "\n" << std::endl;
        damage_reduced_turns--;
    }

    if(defense_reduced_turs <= 0){
        defense_reduced = 0;
    }
    else{
        std::cout << "Monster is hurt by the spells effect" << std::endl;
        std::cout << "Defense reduction: " << defense_reduced << std::endl;
        std::cout << "Remaining turns: " << defense_reduced_turs << "\n" << std::endl;   
        defense_reduced_turs--;     
    }
}

int Monster::get_max_damage()const{ return (range_of_damage[1] - damage_reduced); }

int Monster::get_min_damage()const{ return (range_of_damage[0] - damage_reduced); }

void Monster::set_agility_reduction(float reduction, int turns){
    //προσέχω το reduction μην ρίξει τα stats κάτω του 0
    if(reduction > agility)
        agility_reduced = agility;
    else
        agility_reduced = reduction;

    agility_reduced_turns = turns;
}

void Monster::set_damage_reduction(int reduction, int turns){
    if(reduction > range_of_damage[0])
        damage_reduced  = range_of_damage[0];
    else
        damage_reduced = reduction;

    if(reduction > range_of_damage[1])
        damage_reduced  = range_of_damage[1];
    else
        damage_reduced = reduction;
    
    damage_reduced_turns = turns;
}

void Monster::set_defense_reduction(int reduction, int turns){
    if(reduction > defense)
        defense_reduced = defense;
    else
        defense_reduced = reduction;

    defense_reduced_turs = turns;
}

//Γίνεται επίθεση στον πρώτο διαθέσιμο ήρωα από το τέρας
void Monster::attack(std::vector<Hero*> group_of_heroes){

    for(int i=0; i<(int)group_of_heroes.size(); i++){
        //αν το τέρας είναι ζωντανό
        if(get_status()){
            continue;
        }
        //και άμα ο ήρωας είναι ζωντανός
        if(!group_of_heroes[i]->get_status()){

            float agility = group_of_heroes[i]->get_agility();
            int evade = agility * 100;

            //ελέγχουμε αν ο ήρωες έκανε evade την επίθεση του τέρατος
            if( evade >= (rand() % 100) + 1 ){
                std::cout << "Monster's attack has been evaded" << "\n" << std::endl;
            }
            //αλλιως κανονικά γίνεται η επίθεση στον ήρωα
            else{
                int damage = rand() % (get_max_damage() + 1 - get_min_damage()) + get_min_damage();
                damage -= group_of_heroes[i]->get_defense();

                group_of_heroes[i]->update_health_power(- damage);

                std::cout << "Monster:" << get_name() << " attacks hero" << std::endl;
                std::cout << "Damage dealt:" << damage  << " to:" << group_of_heroes[i]->get_name() << std::endl;
                std::cout << group_of_heroes[i]->get_name() << "'s current health:" << group_of_heroes[i]->get_health_power()
                << "\n" << std::endl;         
            }
            return;
        }
    }
}

//Επαναφέρει ενα μικρό ποσοστό του συνολικού health power
//όταν το τέρας δεν έχει πεθάνει
void Monster::restore(){

    int health_increase = max_health_power/10;
    if(health_power > 0){
        if((health_power + health_increase) < max_health_power){
            health_power += health_increase;
        }
        else{
            health_increase = max_health_power - health_power;
            health_power = max_health_power;
        }
        std::cout << "Monster's name: " << name << std::endl;
        std::cout << "Health power restored: " << health_increase
        << " current health power: " << health_power << "\n" << std::endl;
    }
}

// Dragon functios ///
Dragon::Dragon(std::string name, int level):Monster(name, "dragon"){
    range_of_damage[0] += damage_boost;
    range_of_damage[1] += damage_boost;
    for(int i=1; i<level; i++){
        damage_boost += damage_boost*stats_modifier;

        range_of_damage[0] += range_of_damage[0] * stats_modifier + damage_boost;
        range_of_damage[1] += range_of_damage[1] * stats_modifier + damage_boost;
        defense += defense * stats_modifier;
        agility += agility * stats_modifier;
        max_health_power += max_health_power * stats_modifier;
        health_power = max_health_power;

        this->level++;
    }
}

void Dragon::info()const{
    std::cout << "Dragon information: "<< std::endl;
    Monster::info();  
}

// Exosceleton functios ///
Exosceleton::Exosceleton(std::string name,int level):Monster(name, "exosceleton"){
    defense += defense_boost;
    for(int i=1; i<level; i++){
        defense_boost += defense_boost * stats_modifier;

        range_of_damage[0] += range_of_damage[0] * stats_modifier;
        range_of_damage[1] += range_of_damage[1] * stats_modifier;
        defense += defense * stats_modifier + defense_boost;
        agility += agility * stats_modifier;
        max_health_power += max_health_power * stats_modifier;
        health_power = max_health_power;

        this->level++;
    }
        
}

void Exosceleton::info()const{
    std::cout << "Exosceleton information: "<< std::endl;
    Monster::info();  
}

// Spirit functios ///
Spirit::Spirit(std::string name,int level):Monster(name, "spirit"){
    agility += agility_boost;
    for(int i=1; i<level; i++){
        agility_boost += agility_boost * stats_modifier;

        range_of_damage[0] += range_of_damage[0] * stats_modifier;
        range_of_damage[1] += range_of_damage[1] * stats_modifier;
        defense += defense * stats_modifier;
        agility += agility * stats_modifier + agility_boost;
        max_health_power += max_health_power * stats_modifier;
        health_power = max_health_power;

        this->level++;
    }
}

void Spirit::info()const{
    std::cout << "Spirit information: "<< std::endl;
    Monster::info();  
}