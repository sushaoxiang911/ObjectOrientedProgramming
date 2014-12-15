#ifndef COLLECTION_H
#define COLLECTION_H
/* Collections contain a name and a container of members,
represented as pointers to Records.
Collection objects manage their own Record container. 
The container of Records is not available to clients.
*/
#include "Record.h"
#include "Utility.h"
#include <fstream>
#include <set>
#include <map>
#include <string>
	
class Collection {

public:
	// Construct a collection with the specified name and no members
	Collection(const std::string& name_)
            :name(name_) {}
    
	/* Construct a Collection from an input file stream in save format, using the record list,
	restoring all the Record information.
	Record list is needed to resolve references to record members.
	No check made for whether the Collection already exists or not.
	Throw Error exception if invalid data discovered in file.
	String data input is read directly into the member variable. */
    Collection(std::ifstream& is, const std::set<Record*, comp_record_ptr_title>& library);

    /* Construct a Collection from with the specified name and the members are the union of
    collection1 and collection2*/
    Collection(const std::string& name_, 
        const Collection& collection1, const Collection& collection2);

	// Accessors
	std::string get_name() const
		{return name;}
    int size() const
        {return members.size();}

		
	// Add the Record, throw exception if there is already a Record with the same title.
	void add_member(Record* record_ptr);
	// Return true if there are no members; false otherwise
	bool empty() const
		{return members.empty();}
	// Return true if the record is present, false if not.
	bool is_member_present(Record* record_ptr) const;
	// Remove the specified Record, throw exception if the record was not found.
	void remove_member(Record* record_ptr);
	// discard all members
	void clear()
		{members.clear();}

	// Write a Collections's data to a stream in save format, with endl as specified.
	void save(std::ostream& os) const;

	// This operator defines the order relation between Collections, based just on the name
	bool operator< (const Collection& rhs) const
		{return name < rhs.name;}
    
    /*This function goes through all the records and modifies the statistic_map and track
    based on the occurrance of a specific record*/
    void get_record_list(std::map<Record*, int>& statistic_map, int& track);
	
    friend std::ostream& operator<< (std::ostream& os, const Collection& collection);
		
private:	
    std::set<Record*, comp_record_ptr_title> members;
    std::string name;
};

// Print the Collection data
std::ostream& operator<< (std::ostream& os, const Collection& collection);

#endif
