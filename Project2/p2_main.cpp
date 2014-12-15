#include "Ordered_list.h"
#include "Collection.h"
#include "Record.h"
#include "Utility.h"
#include <iostream>
#include <cctype>
#include <fstream>

using namespace std;

struct Less_than_id
{
    bool operator() (const Record *ptr1, const Record *ptr2) const
    {
        return ptr1->get_ID() < ptr2->get_ID();
    }
};

struct is_in_Collection {
    
    bool operator()(Collection collection, Record* record_ptr) const
    {
        return collection.is_member_present(record_ptr);
    }
} is_in_Collection_obj;

void read_to_newline();

String read_title();

void unrecognized_command();

void print_record_title(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library);

void print_record_id(Ordered_list<Record*, Less_than_id>& id_library);

void print_collection(Ordered_list<Collection>& catalog);

void print_library(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library);

void print_catalog(Ordered_list<Collection>& catalog);

void print_allocation(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                            Ordered_list<Collection>& catalog);

void add_record(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                    Ordered_list<Record*, Less_than_id>& id_library);

void add_collection(Ordered_list<Collection>& catalog);

void add_record_to_collection(Ordered_list<Collection>& catalog,
                                Ordered_list<Record*, Less_than_id>& id_library);

void modify_record_rating(Ordered_list<Record*, Less_than_id>& id_library);

void delete_record_from_library(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                                    Ordered_list<Collection>& catalog,
                                    Ordered_list<Record*, Less_than_id>& id_library);

void delete_collection_from_catalog(Ordered_list<Collection>& catalog);

void delete_record_from_collection(Ordered_list<Collection>& catalog,
                                        Ordered_list<Record*, Less_than_id>& id_library);

void clear_library (Ordered_list<Collection>& catalog,
                        Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                        Ordered_list<Record*, Less_than_id>& id_library);

void clear_catalog(Ordered_list<Collection>& catalog);

void clear_all(Ordered_list<Collection>& catalog,
                Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                Ordered_list<Record*, Less_than_id>& id_library);

void save_all(Ordered_list<Collection>& catalog,
                Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                Ordered_list<Record*, Less_than_id>& id_library);

void restore_all(Ordered_list<Collection>& catalog,
                    Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                    Ordered_list<Record*, Less_than_id>& id_library);

void quit(Ordered_list<Collection>& catalog,
            Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
            Ordered_list<Record*, Less_than_id>& id_library);

