#include "Cruise_ship.h"
#include "Island.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>

using std::cout; using std::endl;
using std::string;
using std::map; using std::pair;
using std::shared_ptr;
using std::for_each;

Cruise_ship::Cruise_ship(const string& name_, Point position_)
            :Ship(name_, position_, 500.0, 15.0, 2.0, 0), 
            cruise_ship_state(NO_CRUISE), cruise_speed(0.)
{}

void Cruise_ship::set_destination_position_and_speed(Point destination, double speed) 
{
    if (cruise_ship_state != NO_CRUISE) 
        cancel_cruise();
    Ship::set_destination_position_and_speed(destination, speed);
    shared_ptr<Island> island_ptr = Model::get_instance()->is_island_at_position(destination);
    if (island_ptr) {
        start_cruise(island_ptr);
        cruise_speed = speed;
    }
}

void Cruise_ship::set_course_and_speed(double course, double speed)
{
    if (cruise_ship_state != NO_CRUISE)
        cancel_cruise();
    Ship::set_course_and_speed(course, speed);
}

void Cruise_ship::stop()
{
    if (cruise_ship_state != NO_CRUISE)
        cancel_cruise();
    Ship::stop();
}

void Cruise_ship::update()
{
    Ship::update();
    // if cannot move, just return, keeps state
    if (!can_move()) 
        return;

    switch(cruise_ship_state) {
        case NO_CRUISE:
            break;
        case MOVING:
            if (can_dock(next_destination)) {
                dock(next_destination);
                // if remain island is empty
                // means we have return the start island and end the cruise there
                if (remain_island.empty()) {
                    cruise_ship_state = NO_CRUISE;
                    cout << get_name() << " cruise is over at " 
                         << next_destination->get_name() << endl;
                // if remain island is not empty
                // set state to REFUEL
                } else {
                    cruise_ship_state = REFUEL;
                    remain_island.erase(next_destination->get_name());
                }
            }
            break;
        case REFUEL:
            Ship::refuel();
            cruise_ship_state = SIGHT;
            break;
        case SIGHT:
            cruise_ship_state = SET_COURSE;
            break;
        default:
            next_destination = find_next();
            if (!next_destination)
                next_destination = start_island;
            Ship::set_destination_position_and_speed(next_destination->get_location(),
                    cruise_speed);
            cout << get_name() << " will visit " << next_destination->get_name() << endl;
            cruise_ship_state = MOVING;
            break;
    }
}

void Cruise_ship::describe() const
{
    cout << "\nCruise_ship ";
    Ship::describe();
    switch(cruise_ship_state) {
        case MOVING:
            cout << "On cruise to " << next_destination->get_name() << endl;
            break;
        case NO_CRUISE:
            break;
        default:
            cout << "Waiting during cruise at " << get_docked_Island()->get_name() << endl;
    }
}

shared_ptr<Island> Cruise_ship::find_next()
{
    shared_ptr<Island> result;
    double min_distance = -1.0;
    // find the nearest island from the remain island set
    for (auto it = remain_island.begin(); it != remain_island.end(); ++it) {
        double current_distance = 
            cartesian_distance((it->second)->get_location(), get_location());
        if (min_distance < 0 || current_distance < min_distance) {
            min_distance = current_distance;
            result = it->second;
        }    
    }
    return result;
}

void Cruise_ship::start_cruise(shared_ptr<Island> island_ptr)
{
    remain_island = Model::get_instance()->get_Island_container();
    start_island = island_ptr;
    next_destination = island_ptr;
    cout << get_name() << " will visit " << island_ptr->get_name() << endl;
    cout << get_name() << " cruise will start and end at " << island_ptr->get_name() << endl;
    cruise_ship_state = MOVING;
}

void Cruise_ship::cancel_cruise()
{
    cout << get_name() << " canceling current cruise" << endl; 
    start_island.reset();
    next_destination.reset();
    remain_island.clear();
    cruise_ship_state = NO_CRUISE;
}
