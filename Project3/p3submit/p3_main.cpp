#include "Collection.h"
#include "Record.h"
#include "Utility.h"
#include <iostream>
#include <cctype>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <functional>
#include <iterator>
#include <new>

using std::cout; using std::cin; using std::endl;
using std::ifstream; using std::ofstream; using std::ostream_iterator;

using std::set; using std::map; using std::list; using std::vector;
using std::string;

using std::mem_fn; using std::greater; using std::ref; using std::bind;

using std::for_each; using std::find_if_not; using std::transform; using std::replace_if;
using std::find_if; using std::lower_bound; using std::unique; using std::copy;

using std::bad_alloc;

using std::placeholders::_1;

typedef set<Record*, comp_record_ptr_title> title_ordered_t;
typedef vector<Record*> id_ordered_t;
typedef vector<Collection> name_ordered_t;

struct database {
    title_ordered_t title_library;
    id_ordered_t id_library;
    name_ordered_t catalog;
};

typedef void (*command_function)(database&);
typedef map<string, command_function> command_map_t;
void load_command_map(command_map_t& cm);

bool comp_record_ptr_id(const Record* record1, const Record* record2)
{return record1->get_ID() < record2->get_ID();}

/*Helper functions*/
void read_to_newline();
string read_title();
int read_number();
void unrecognized_command();
title_ordered_t::iterator find_record_title_helper(title_ordered_t &title_library,
                                                    const string &title);
id_ordered_t::iterator find_record_id_helper(id_ordered_t &id_library,
                                            int id);
name_ordered_t::iterator find_collection_helper(name_ordered_t &catalog,
                                                const string &collection_name);
void record_already_exist(const title_ordered_t &title_library, 
                            const string &title);
void add_record_helper(database& data_library, Record* record_ptr);
void add_collection_helper(name_ordered_t &catalog, Collection collection);
void clear_helper(database &data_library);
void delete_record(Record* record);

/*Command functions*/
void print_record_title(database& data_library);
void print_record_id(database& data_library);
void print_collection(database& data_library);
void print_library(database& data_library);
void print_catalog(database& data_library);
void print_allocation(database& data_library);
void add_record(database& data_library);
void add_collection(database& data_library);
void add_record_to_collection(database& data_library);
void modify_record_rating(database& data_library);
void delete_record_from_library(database& data_library);
void delete_collection_from_catalog(database& data_library);
void delete_record_from_collection(database& data_library);
void delete_record(Record* record);
void clear_library (database& data_library);
void clear_catalog(database& data_library);
void clear_all(database& data_library);
void save_all(database& data_library);
void restore_all(database& data_library);
void quit(database& data_library);
void find_record_with_string(database& data_library);
void list_rating(database& data_library);
void collection_statistic(database& data_library);
void combine_collection(database& data_library);
void modify_title(database& data_library);


int main() {
    database data_library;
    command_map_t command_map;
    load_command_map(command_map);    
    
    while(true) {
        cout << "\nEnter command: ";
        char action;
        char object;
        cin >> action >> object;
        string command;
        command.push_back(action);
        command.push_back(object);
        
        if (command == "qq") {
            quit(data_library);
            return 0;
        }
        auto command_it = command_map.find(command);
        if (command_it == command_map.end()) {
            unrecognized_command();
            continue;
        }
        try {
            (command_it->second)(data_library);
        } catch (Error &exception) {
            cin.clear();
            read_to_newline();
            cout << exception.msg << endl;
        } catch (Error_title &exception) {
            cout << exception.msg << endl;
            
        } catch (bad_alloc &ba) {
            cout << "Bad allocation!" << endl;
            return 0;
        } catch (...) {
            cout << "Unknown exception caught!" << endl;
            return 0;
        }
    } 
}
/*This function loads all the commands into command_map_t object command_map*/
void load_command_map(command_map_t& cm)
{
    cm["fr"] = &print_record_title;
    cm["pr"] = &print_record_id;
    cm["pc"] = &print_collection;
    cm["pL"] = &print_library;
    cm["pC"] = &print_catalog;
    cm["pa"] = &print_allocation;
    cm["ar"] = &add_record;
    cm["ac"] = &add_collection;
    cm["am"] = &add_record_to_collection;
    cm["mr"] = &modify_record_rating;
    cm["dr"] = &delete_record_from_library;
    cm["dc"] = &delete_collection_from_catalog;
    cm["dm"] = &delete_record_from_collection;
    cm["cL"] = &clear_library;
    cm["cC"] = &clear_catalog;
    cm["cA"] = &clear_all;
    cm["sA"] = &save_all;
    cm["rA"] = &restore_all;
    cm["fs"] = &find_record_with_string; 
    cm["lr"] = &list_rating;
    cm["cs"] = &collection_statistic;
    cm["cc"] = &combine_collection;
    cm["mt"] = &modify_title;
}