int main() {
    char action;
    char object;
 
    Ordered_list<Record*, Less_than_ptr<Record*> > title_library;
    Ordered_list<Record*, Less_than_id> id_library;

    Ordered_list<Collection> catalog;
    while(true) {
        try {
            cout << "\nEnter command: ";
            cin >> action >> object;
            switch (action) {
                case 'f':
                    switch (object) {
                        case 'r':
                            print_record_title(title_library);
                            break;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                case 'p':
                    switch(object) {
                        case 'r':
                            print_record_id(id_library);
                            break;
                        case 'c':
                            print_collection(catalog);
                            break;
                        case 'L':
                            print_library(title_library);
                            break;
                        case 'C':
                            print_catalog(catalog);
                            break;
                        case 'a':
                            print_allocation(title_library, catalog);
                            break;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                case 'a':
                    switch(object) {
                        case 'r':
                            add_record(title_library, id_library);
                            break;
                        case 'c':
                            add_collection(catalog);
                            break;
                        case 'm':
                            add_record_to_collection(catalog, id_library);
                            break;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                case 'm':
                    switch(object) {
                        case 'r':
                            modify_record_rating(id_library);
                            break;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                case 'd':
                    switch (object) {
                        case 'r':
                            delete_record_from_library(title_library, catalog, id_library);
                            break;
                        case 'c':
                            delete_collection_from_catalog(catalog);
                            break;
                        case 'm':
                            delete_record_from_collection(catalog, id_library);
                            break;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                case 'c':
                    switch (object) {
                        case 'L':
                            clear_library(catalog, title_library, id_library);
                            cout << "All records deleted" << endl;
                            break;
                        case 'C':
                            clear_catalog(catalog);
                            cout << "All collections deleted" << endl;
                            break;
                        case 'A':
                            clear_all(catalog, title_library, id_library);
                            cout << "All data deleted" << endl;
                            break;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                case 's':
                    switch(object) {
                        case 'A':
                            save_all(catalog, title_library, id_library);
                            break;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                case 'r':
                    switch(object) {
                        case 'A':
                            restore_all(catalog, title_library, id_library);
                            break;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                case 'q':
                    switch(object) {
                        case 'q':
                            quit(catalog, title_library, id_library);
                            return 0;
                        default:
                            unrecognized_command();
                            break;
                    }
                    break;
                default:
                    unrecognized_command();
            } 
        } catch (Error &exception) {
            cin.clear();
            read_to_newline();
            cout << exception.msg << endl;
        } catch (bad_alloc &ba) {
            cout << "Bad allocation!" << endl;
            exit(1);
        } catch (String_exception &exception) {
            cout << exception.msg << endl;
            exit(1);
        } catch (...) {
            cout << "Unknown exception caught!" << endl;
            exit(1);
        }
    } 
}
/*This function read all the characters from the stream until hitting a newline character*/
void read_to_newline()
{
    while(cin.get() != '\n'){}
}
/*This function first reads the entire line to the result and then eliminates all the
heading and tailing whitespaces. Also it turns continuing whitespace into a single one
the allocation will also be shrinked.*/
String read_title()
{
    String buff;
    getline(cin, buff);
    int i = 0;

    while (i < buff.size()) {
        if (isspace(buff[i]))
            buff[i] = ' ';
        ++i;
    }

    i = 0;
    while (i < buff.size()) {
        if (isspace(buff[i]))
            ++i;
        else
            break;
    }
    if (i > 0)
        buff.remove(0, i);
    i = buff.size() - 1;
    while (i >= 0) {
        if (isspace(buff[i]))
            --i;
        else
            break;
    }
    if (i < buff.size() - 1)
        buff.remove(i + 1, buff.size() - i - 1);

    i = 0;
    int j = 0;
    while (i < buff.size()) {
        while (i < buff.size()) {
            if (isspace(buff[i]))
                break;
            else
                ++i;
        }
        j = i;
        while (j < buff.size()) {
            if (!isspace(buff[j]))
                break;
            else
                ++j;
        }
        if (j - i - 1 > 0)
            buff.remove(i + 1, j - i - 1);
        ++i;
    }
    String result(buff);
    return result;
}
/*This function is called when there are bad commands. It first reads all the remained
characters of the current line and print error message*/
void unrecognized_command()
{
    read_to_newline();
    cout << "Unrecognized command!" << endl;
}
/*This function takes title_library as the argument. It first reads the input searching 
title and then searches it from title_library. If the title is not valid or cannot find
the title from the library, it will throw corresponding exception*/
void print_record_title(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library)
{
    String search_title = read_title();
    if (search_title.size() > 0) {
        Record temp_record(search_title);
        auto it = title_library.find(&temp_record);
        if (it == title_library.end())
            throw Error("No record with that title!");
        else
            cout << *(*it);
        
    } else 
        throw Error("Could not read a title!");         
}
/*This function takes id_library as the argument. It first reads the searching number 
and then searches it from id_library. If it cannot read an iteger id or cannot find 
the id from the library, it will throw corresponding exception*/
void print_record_id(Ordered_list<Record*, Less_than_id>& id_library)
{
    int number = -1;
    if (!(cin >> number)) {
        throw Error("Could not read an integer value!");
    }

    Record temp_record(number);
    auto it = id_library.find(&temp_record);
    if (it == id_library.end()) {
        throw Error("No record with that ID!");
    }
    cout << *(*it);
}
/*This function takes catalog as the argument. It first reads the searching name
and then searches it from catalog. If it cannot find the name from catalog, it will
throw and exception*/
void print_collection(Ordered_list<Collection>& catalog)
{
    String name;
    cin >> name;
    Collection temp_collection(name);
    
    auto it = catalog.find(temp_collection);
    if (it == catalog.end()) {
        throw Error("No collection with that name!");
    }
    cout << *it;
}
/*This function takes title_library as the argument. It prints all the records that 
are contained in the library*/
void print_library(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library)
{
    if (title_library.empty())
        cout << "Library is empty" << endl;
    else {
        cout << "Library contains " << title_library.size() << " records:" << endl;
        for (Record* record : title_library) 
            cout << *record;
    }
}
/*This function takes catalog as the argument. It prints all the collections that
are contained in the catalog*/
void print_catalog(Ordered_list<Collection>& catalog)
{
    if (catalog.empty())
        cout << "Catalog is empty" << endl;
    else {
        cout << "Catalog contains " << catalog.size() << " collections:" << endl;
        for (auto it = catalog.begin(); it != catalog.end(); ++it)
            cout << *it;
    }
} 
/*This function takes title_library and catalog as arguments. It print all the related
information about record number, collection number, container number, nodes contained
in the containers and the strings as long as the byte allocation*/
void print_allocation(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library, 
                            Ordered_list<Collection>& catalog)
{
    cout << "Memory allocations:" << endl;
    cout << "Records: " << title_library.size() << endl;
    cout << "Collections: " << catalog.size() << endl;
    cout << "Lists: " << g_Ordered_list_count << endl;
    cout << "List Nodes: " << g_Ordered_list_Node_count << endl;
    cout << "Strings: " << String::get_number() << " with " 
            << String::get_total_allocation() << " bytes total" << endl;
}
/*This function takes title_library and id_library as arguments. It creates a
new record with the title and medium obtained from input and adds it into
title_library and id_library. If the name is not valid or there is a record 
with the same name inside the library, it throws the corresponding exception. */
void add_record(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                    Ordered_list<Record*, Less_than_id>& id_library)
{
    String medium;
    cin >> medium;
    String title = read_title();

    if (title.size() > 0) {
        Record temp_record(title);
        auto it = title_library.find(&temp_record);
        if (it == title_library.end()) {
            Record* record_ptr = new Record(medium, title);
            title_library.insert(record_ptr);
            id_library.insert(record_ptr);
            cout << "Record " << record_ptr->get_ID() << " added" << endl;
        } else 
            throw Error("Library already has a record with this title!");
    } else 
        throw Error("Could not read a title!");
}
/*This function takes catalog as the argument. It constructs a new collection
with the name obtained from input and adds it into the catalog. If the collection
with that name has been added, it will throw an exception. */
void add_collection(Ordered_list<Collection>& catalog)
{
    String name;
    cin >> name;
    Collection temp_collection(name);
    auto it = catalog.find(temp_collection);
    if (it == catalog.end()) {
        catalog.insert(temp_collection);
        cout << "Collection " << temp_collection.get_name() << " added" << endl;
    } else {
        throw Error("Catalog already has a collection with this name!");
    }
}
/*This function takes catalog and id_library as the arguments. It reads the collection
name and the reocord id and add the record with that id into the collection. If there is
no collection with that name or it cannot read an number as id or there is no record
with that id, it will throw the corresponding exception.*/
void add_record_to_collection(Ordered_list<Collection>& catalog, 
                                Ordered_list<Record*, Less_than_id>& id_library)
{
    String name;
    cin >> name;
    Collection temp_collection(name);
    auto collection_it = catalog.find(temp_collection);
    if (collection_it == catalog.end()){
        throw Error("No collection with that name!");
    }
    int number = -1;
    if (!(cin >> number)) {
        throw Error("Could not read and integer value!");
    }
    
    Record temp_record(number);
    auto record_it = id_library.find(&temp_record);
    if (record_it == id_library.end()) {
        throw Error("No record with that ID!");
    }
    (*collection_it).add_member(*record_it);
    cout << "Member " << (*record_it)->get_ID() << " " << (*record_it)->get_title() 
         << " added" << endl;
}
/*This function takes id_library as the argument. It first read an iteger as the searching
id and another integer as the new rating. If it cannot read the integer or there is no 
record with the id number obtained from stream, it will throw the corresponding exception.*/
void modify_record_rating(Ordered_list<Record*, Less_than_id>& id_library)
{
    int number = -1;
    if (!(cin >> number)) {
        throw Error("Could not read an integer value!");
    }
    Record temp_record(number);

    auto it = id_library.find(&temp_record);
    if (it == id_library.end()) {
        throw Error("No record with that ID");
    }
    if (!(cin >> number)) {
        throw Error("Could not read an integer value!");
    }
    (*it)->set_rating(number);
    cout << "Rating for record " << (*it)->get_ID() << " changed to " << number << endl;
}
/*This function takes title_library, catalog, id_library. It reads the record title that
is going to be deleted and try to find if it is in a collection. If not, it delete the record
and the node in title_library and id_library. If the title is not valid or there is no record
with that title or the record is in a collection, it will throw the corresponding exception.*/
void delete_record_from_library(Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                                    Ordered_list<Collection>& catalog,
                                    Ordered_list<Record*, Less_than_id>& id_library)
{
    String search_title = read_title();
    if (search_title.size() <= 0)
        throw Error("Could not read a title!");
    Record name_record(search_title);
    auto title_it = title_library.find(&name_record);
    if (title_it == title_library.end())
        throw Error("No record with that title!");
    
    Record* record_ptr = *title_it;

    if (apply_if_arg (catalog.begin(), catalog.end(), is_in_Collection_obj, record_ptr))
        throw Error("Cannot delete a record that is a member of a collection!");
    cout << "Record " << record_ptr->get_ID() << " " 
            << record_ptr->get_title() << " deleted" << endl;
    title_library.erase(title_it);
    Record id_record(record_ptr->get_ID());
    auto id_it = id_library.find(&id_record);
    id_library.erase(id_it);
    delete record_ptr;

}
/*This function takes catalog as the argument. It reads the name taht is going to be 
deleted and delete the collection from the catalog. if there is no collection with that
name, it will throw an exception.*/
void delete_collection_from_catalog(Ordered_list<Collection>& catalog)
{
    String search_name;
    cin >> search_name;
    
    Collection temp_collection(search_name);

    auto collection_it = catalog.find(temp_collection);
    if (collection_it == catalog.end()) {
        throw Error("No collection with that name!");
    }
    cout << "Collection " << (*collection_it).get_name() << " deleted" << endl;
    (*collection_it).clear();
    catalog.erase(collection_it);
    
    
}
/*This function takes catalog and id_library as the arguments. It first reads collection name
and the record id and delete the record with that id from the collection. If there is no collection
with that name or it could not read an integer for record id or there is no record with that id
number or the record with that id is not in the collection, it will throw the corresponding error.*/
void delete_record_from_collection(Ordered_list<Collection>& catalog, 
                                        Ordered_list<Record*, Less_than_id>& id_library)
{
    String search_name;
    cin >> search_name;
    
    Collection temp_collection(search_name); 

    auto collection_it = catalog.find(temp_collection);
    
    if (collection_it == catalog.end()) {
        throw Error("No collection with that name!");
    }

    int number = -1;
    if (!(cin >> number)) {
        throw Error("Could not read an integer value!");
    }
    
    Record temp_record(number);
    auto record_it = id_library.find(&temp_record);
    if (record_it == id_library.end()) {
        throw Error("No record with that ID!");
    }

    (*collection_it).remove_member(*record_it);
    
    cout << "Member " << (*record_it)->get_ID() << ' '
            << (*record_it)->get_title() << " deleted" << endl;
}
/*This function takes catalog, title_library and id_library as arguments. It clears all
the records in the library and clear libraries. If thereis one record that is in a collection, 
it will throw the exception.*/
void clear_library (Ordered_list<Collection>& catalog, 
                        Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                        Ordered_list<Record*, Less_than_id>& id_library) 
{
    auto collection_it = catalog.begin();
    while (collection_it != catalog.end()) {
        if (!(*collection_it).empty()) {
            throw Error("Cannot clear all records unless all collections are empty!");
        }
        ++collection_it;
    }
    auto record_it = title_library.begin();
    while (record_it != title_library.end()) {
        delete *record_it;
        ++record_it;
    }
    title_library.clear();
    id_library.clear();
    Record::reset_ID_counter();
}
/*This function takes the catalog as the argument. It clear all the collections in the 
catalog.*/
void clear_catalog(Ordered_list<Collection>& catalog)
{
    catalog.clear();
}
/*This function takes catalog, title_library and id_library as arguments. It first clears
all the collections and the catalog. Then it clear title_library and id library.*/
void clear_all(Ordered_list<Collection>& catalog,
                Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                Ordered_list<Record*, Less_than_id>& id_library)
{
    clear_catalog(catalog);
    clear_library(catalog, title_library, id_library);
}
/*This function takes catalog, title_library and id_library as arguments. It saves all the 
records and collection in format into a file. If the file cannot be opened, it will throw
the exception.*/
void save_all(Ordered_list<Collection>& catalog,  
                Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                Ordered_list<Record*, Less_than_id>& id_library)
{
    String file_name;
    cin >> file_name;
    ofstream os;
    os.open (file_name.c_str());
    if (!os.is_open()) {
        throw Error("Could not open file!");
    }
    os << title_library.size() << endl;
    auto record_it = title_library.begin();
    while (record_it != title_library.end()) {
        (*record_it)->save(os);
        ++record_it;
    }
    
    os << catalog.size() << endl;
    auto collection_it = catalog.begin();
    while (collection_it != catalog.end()) {
        (*collection_it).save(os);
        ++collection_it;
    }
    os.close();
    cout << "Data saved" << endl;
}
/*This function takes catalog, title_library and id_library as arguments. It tries to restore
all the data from file into the catalog an libraries. If the data in the file is not valid and
get checked during restoring process, the original data would be obtained back. If the file cannot
be opened or there is invalid data in the file, it will throw the corresponding exception.*/
void restore_all(Ordered_list<Collection>& catalog,  
                    Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
                    Ordered_list<Record*, Less_than_id>& id_library)
{
    String file_name;
    cin >> file_name;
    ifstream is;
    is.open(file_name.c_str());
    if (!is.is_open()) {
        throw Error("Could not open file!");
    }
    
    Ordered_list<Record*, Less_than_ptr<Record*> >title_library_backup(title_library);
    Ordered_list<Record*, Less_than_id>id_library_backup(id_library);
    Ordered_list<Collection>catalog_backup(catalog);
    
    title_library.clear();
    id_library.clear();
    catalog.clear();
    
    try {
        Record::save_ID_counter();
        Record::reset_ID_counter();
  
        int number = -1;
        if (!(is >> number)) {
            throw Error("Invalid data found in file!");
        }  
   
        for (int i = 0; i < number; ++i) {
            Record* new_record = new Record(is);
            title_library.insert(new_record);
            id_library.insert(new_record);
        }
 
        if (!(is >> number)) {
            throw Error("Invalid data found in file!");
        }

        for (int i = 0; i < number; ++i) {
            Collection new_collection(is, title_library);
            catalog.insert(new_collection);
        }
        
    } catch (Error &exception) {
  
        clear_all(catalog, title_library, id_library);
        title_library = title_library_backup;
        id_library = id_library_backup;
        catalog = catalog_backup; 
        
        Record::restore_ID_counter();
        
        is.close();
        throw exception;
    }
    is.close();

    auto it = title_library_backup.begin();
    while (it != title_library_backup.end()) {
        delete *it;
        ++it;
    }
    cout << "Data loaded" << endl;
}
/*This function takes catalog, title_library and id_library as arguments. It
clears all the data and terminates the program*/
void quit(Ordered_list<Collection>& catalog,  
            Ordered_list<Record*, Less_than_ptr<Record*> >& title_library,
            Ordered_list<Record*, Less_than_id>& id_library)
{
    clear_all(catalog, title_library, id_library);
    cout << "All data deleted" << endl;
    cout << "Done" << endl;
}



