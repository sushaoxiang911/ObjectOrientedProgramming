/***** Ship Class *****/
/* A Ship has a name, initial position, amount of fuel, and parameters 
that govern its movement. It can be commanded to move to either a position or follow a course, or stop,
dock at or refuel at an Island. It consumes fuel while moving, and becomes immobile
if it runs out of fuel. It inherits the Sim_object interface to the rest of the system,
and the Track_base class provides the basic movement functionality, with the unit of time
corresponding to 1.0 for one "tick" - an hour of simulated time.

The update function updates the position and/or state of the ship.
The describe function outputs information about the ship state.
Accessors make the ship state available to either the public or to derived classes.
The is a "fat interface" for the capabilities of derived types of Ships. These
functions are implemented in this class to throw an Error exception.
*/

#ifndef SHIP_H
#define SHIP_H

#include "Sim_object.h"
#include "Track_base.h"
#include "Geometry.h"
class Island;

// the state of a basic ship class type
enum ShipState { DOCKED, STOPPED, MOVING_TO_POSITION, MOVING_ON_COURSE, DEAD_IN_THE_WATER,
                SINKING, SUNK, ON_THE_BOTTOM };

class Ship : public Sim_object, private Track_base {
public:
	// initialize, then output constructor message
	Ship(const std::string& name_, Point position_, double fuel_capacity_, 
		double maximum_speed_, double fuel_consumption_, int resistance_);
		
	// made pure virtual to mark this class as abstract, but defined anyway
	// to output destructor message
	virtual ~Ship() = 0;
	
	/*** Readers ***/
	// return the current position
	Point get_location() const override {return Track_base::get_position();}
	
	// Return true if ship can move (it is not dead in the water or in the process or sinking); 
	bool can_move() const;
	
	// Return true if ship is moving;
	bool is_moving() const;

	// Return true if ship is docked; 
	bool is_docked() const;
	
	// Return true if ship is afloat (not in process of sinking), false if not
	bool is_afloat() const;
		
	// Return true if ship is on the bottom
	bool is_on_the_bottom() const;
	
	// Return true if the ship is Stopped and the distance to the supplied island
	// is less than or equal to 0.1 nm
	bool can_dock(Island* island_ptr) const;
	
	/*** Interface to derived classes ***/
	// Update the state of the Ship
	void update() override;
	// output a description of current state to cout
	void describe() const override;
	
	void broadcast_current_state() override;
	
	/*** Command functions ***/
	// Start moving to a destination position at a speed
     // may throw Error("Ship cannot move!")
     // may throw Error("Ship cannot go that fast!")
	virtual void set_destination_position_and_speed(Point destination_position, double speed);
	// Start moving on a course and speed
     // may throw Error("Ship cannot move!")
     // may throw Error("Ship cannot go that fast!");
	virtual void set_course_and_speed(double course, double speed);
	// Stop moving
     // may throw Error("Ship cannot move!");
	virtual void stop();
	// dock at an Island - set our position = Island's position, go into Docked state
     // may throw Error("Can't dock!");
	virtual void dock(Island * island_ptr);
	// Refuel - must already be docked at an island; fill takes as much as possible
     // may throw Error("Must be docked!");
	virtual void refuel();

	/*** Fat interface command functions ***/
	// These functions throw an Error exception for this class
    // will always throw Error("Cannot load at a destination!");
	virtual void set_load_destination(Island *);
    // will always throw Error("Cannot unload at a destination!");
	virtual void set_unload_destination(Island *);
    // will always throw Error("Cannot attack!");
	virtual void attack(Ship * in_target_ptr);
    // will always throw Error("Cannot attack!");
	virtual void stop_attack();

	// interactions with other objects
	// receive a hit from an attacker
	virtual void receive_hit(int hit_force, Ship* attacker_ptr);
		
protected:
	// future projects may need additional protected members

	double get_maximum_speed() const;
	// return pointer to the Island currently docked at, or nullptr if not docked
	Island* get_docked_Island() const;

private:
    Point destination;                  // the current destination
    double fuel_capacity;               // the capacity of fuel
    double maximum_speed;               // the max speed
	double fuel_consumption;			// tons/nm required
    int resistance;                     // Current resistance
	double fuel;						// Current amount of fuel
    
    ShipState ship_state;               // Current state
    Island* current_island_ptr;         // Current Docked island 

	// Updates position, fuel, and movement_state, assuming 1 time unit (1 hr)
	void calculate_movement();

	// disallow copy/move, construction or assignment
    Ship(const Ship& rhs);
    Ship(Ship&& rhs);
    Ship& operator= (const Ship& rhs);
    Ship& operator= (Ship&& rhs);
};

#endif