/*This function read all the characters from the stream until hitting a newline character*/
void read_to_newline()
{
    while(cin.get() != '\n');
}
/*This function first reads the entire line to the result and then eliminates all the
heading and tailing whitespaces. If the title is not available, error will be thrown.*/
string read_title()
{
    string buff;
    getline(cin, buff);
  
    /*Replace all the whitespaces to ' '.*/ 
    replace_if(buff.begin(), buff.end(), (int(*)(int))isspace, ' ');

    /*Shrink the continuous ' ' into a single one and erase erase the duplicate ' '.*/
    buff.erase(unique(buff.begin(), buff.end(), 
        [](char lhs, char rhs) {return (lhs == rhs) && (lhs == ' ');}), 
            buff.end());
    
    /*Erase the leading space if any.*/
    if (buff[0] == ' ')
        buff.erase(0, 1);
    /*Erase the tailing space if any.*/
    int last = buff.size() - 1;
    if (buff[last] == ' ')
        buff.erase(last, 1);
    
    /*If the title is not available, throw the exception*/
    if (buff.size() <= 0)
        throw Error_title("Could not read a title!");
 
    return buff;
}
/*This function reads an integer, if the it cannot read an integer, corresponding error will
be thrown.*/
int read_number()
{
    int number;
    if (!(cin >> number))
        throw Error("Could not read an integer value!");
    return number;
}

/*This function is called when there are bad commands. It first reads all the remained
characters of the current line and print error message*/
void unrecognized_command()
{
    read_to_newline();
    cout << "Unrecognized command!" << endl;
}

/*This function tries to find an iterator of the record from title_library. If the title
cannot be found, corresponding error will be thrown.*/
title_ordered_t::iterator find_record_title_helper(
                set<Record*, comp_record_ptr_title> &title_library,
                const string &title)
{
    Record temp_record(title);
    auto it = title_library.find(&temp_record);
    if (it == title_library.end())
        throw Error_title("No record with that title!");
    return it;
}
/*This function tries to find an iterator of the record from id_library. If the id cannot
be found, corresponding error will be thrown.*/
id_ordered_t::iterator find_record_id_helper(vector<Record*> &id_library,
                int id)
{
    Record temp_record(id);
    auto it = lower_bound(id_library.begin(), id_library.end(), &temp_record,
                comp_record_ptr_id);
    if (it == id_library.end() || (*it)->get_ID() != temp_record.get_ID())
        throw Error("No record with that ID!");
    return it;
}
/*This function tries to find an iterator of collection from catalog. If the name cannot
be found, corresponding error will be thrown.*/
name_ordered_t::iterator find_collection_helper(vector<Collection> &catalog,
                                    const string &collection_name)
{
    Collection temp_collection(collection_name);
    auto it = lower_bound(catalog.begin(), catalog.end(), temp_collection);
    if (it == catalog.end() || it->get_name() != temp_collection.get_name())
        throw Error("No collection with that name!");
    return it;
}


/*This function first reads the input searching title and then searches it from title_library.*/
void print_record_title(database& data_library)
{
    string search_title = read_title();
    
    auto it = find_record_title_helper(data_library.title_library, search_title);
    cout << *(*it);
    
}
/*This function first reads the searching number and then searches it from id_library. 
If it cannot read an iteger id or cannot find the id from the library, it will throw 
corresponding exception*/
void print_record_id(database& data_library)
{
    int number = read_number();
    auto it = find_record_id_helper(data_library.id_library, number); 
    cout << *(*it);
}

