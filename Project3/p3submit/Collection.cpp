#include "Collection.h"
#include "Record.h"
#include "Utility.h"
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <iterator>

using std::placeholders::_1;
using std::string;
using std::set;
using std::map;
using std::make_pair;
using std::for_each;
using std::ostream_iterator;

Collection::Collection(std::ifstream& is, const set<Record*, comp_record_ptr_title>& library)
{
    int number;
    string temp_string;
    is >> name >> number;
    if (!is)
        throw Error("Invalid data found in file!");
    is.get();   
    for (int i = 0; i < number; i++) {     
        getline(is, temp_string);
        if (temp_string.size() == 0)
            throw Error("Invalid data found in file!");
        Record temp_record(temp_string);
        auto it = library.find(&temp_record);

        if (it == library.end())
            throw Error("Invalid data found in file!");
        members.insert(*it); 
    }
}

Collection::Collection(const string& name_, 
            const Collection& collection1, const Collection& collection2)
            :name(name_)
{
    members.insert(collection1.members.begin(), collection1.members.end());
    members.insert(collection2.members.begin(), collection2.members.end());
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
    auto it = members.find(record_ptr);
    if (it == members.end())
        return false;
    else
        return true;
}

void Collection::remove_member(Record* record_ptr)
{
    auto it = members.find(record_ptr);
    if (it == members.end())
        throw Error("Record is not a member in the collection!");
    else
        members.erase(it);
}

void Collection::save(std::ostream& os) const
{
    os << name << ' ' << members.size() << std::endl;
    for_each(members.begin(), members.end(), 
                [&os](Record* record) {os << record->get_title() << std::endl;});
}

void Collection::get_record_list(map<Record*, int>& statistic_map, int& track)
{
    for_each(members.begin(), members.end(), 
                [&statistic_map, &track](Record* record){
                    auto map_it = statistic_map.find(record);
                    if (map_it == statistic_map.end())
                        statistic_map.insert(make_pair(record, 1));
                    else {
                        map_it->second++;
                        if (map_it->second == 2)
                            track++;
                    }
                });
}

std::ostream& operator<< (std::ostream& os, const Collection& collection)
{
    os << "Collection " << collection.get_name() << " contains:";
    if (collection.empty())
        os << " None" << std::endl;
    else {
        os << std::endl; 
        ostream_iterator<Record*> out_it(os);
        copy(collection.members.begin(), collection.members.end(), out_it);
    }
    return os;
}
