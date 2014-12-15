#include "Collection.h"
#include "String.h"
#include "Record.h"
#include "Utility.h"
#include <iostream>
#include <fstream>

Collection::Collection(std::ifstream& is, const Ordered_list<Record*, Less_than_ptr<Record*> >& library)
{
    int number;
    String temp_string;
    is >> name >> number;
    if (!is)
        throw Error("Invalid data found in file!");
    for (int i = 0; i < number; i++) {     
        is.get();   
        getline(is, temp_string);
        if (temp_string.size() == 0)
            throw Error("Invalid data found in file!");
        Record temp_record(temp_string);
        Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it = 
            library.find(&temp_record);
        if (it == library.end())
            throw Error("Invalid data found in file!");
        members.insert(*it);    
    }
}

void Collection::add_member(Record* record_ptr)
{
    if (!is_member_present(record_ptr))
        members.insert(record_ptr);
    else 
        throw Error("Record is already a member in the collection!");
}

bool Collection::is_member_present(Record* record_ptr) const
{
    Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it =
        members.find(record_ptr);
    if (it == members.end())
        return false;
    else
        return true;
}

void Collection::remove_member(Record* record_ptr)
{
    Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it = 
        members.find(record_ptr);
    if (it == members.end())
        throw Error("Record is not a member in the collection!");
    else
        members.erase(it);
}

void Collection::save(std::ostream& os) const
{
    os << name << ' ' << members.size() << std::endl;
    Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it = 
        members.begin();
    while (it != members.end()) {
        os << (*it)->get_title() << std::endl; 
        ++it;    
    }
}

std::ostream& operator<< (std::ostream& os, const Collection& collection)
{
    os << "Collection " << collection.get_name() << " contains:";
    if (collection.empty())
        os << " None" << std::endl;
    else {
        os << std::endl; 
        Ordered_list<Record*, Less_than_ptr<Record*> >::Iterator it =
            collection.members.begin();
        while (it != collection.members.end()) {
            os << *(*it);
            ++it;
        }
    }
    return os;
}