/*This function reads the searching name and then searches it from catalog. If 
it cannot find the name from catalog, it will throw and exception*/
void print_collection(database& data_library)
{
    string name;
    cin >> name;
    auto it = find_collection_helper(data_library.catalog, name);
    cout << *it;
}
/*This function prints all the records that are contained in the library*/
void print_library(database& data_library)
{
    if (data_library.title_library.empty())
        cout << "Library is empty" << endl;
    else {
        cout << "Library contains " << data_library.title_library.size() << " records:" << endl;
        ostream_iterator<Record*> out_it(cout);
        copy(data_library.title_library.begin(), data_library.title_library.end(), out_it);
    }
}
/*This function prints all the collections that are contained in the catalog*/
void print_catalog(database& data_library)
{
    if (data_library.catalog.empty())
        cout << "Catalog is empty" << endl;
    else {
        cout << "Catalog contains " << data_library.catalog.size() << " collections:" << endl;
        ostream_iterator<Collection> out_it(cout);
        copy(data_library.catalog.begin(), data_library.catalog.end(), out_it);
    }
} 
/*This function prints all the related information about record number, collection number 
in the containers*/
void print_allocation(database& data_library)
{
    cout << "Memory allocations:" << endl;
    cout << "Records: " << data_library.title_library.size() << endl;
    cout << "Collections: " << data_library.catalog.size() << endl;
}

/*This function checks if the title is already existed. If it is, error is thrown since
a new record cannot be inserted or the new record name is not valid.*/
void record_already_exist(const set<Record*, comp_record_ptr_title>& title_library, 
                            const string &title)
{
    Record temp_record(title);
    auto title_it = title_library.find(&temp_record);
    if (title_it != title_library.end())
        throw Error_title("Library already has a record with this title!");
}
/*This function add a record_ptr to title_library and id_library*/
void add_record_helper(database& data_library, Record* record_ptr)
{
    data_library.title_library.insert(record_ptr);
    auto id_it = lower_bound(data_library.id_library.begin(), data_library.id_library.end(),
                                record_ptr, comp_record_ptr_id);
    data_library.id_library.insert(id_it, record_ptr);
}
/*This function creates a new record with the title and medium obtained from input 
and adds it into title_library and id_library.*/
void add_record(database& data_library)
{
    string medium;
    cin >> medium;
    string title = read_title();
    record_already_exist(data_library.title_library, title);
    Record* record_ptr = new Record(medium, title);
    add_record_helper(data_library, record_ptr);
    cout << "Record " << record_ptr->get_ID() << " added" << endl;
}
/*This function adds a collection into the catalog, if the collection name has already
existed, an error is thrown.*/
void add_collection_helper(vector<Collection>& catalog, Collection collection)
{
    auto it = lower_bound(catalog.begin(), catalog.end(), collection);
    if (it == catalog.end() || it->get_name() != collection.get_name()){
        catalog.insert(it, collection);
    } else
        throw Error("Catalog already has a collection with this name!");
}

