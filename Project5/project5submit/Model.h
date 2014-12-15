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
#include <memory>

class Sim_object;
class Island;
class Ship;
class View;
struct Point;

class Model {
public:
    static Model* get_instance()
    {
        if (!model_ptr)
            model_ptr = new Model();
        return model_ptr;
    }

    friend class Model_destroyer;
	// return the current time
	int get_time() {return time;}

	// is name already in use for either ship or island?
    // either the identical name, or identical in first two characters counts as in-use
	bool is_name_in_use(const std::string& name) const;

	// is there such an island?
	bool is_island_present(const std::string& name) const;
	// add a new island to the lists
	void add_island(std::shared_ptr<Island> island_ptr);
	// will throw Error("Island not found!") if no island of that name
	std::shared_ptr<Island> get_island_ptr(const std::string& name) const;

	// is there a ship with name
	bool is_ship_present(const std::string& name) const;
	// add a new ship to the list, and update the view
	void add_ship(std::shared_ptr<Ship> ship_ptr);
	// will throw Error("Ship not found!") if no ship of that name
	std::shared_ptr<Ship> get_ship_ptr(const std::string& name) const;
	
	// tell all objects to describe themselves
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();	
	
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(std::shared_ptr<View> view_ptr);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(std::shared_ptr<View> view_ptr);
	
    // notify the views about an object's location
	void notify_location(const std::string& name, Point location);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);
    // notify views about a ship's speed
    void notify_speed(const std::string& name, double speed);
    // notify views about a ship's course
    void notify_course(const std::string& name, double course);
    // notify views about a ship's fuel
    void notify_fuel(const std::string& name, double fuel);

    //remove the Ship from the containers
    void remove_ship(std::shared_ptr<Ship> ship_ptr);
    std::map<std::string, std::shared_ptr<Island> > get_Island_container() 
    {return island_container;}
    std::shared_ptr<Island> is_island_at_position(Point position);    
	
private:
    
    // create the initial objects
	Model();

	// destroy all objects
    ~Model();

    // singleton pointer
    static Model* model_ptr;

	// the simulated time
    int time;     
    // the Sim_object container
    std::map<std::string, std::shared_ptr<Sim_object> > object_container;
    // the Ship container   
    std::map<std::string, std::shared_ptr<Ship> > ship_container;
    // the Island container
    std::map<std::string, std::shared_ptr<Island> > island_container;
    // the View container
    std::vector<std::shared_ptr<View> > view_container;

    // This function inserts the island_ptr into the containers
    void insert_island(std::shared_ptr<Island> island_ptr);
    // This function inserts the ship_ptr into the containers
    void insert_ship(std::shared_ptr<Ship> ship_ptr);

	// disallow copy/move construction or assignment
    Model(const Model& rhs);
    Model(Model&& rhs);
    Model& operator= (const Model& rhs);
    Model& operator= (Model&& rhs);

};

// the destroyer of the singleton class
class Model_destroyer {
public:
    ~Model_destroyer() {delete Model::model_ptr;}        
};


#endif
