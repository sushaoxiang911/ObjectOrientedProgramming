#include "Ship.h"
#include "Island.h"
#include "Utility.h"
#include "Model.h"
#include <iostream>

using std::cout; using std::endl;
using std::string;
using std::shared_ptr;
/*
Define the destructor function even if it was declared as a pure virtual function.
This seems odd, because pure virtual functions are usually not defined in the class
that declares them. But this is often done as a way to make a class abstract, 
if there is no other virtual function that makes sense to mark as pure. Here we
are defining it just to get the destructor message output.
*/
Ship::Ship(const string& name_, Point position_, double fuel_capacity_,
            double maximum_speed_, double fuel_consumption_, int resistance_)
            :Sim_object(name_), track(position_), fuel_capacity(fuel_capacity_),
            maximum_speed(maximum_speed_), fuel_consumption(fuel_consumption_),
            resistance(resistance_), fuel(fuel_capacity_), ship_state(STOPPED)
{}

Ship::~Ship()
{}

bool Ship::can_move() const
{
    return (ship_state < DEAD_IN_THE_WATER);
}

bool Ship::is_moving() const
{
    return (ship_state == MOVING_TO_POSITION || ship_state == MOVING_ON_COURSE);
}

bool Ship::is_docked() const
{
    return (ship_state == DOCKED);
}

bool Ship::is_afloat() const
{
    return (ship_state != SUNK);
}

bool Ship::can_dock(shared_ptr<Island> island_ptr) const
{
    return (ship_state == STOPPED && 
            cartesian_distance(island_ptr->get_location(), get_location()) <= 0.1);
}

void Ship::update()
{
    cout << get_name();
    switch (ship_state) {

        case SUNK:
            cout << " sunk" << endl;
            break;
        case MOVING_ON_COURSE:
            calculate_movement();
            cout << " now at " << get_location() << endl;
            Model::get_instance()->notify_location(get_name(), get_location());
            break;
        case MOVING_TO_POSITION:
            calculate_movement();
            cout << " now at " << get_location() << endl;
            Model::get_instance()->notify_location(get_name(), get_location());
            break;
        case STOPPED:
            cout << " stopped at " << get_location() << endl;
            break;
        case DOCKED:
            cout << " docked at " << get_docked_Island()->get_name() << endl;
            break;
        case DEAD_IN_THE_WATER:
            cout << " dead in the water at " << get_location() << endl;
            break;
    }
}

void Ship::describe() const
{
    cout << get_name() << " at " << get_location();
    if (!is_afloat())
        cout << " sunk" << endl;
    else {
        cout << ", fuel: " << fuel << " tons, resistance: " << resistance << endl;
        switch (ship_state) {
            case MOVING_ON_COURSE:
                cout << "Moving on " << track.get_course_speed() << endl;
                break;
            case MOVING_TO_POSITION:
                cout << "Moving to " << destination << " on " 
                        << track.get_course_speed() << endl;
                break;
            case STOPPED:
                cout << "Stopped" << endl;
                break;
            case DOCKED: 
                cout << "Docked at " << current_island_ptr->get_name() << endl;
                break;
            default: 
                cout << "Dead in the water" << endl;
                break;
        }
    }
}

void Ship::broadcast_current_state()
{
    Model::get_instance()->notify_location(get_name(), get_location());
    Model::get_instance()->notify_speed(get_name(), track.get_speed());
    Model::get_instance()->notify_course(get_name(), track.get_course());
    Model::get_instance()->notify_fuel(get_name(), fuel); 
}

void Ship::set_destination_position_and_speed(Point destination_position, double speed)
{
    if (!can_move())
        throw Error("Ship cannot move!");
    if (get_maximum_speed() < speed)
        throw Error("Ship cannot go that fast!");
    Compass_vector current_compass_vector(get_location(), destination_position);
    track.set_course(current_compass_vector.direction);
    track.set_speed(speed);
    Model::get_instance()->notify_speed(get_name(), track.get_speed());
    Model::get_instance()->notify_course(get_name(), track.get_course());
    destination = destination_position;
    cout << get_name() << " will sail on " << track.get_course_speed() 
            << " to " << destination << endl;
    ship_state = MOVING_TO_POSITION;
}

void Ship::set_course_and_speed(double course, double speed)
{
    if (!can_move())
        throw Error("Ship cannot move!");
    if (get_maximum_speed() < speed)
        throw Error("Ship cannot go that fast!");
    track.set_course(course);
    track.set_speed(speed);
    Model::get_instance()->notify_speed(get_name(), track.get_speed());
    Model::get_instance()->notify_course(get_name(), track.get_course());
    cout << get_name() << " will sail on " << track.get_course_speed() << endl;
    ship_state = MOVING_ON_COURSE;
}

void Ship::stop()
{
    if (!can_move())
        throw Error("Ship cannot move!");
    track.set_speed(0.);
    Model::get_instance()->notify_speed(get_name(), track.get_speed());
    cout << get_name() << " stopping at " << get_location() << endl;
    ship_state = STOPPED;
}