/*This function constructs a new collection with the name obtained from input and 
adds it into the catalog. If the collection with that name has been added, it will 
throw an exception. */
void add_collection(database& data_library)
{
    string name;
    cin >> name;
    Collection temp_collection(name);
    
    add_collection_helper(data_library.catalog, temp_collection);   
    cout << "Collection " << temp_collection.get_name() << " added" << endl;
}
/*This function reads the collection name and the reocord id and add the record 
with that id into the collection. If there is no collection with that name or 
it cannot read an number as id or there is no record with that id, it will throw 
the corresponding exception.*/
void add_record_to_collection(database &data_library)
{
    string name;
    cin >> name;
    auto collection_it = find_collection_helper(data_library.catalog, name);
    int number = read_number();
    auto record_it = find_record_id_helper(data_library.id_library, number);
    (*collection_it).add_member(*record_it);
    cout << "Member " << (*record_it)->get_ID() << " " << (*record_it)->get_title() 
         << " added" << endl;
}
/*This function first read an iteger as the searchingid and another integer as 
the new rating. If it cannot read the integer or there is no record with the 
id number obtained from stream, it will throw the corresponding exception.*/
void modify_record_rating(database& data_library)
{
    int number = read_number();
    auto it = find_record_id_helper(data_library.id_library, number);
    number = read_number();        
    (*it)->set_rating(number);
    cout << "Rating for record " << (*it)->get_ID() << " changed to " << number << endl;
}
/*This function reads the record title that is going to be deleted and try to 
find if it is in a collection. If not, it delete the record and the node in 
title_library and id_library. If the title is not valid or there is no record 
with that title or the record is in a collection, it will throw the 
corresponding exception.*/
void delete_record_from_library(database& data_library)
{
    string search_title = read_title();
    auto title_it = find_record_title_helper(data_library.title_library, search_title);
    Record* record_ptr = *title_it;
    if (find_if(data_library.catalog.begin(), data_library.catalog.end(), 
            bind(&Collection::is_member_present, _1, record_ptr)) != data_library.catalog.end())
        throw Error_title("Cannot delete a record that is a member of a collection!");
    cout << "Record " << record_ptr->get_ID() << " " 
            << record_ptr->get_title() << " deleted" << endl;
    data_library.title_library.erase(title_it);
    auto id_it = find_record_id_helper(data_library.id_library, record_ptr->get_ID());
    data_library.id_library.erase(id_it);
    delete record_ptr;
}
/*This function reads the name taht is going to be deleted and delete the collection 
from the catalog. if there is no collection with that name, it will throw an exception.*/
void delete_collection_from_catalog(database& data_library)
{
    string name;
    cin >> name;

    auto collection_it = find_collection_helper(data_library.catalog, name);
    cout << "Collection " << (*collection_it).get_name() << " deleted" << endl;
    (*collection_it).clear();
    data_library.catalog.erase(collection_it);
}
/*This function first reads collection name and the record id and delete the record with 
that id from the collection. If there is no collection with that name or it could not 
read an integer for record id or there is no record with that id number or the record 
with that id is not in the collection, it will throw the corresponding error.*/
void delete_record_from_collection(database& data_library)
{
    string name;
    cin >> name;
    
    auto collection_it = find_collection_helper(data_library.catalog, name);
    int number = read_number();
    auto record_it = find_record_id_helper(data_library.id_library, number);
    (*collection_it).remove_member(*record_it);
    cout << "Member " << (*record_it)->get_ID() << ' '
            << (*record_it)->get_title() << " deleted" << endl;
}

/*Delete the resorce kept by Record* record*/
void delete_record(Record* record)
{
    delete record;
}

/*This function delete all the dynamically allocated records and clear title_library and
id_library*/
void clear_helper(database& data_library)
{
    for_each(data_library.title_library.begin(), data_library.title_library.end(),
                delete_record);
 
    data_library.title_library.clear();
    data_library.id_library.clear();
    Record::reset_ID_counter();
}

