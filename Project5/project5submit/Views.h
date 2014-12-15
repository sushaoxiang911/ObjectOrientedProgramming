#ifndef VIEWS_H
#define VIEWS_H

#include "View.h"
#include "Geometry.h"
#include <map>

struct ship_sailing;

/* the map_view class that is in charge of printing a map view
*/
class map_view : public View {
public:
	// default constructor sets the default size, scale, and origin
	map_view(); 
    // destructor
	~map_view() {}
	
    // prints out the current map
	void draw() override;
	// Discard the saved information - drawing will show only a empty pattern
	void clear() override; 

	// Save the supplied name and location for future use in a draw() call
	// If the name is already present,the new location replaces the previous one.
	void update_location(const std::string& name, Point location) override;
	// Remove the name and its location; no error if the name is not present.
	void update_remove(const std::string& name) override;
    // sailing_view functions
    void update_speed(const std::string& name, double speed) override {}
    void update_course(const std::string& name, double course) override {}
    void update_fuel(const std::string& name, double fuel) override {}
    // override get_name. Just return an empty string, indicating it is not a bridge view
    std::string get_name() override {return "";}
    
    // map_view new functions
	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	void set_size(int size_);	
	// If scale is not postive, will throw Error("New map scale must be positive!");
	void set_scale(double scale_);
	// any values are legal for the origin
	void set_origin(Point origin_);
	// set the parameters to the default values
	void set_defaults();
		
private:
	int size;			// current size of the display
	double scale;		// distance per cell of the display
	Point origin;		// coordinates of the lower-left-hand corner

    // the object data container	
    std::map<std::string, Point> location_record;
	// Calculate the cell subscripts corresponding to the location parameter, using the 
	// current size, scale, and origin of the display. 
	// Return true if the location is within the map, false if not
	bool get_subscripts(int &ix, int &iy, Point location); 
    // convert to print subscript
    int convert(int x) {return size-1-x;}
};

/* the sailing view class which is in charge of printing the sailing view
*/
class sailing_view : public View {
public:
    ~sailing_view() {}   

    void draw() override;
    void clear() override;

    // override map_view
    void update_location(const std::string& name, Point location) override {}
    void update_remove(const std::string& name) override;
    // update ship speed
    void update_speed(const std::string& name, double speed) override;
    // update ship course
    void update_course(const std::string& name, double course) override;
    // update ship fuel
    void update_fuel(const std::string& name, double fuel) override;  
    // update get_name. return empty string, indicating it is not a bridge view
    std::string get_name() override {return "";}
private:
    // the data container
    std::map<std::string, ship_sailing> sailing_record;
};

/* the bridge view class which is in charge of printing a bridge view of a specific ship
*/
class bridge_view : public View {
public:
    // constructor the object with a specific name
    bridge_view(std::string name_);
    ~bridge_view() {}
    
    void draw() override;
    void clear() override;

    // update location if there is a change on a ship's location
    void update_location(const std::string& name, Point location) override;
    // if the ownship is removed, means it is sunk and we set sunk to true
    void update_remove(const std::string& name) override;
    // if the course of ownship is updated, set the course
    void update_course(const std::string& name, double course) override;
    void update_fuel(const std::string& name, double fuel) override {};   
    void update_speed(const std::string& name, double speed) override {};
    // return the ship_name of this view
    std::string get_name() override {return ship_name;}
private:
    bool sunk;                                      // if the ownship of the view is sunk
    double ship_course;                             // the current course of the ship
    std::string ship_name;                          // the name of the ship
    std::map<std::string, Point> location_record;   // the location of all the Sim_object
};

#endif
