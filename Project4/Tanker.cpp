#include "Tanker.h"
#include "Ship.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>
#include <string>

using std::cout; using std::endl;

Tanker::Tanker(const std::string& name_, Point position_)
        :Ship(name_, position_, 100.0, 10.0, 2.0, 0), cargo_capacity(1000.0), cargo(0.),
        tanker_state(NO_CARGO_DESTINATIONS), load_destination(NULL), unload_destination(NULL)
{
    cout << "Tanker " << get_name() << " constructed" << endl;
}

Tanker::~Tanker()
{
    cout << "Tanker " << get_name() << " destructed" << endl;
}

void Tanker::set_destination_position_and_speed(Point destination, double speed)
{
    if (tanker_state != NO_CARGO_DESTINATIONS)
        throw Error("Tanker has cargo destinations!");
    Ship::set_destination_position_and_speed(destination, speed);
}

void Tanker::set_course_and_speed(double course, double speed)
{
    if (tanker_state != NO_CARGO_DESTINATIONS)
        throw Error("Tanker has cargo destinations!");
    Ship::set_course_and_speed(course, speed);
}

void Tanker::set_load_destination(Island* island_ptr)
{
    if (tanker_state != NO_CARGO_DESTINATIONS)
        throw Error("Tanker has cargo destinations!");
    load_destination = island_ptr;
    if (load_destination == unload_destination)
        throw Error("Load and unload cargo destinations are the same!");
    cout << get_name() << " will load at " << island_ptr->get_name() << endl;
    set_destination_helper();
}

void Tanker::set_unload_destination(Island* island_ptr)
{
    if (tanker_state != NO_CARGO_DESTINATIONS)
        throw Error("Tanker has cargo destinations!");
    unload_destination = island_ptr;
    if (unload_destination == load_destination)
        throw Error("Load and unload cargo destinations are the same!");
    cout << get_name() << " will unload at " << island_ptr->get_name() << endl;
    set_destination_helper();
}

void Tanker::stop()
{
    Ship::stop();
    clear_destinations();
    cout << get_name() << " now has no cargo destinations" << endl;
}

void Tanker::update()
{
    Ship::update();
    if (!can_move()) {
        clear_destinations();
        cout << get_name() << " now has no cargo destinations" << endl;
    }
    
    
    switch (tanker_state) {
        case NO_CARGO_DESTINATIONS:
            return;
        
        case UNLOADING:
            if (cargo == 0.0) {
                Ship::set_destination_position_and_speed(load_destination->get_location(),
                            get_maximum_speed());
                tanker_state = MOVING_TO_LOADING;
            } else {
                unload_destination->accept_fuel(cargo);
                cargo = 0.0;
            }
            break;
        case MOVING_TO_LOADING:
            if (can_dock(load_destination)) {
                dock(load_destination);
                tanker_state = LOADING;
            }
            break;
        case LOADING:
        {
            Ship::refuel();
            double cargo_fuel_required = cargo_capacity - cargo;
            if (cargo_fuel_required < 0.005) {
                cargo = cargo_capacity;
                Ship::set_destination_position_and_speed(unload_destination->get_location(),
                            get_maximum_speed());
                tanker_state = MOVING_TO_UNLOADING;
            } else {
                double cargo_fuel_provided = load_destination->provide_fuel(cargo_fuel_required);
                cargo += cargo_fuel_provided;
                std::cout << get_name() << " now has " << cargo << " of cargo" << std::endl;
            }
            break;
        }
        default:
            if (can_dock(unload_destination)) {
                dock(unload_destination);
                tanker_state = UNLOADING;
            }
            break;
    }
}

void Tanker::describe() const
{
    cout << "\nTanker ";
    Ship::describe();
    cout << "Cargo: " << cargo << " tons";
    switch (tanker_state) {
        case NO_CARGO_DESTINATIONS:
            cout << ", no cargo destinations" << endl;
            break;
        case LOADING:
            cout << ", loading" << endl;
            break;
        case UNLOADING:
            cout << ", unloading" << endl;
            break;
        case MOVING_TO_LOADING:
            cout << ", moving to loading destination" << endl;
            break;
        case MOVING_TO_UNLOADING:
            cout << ", moving to unloading destination" << endl;
            break;
    }
}

void Tanker::set_destination_helper()
{   
    if (load_destination && unload_destination) {
        if (is_docked()) {
            if (get_docked_Island() == load_destination) {
                tanker_state = LOADING;
                return;
            }
            if (get_docked_Island() == unload_destination) {
                tanker_state = UNLOADING;
                return;
            }
        }

        if (!is_moving()) {
            if (!cargo && can_dock(load_destination)) {
                tanker_state = LOADING;
                return;
            }
            if (cargo > 0 && can_dock(unload_destination)) {
                tanker_state = UNLOADING;
                return;
            }
        }

        if (!cargo) {
            Ship::set_destination_position_and_speed(load_destination->get_location(), 
                      get_maximum_speed());
            tanker_state = MOVING_TO_LOADING;
            return;
        }

        if (cargo > 0) {
            Ship::set_destination_position_and_speed(unload_destination->get_location(), 
                        get_maximum_speed());
            tanker_state = MOVING_TO_UNLOADING;
            return;
        }
    }
}

void Tanker::clear_destinations()
{
    tanker_state = NO_CARGO_DESTINATIONS;
    load_destination = NULL;
    unload_destination = NULL;
}

