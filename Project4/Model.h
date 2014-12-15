/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Islands and Ships there are, but it does not
know about any of their derived classes, nor which Ships are of what kind of Ship. 
It has facilities for looking up objects by name, and removing Ships.  When
created, it creates an initial group of Islands and Ships using the Ship_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.
*/

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <map>
#include <vector>

// Declare the global model pointer
class Model;
extern Model* g_Model_ptr;

class Sim_object;
class Island;
class Ship;
class View;
struct Point;

class Model {
public:
	// create the initial objects, output constructor message
	Model();
	
	// destroy all objects, output destructor message
	~Model();

	// return the current time
	int get_time() {return time;}

	// is name already in use for either ship or island?
    // either the identical name, or identical in first two characters counts as in-use
	bool is_name_in_use(const std::string& name) const;

	// is there such an island?
	bool is_island_present(const std::string& name) const;
	// add a new island to the lists
	void add_island(Island* island_ptr);
	// will throw Error("Island not found!") if no island of that name
	Island* get_island_ptr(const std::string& name) const;

	// is there such an ship?
	bool is_ship_present(const std::string& name) const;
	// add a new ship to the list, and update the view
	void add_ship(Ship* ship_ptr);
	// will throw Error("Ship not found!") if no ship of that name
	Ship* get_ship_ptr(const std::string& name) const;
	
	// tell all objects to describe themselves
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();	
	
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(View* view_ptr);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(View* view_ptr);
	
    // notify the views about an object's location
	void notify_location(const std::string& name, Point location);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);

	
private:
	int time;		                                        // the simulated time
    std::map<std::string, Sim_object*> object_container;    // the Sim_object container
    std::map<std::string, Ship*> ship_container;            // the Ship container
    std::map<std::string, Island*> island_container;        // the Island container

    std::vector<View*> view_container;                      // the view container

    // This function inserts the island_ptr into the containers
    void insert_island(Island* island_ptr);
    // This function inserts the ship_ptr into the containers
    void insert_ship(Ship* ship_ptr);

	// disallow copy/move construction or assignment
    Model(const Model& rhs);
    Model(Model&& rhs);
    Model& operator= (const Model& rhs);
    Model& operator= (Model&& rhs);

};

#endif
