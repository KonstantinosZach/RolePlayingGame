#include "usuables.h"

//Useables functions
Useables::Useables(std::string name, std::string type):name(name), type(type){
    id = useables_ids;
    useables_ids++;
}

Useables::~Useables(){}

std::string Useables::get_name()const{ return name; }

std::string Useables::get_type()const { return type; }

int Useables::get_price()const{ return price; }

int Useables::get_id()const{ return id; }

int Useables::useables_ids = 0;