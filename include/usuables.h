#pragma once
#include <vector>
#include <iostream>

//Η κύρια κλάση από την οποία κληρωνομούν όλες οι κλάσεις 
//οι οποίες δημιουργούν αντικείμενα τα οποία μπορούν να χρησιμοποιηθούν από τον ήρωα
//δηλαδή οι (items, spells), ουσιαστικά ότι βρίσκεται στο shop και στο inventory

class Useables{
    protected:
        int id = 0;                 //μοναδικό id για κάθε αντικείμενο useable
        static int useables_ids;    //μετρητής όλων των useables, το οποίο δίνει και το μοναδικό id
        int price = 0;
        std::string name;
        std::string type;           //"item" / "spell"
    
        Useables(std::string name, std::string type);

    public:
        virtual ~Useables() = 0;
        virtual void info()const = 0;
        virtual int get_required_level()const = 0;
        virtual std::string get_type()const;
        std::string get_name()const;
        int get_price()const;
        int get_id()const;
};