/*This function takes catalog, title_library and id_library as arguments. It clears all
the records in the library and clear libraries. If thereis one record that is in a collection, 
it will throw the exception.*/
void clear_library (database& data_library) 
{
    if (find_if_not(data_library.catalog.begin(), data_library.catalog.end(),
            mem_fn(&Collection::empty)) != data_library.catalog.end())
        throw Error("Cannot clear all records unless all collections are empty!");
    clear_helper(data_library);
    cout << "All records deleted" << endl;  
}
/*This function takes the catalog as the argument. It clear all the collections in the 
catalog.*/
void clear_catalog(database& data_library)
{
    data_library.catalog.clear();
    cout << "All collections deleted" << endl;
}
/*This function takes catalog, title_library and id_library as arguments. It first clears
all the collections and the catalog. Then it clear title_library and id library.*/
void clear_all(database& data_library)
{
    data_library.catalog.clear();
    clear_helper(data_library);
    cout << "All data deleted" << endl;
}
/*This function takes catalog, title_library and id_library as arguments. It saves all the 
records and collection in format into a file. If the file cannot be opened, it will throw
the exception.*/
void save_all(database& data_library)
{
    string file_name;
    cin >> file_name;
    ofstream os;
    os.open(file_name.c_str());
    if (!os.is_open())
        throw Error("Could not open file!");

    os << data_library.title_library.size() << endl;
    for_each(data_library.title_library.begin(), data_library.title_library.end(),
                bind(&Record::save, _1, ref(os)));    
    
    os << data_library.catalog.size() << endl;
    for_each(data_library.catalog.begin(), data_library.catalog.end(),
                bind(&Collection::save, _1, ref(os))); 
    os.close();
    cout << "Data saved" << endl;
}
/*This function takes catalog, title_library and id_library as arguments. It tries to restore
all the data from file into the catalog an libraries. If the data in the file is not valid and
get checked during restoring process, the original data would be obtained back. If the file 
cannot be opened or there is invalid data in the file, it will throw the corresponding 
exception.*/
void restore_all(database& data_library)
{
    string file_name;
    cin >> file_name;
    ifstream is;
    is.open(file_name.c_str());
    if (!is.is_open())
        throw Error("Could not open file!");
   
    database data_library_backup = data_library; 
    
    data_library.title_library.clear();
    data_library.id_library.clear();
    data_library.catalog.clear();
    
    try {
        Record::save_ID_counter();
        Record::reset_ID_counter();
  
        int number;
        if (!(is >> number))
            throw Error("Invalid data found in file!");
   
        for (int i = 0; i < number; ++i) {
            Record* new_record = new Record(is);
            add_record_helper(data_library, new_record);
        }
        if (!(is >> number))
            throw Error("Invalid data found in file!");

        for (int i = 0; i < number; ++i) {
            Collection new_collection(is, data_library.title_library);
            data_library.catalog.insert(lower_bound(data_library.catalog.begin(),
                        data_library.catalog.end(), new_collection), new_collection);
        }
        
    } catch (Error &exception) {
  
        data_library.catalog.clear();
        clear_helper(data_library);
        data_library = data_library_backup;
        
        Record::restore_ID_counter();
        
        is.close();
        throw exception;
    }
    is.close();
    for_each(data_library_backup.title_library.begin(),
                data_library_backup.title_library.end(),
                delete_record);
    cout << "Data loaded" << endl;
}
/*This function takes catalog, title_library and id_library as arguments. It
clears all the data and terminates the program*/
void quit(database& data_library)
{
    clear_all(data_library);
    cout << "Done" << endl;
}
/*This function class is to track if there is a record contains the keyword. 
Since the comparison is case-insensitive, the keyword passed to the constructor
should be lower case.*/
class Record_contains_string{
private:
    // The bool value that keep track of the existence.
    bool is_found;
    // The keyword for comparison.
    string keyword;
public:
    // The constructor with a string keyword as a parameter.
    Record_contains_string(string keyword_):is_found(false), keyword(keyword_){}
    // Operator() for algorithm to use. It first define a local string same
    // as title and then change it to lower case. If the keyword can be found,
    // print it and change the status of is_found to true.
    void operator()(Record* record){
        string lower_name = record->get_title();
        transform(lower_name.cbegin(), lower_name.cend(), 
                    lower_name.begin(),
                    (int(*)(int))tolower);
        if(lower_name.find(keyword) != string::npos) {
            is_found = true;
            cout << *record;
        }
    }
    // Getter of is_found.
    bool get_is_found(){return is_found;}
};

/*This function finds all records which contain the specific string inside*/
void find_record_with_string(database& data_library)
{
    string keyword;
    cin >> keyword;

    transform(keyword.cbegin(), keyword.cend(),
                keyword.begin(),
                (int(*)(int))tolower);    

    Record_contains_string obj =  
        for_each(data_library.title_library.begin(), data_library.title_library.end(),
                    Record_contains_string(keyword));
    if (!obj.get_is_found())
        throw Error("No records contain that string!");    
}
/*This function lists all the records by decreasing order os rating. */
void list_rating(database& data_library)
{
    int record_num = data_library.title_library.size();
    if (!record_num)
        cout << "Library is empty" << endl;
    else {
        vector<list<Record*> > rating_container(rating_max+1);
        /*For each records in title_library, we store it into a temprary vector of lists.
        The 0 list stores records with rating 5 and the 5 list stores records withrating 0*/
        for_each(data_library.title_library.begin(), data_library.title_library.end(),
            [&rating_container](Record* record){
                rating_container[rating_max-record->get_rating()].push_back(record);
            });
        for(list<Record*> &current_list : rating_container) {
            ostream_iterator<Record*> out_it(cout);
            copy(current_list.begin(), current_list.end(), out_it);
        }
    }
}