void Ship::dock(shared_ptr<Island> island_ptr)
{
    if (!can_dock(island_ptr))
        throw Error("Can't dock!");
    track.set_position(island_ptr->get_location());

    Model::get_instance()->notify_location(get_name(), get_location());

    current_island_ptr = island_ptr;
    ship_state = DOCKED;
    cout << get_name() << " docked at " << get_docked_Island()->get_name() << endl;
}

void Ship::refuel()
{
    if (!is_docked())
        throw Error("Must be docked!");
    double fuel_required = fuel_capacity - fuel;
    if (fuel_required < 0.005) {
        fuel = fuel_capacity;
        Model::get_instance()->notify_fuel(get_name(), fuel); 
        return;
    }
    double fuel_provided = get_docked_Island()->provide_fuel(fuel_required);
    fuel += fuel_provided;
    Model::get_instance()->notify_fuel(get_name(), fuel); 
    cout << get_name() << " now has " << fuel << " tons of fuel" << endl;
}

void Ship::set_load_destination(shared_ptr<Island> island_ptr)
{
    throw Error("Cannot load at a destination!");
}

void Ship::set_unload_destination(shared_ptr<Island> island_ptr)
{
    throw Error("Cannot unload at a destination!");
}

void Ship::attack(shared_ptr<Ship> in_target_ptr)
{
    throw Error("Cannot attack!");
}

void Ship::stop_attack()
{
    throw Error("Cannot attack!");
}

void Ship::receive_hit(int hit_force, shared_ptr<Ship> attacker_ptr)
{
    resistance -= hit_force;
    cout << get_name() << " hit with " << hit_force 
            << ", resistance now " << resistance << endl;
    
    if (resistance < 0) {
        cout << get_name() << " sunk" << endl;
        ship_state = SUNK;
        track.set_speed(0.);
        Model::get_instance()->notify_speed(get_name(), track.get_speed());
        Model::get_instance()->notify_gone(get_name());
        Model::get_instance()->remove_ship(shared_from_this());
    }
}
/* Protected Function Definitions*/
double Ship::get_maximum_speed() const
{
    return maximum_speed;
}

shared_ptr<Island> Ship::get_docked_Island() const
{
    return current_island_ptr;
}
/* Private Function Definitions */

/*
Calculate the new position of a ship based on how it is moving, its speed, and
fuel state. This function should be called only if the state is 
MOVING_TO_POSITION or MOVING_ON_COURSE.

Track_base has an update_position(double time) function that computes the new position
of an object after the specified time has elapsed. If the Ship is going to move
for a full time unit (one hour), then it will get go the "full step" distance,
so update_position would be called with time = 1.0. If we can move less than that,
e.g. due to not enough fuel, update position  will be called with the corresponding
time less than 1.0.

For clarity in specifying the computation, this code assumes the specified private variable names, 
but you may change the variable names or state names if you wish (e.g. movement_state).
*/
void Ship:: calculate_movement()
{
	// Compute values for how much we need to move, and how much we can, and how long we can,
	// given the fuel state, then decide what to do.
	double time = 1.0;	// "full step" time
	// get the distance to destination
	double destination_distance = cartesian_distance(get_location(), destination);
	// get full step distance we can move on this time step
	double full_distance = track.get_speed() * time;
	// get fuel required for full step distance
	double full_fuel_required = full_distance * fuel_consumption;	// tons = nm * tons/nm
	// how far and how long can we sail in this time period based on the fuel state?
	double distance_possible, time_possible;
	if(full_fuel_required <= fuel) {
		distance_possible = full_distance;
		time_possible = time;
		}
	else {
		distance_possible = fuel / fuel_consumption;	// nm = tons / tons/nm
		time_possible = (distance_possible / full_distance) * time;
		}
	
	// are we are moving to a destination, and is the destination within the distance possible?
	if(ship_state == MOVING_TO_POSITION && destination_distance <= distance_possible) {
		// yes, make our new position the destination
		track.set_position(destination);
		// we travel the destination distance, using that much fuel
		double fuel_required = destination_distance * fuel_consumption;
		fuel -= fuel_required;
        Model::get_instance()->notify_fuel(get_name(), fuel); 
		track.set_speed(0.);
        Model::get_instance()->notify_speed(get_name(), track.get_speed());
		ship_state = STOPPED;
		}
	else {
		// go as far as we can, stay in the same movement state
		// simply move for the amount of time possible
		track.update_position(time_possible);
		// have we used up our fuel?
		if(full_fuel_required >= fuel) {
			fuel = 0.0;
            track.set_speed(0.);
			ship_state = DEAD_IN_THE_WATER;
            Model::get_instance()->notify_fuel(get_name(), fuel); 
			}
		else {
			fuel -= full_fuel_required;
            Model::get_instance()->notify_fuel(get_name(), fuel); 
			}
		}
}