/*This function class is to keep track of the records in each collection*/
class Statistic {
private:
    /*The map that takes record pointer as the key and the occurrance as value*/
    map<Record*, int> statistic_map;
    /*The total number of records in collections*/
    int total_collection_record;
    /*The integer that keeps track of the the reacords that appears 
    more than one time in catalog.*/
    int record_occurs_more_than_once;
public:
    /*The constructor*/
    Statistic():total_collection_record(0), record_occurs_more_than_once(0){}
    /*Operator() takes collection as argument. total_collection_record will increment by the
    collection size and the statitic_map and more_than_one would be updated*/
    void operator()(Collection collection) {
        total_collection_record += collection.size();
        collection.get_record_list(statistic_map, record_occurs_more_than_once);
    }
    /*Getter of total_collection_record*/
    int get_total_num(){return total_collection_record;}
    /*Getter of the number of records occurred at least once*/
    int get_at_least_one(){return statistic_map.size();}
    /*Getter of the number of records occurred more than once*/
    int get_more_than_one(){return record_occurs_more_than_once;}

};

/*This function prints out the statistic data of the library. It first prints out the number
of records that appears at least once in the collections. Then it prints the number of records
that appears more than once. At last, it prints the total records in collections*/
void collection_statistic(database& data_library)
{
    Statistic statistic_obj = 
        for_each(data_library.catalog.begin(), data_library.catalog.end(), Statistic());
    int record_num = data_library.title_library.size();
    cout << statistic_obj.get_at_least_one() << " out of " << record_num 
            << " Records appear in at least one Collection" << endl;
    cout << statistic_obj.get_more_than_one() << " out of " << record_num
            << " Records appear in more than one Collection" << endl;
    cout << "Collections contain a total of " << statistic_obj.get_total_num()
            << " Records" << endl;
 
}
/*This function combines two collections into a new one. It first reads two names of the old
collections and a new name of the new collection. If the collection with the first names 
don't exist or the new name has already existed in the catalog, corresponding error will be
thrown*/
void combine_collection(database& data_library)
{
    string name1;
    cin >> name1;
    string name2;
    cin >> name2;

    string new_name;
    cin >> new_name;

    auto collection_it1 = find_collection_helper(data_library.catalog, name1);
    auto collection_it2 = find_collection_helper(data_library.catalog, name2);
    
    Collection new_collection(new_name, *collection_it1, *collection_it2);
    add_collection_helper(data_library.catalog, new_collection);
    
    cout << "Collections " << name1 << " and " << 
            name2 << " combined into new collection " << new_name << endl;
        
}

/*This functions changes the title of a record. It first tries to find the record with
the id and then change the title to a new one. If the id cannot be found or the new title
is already existed or the title is not valid, error will be thrown.*/
void modify_title(database& data_library)
{
    int number = read_number();
    
    auto id_it = find_record_id_helper(data_library.id_library, number);
    auto title_it = find_record_title_helper(data_library.title_library, (*id_it)->get_title());
    string title = read_title();
    record_already_exist(data_library.title_library, title);
    
    Record* new_record = new Record(title, *(*id_it));
    Record* old_record = *id_it;

    for_each(data_library.catalog.begin(), data_library.catalog.end(),
                [old_record, new_record](Collection &collection) {
                    if (collection.is_member_present(old_record)) {
                        collection.remove_member(old_record);
                        collection.add_member(new_record);
                    }
                });
 
    data_library.id_library.erase(id_it);
    data_library.title_library.erase(title_it);
    delete old_record;

    add_record_helper(data_library, new_record);
    cout << "Title for record " << new_record->get_ID() << " changed to " 
            << new_record->get_title() << endl;
}
