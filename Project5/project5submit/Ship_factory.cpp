#include "Ship_factory.h"
#include "Ship.h"
#include "Tanker.h"
#include "Warship.h"
#include "Cruiser.h"
#include "Cruise_ship.h"
#include "Utility.h"

using std::shared_ptr;

shared_ptr<Ship> create_ship(const std::string& name, const std::string& type, Point initial_position)
{
		shared_ptr<Ship> new_ship = nullptr;
        if (type == "Tanker") {
        	new_ship.reset(new Tanker(name, initial_position));
            return new_ship;
        } else if (type == "Cruiser") {
     		new_ship.reset(new Cruiser(name, initial_position));
            return new_ship;
        } else if (type == "Cruise_ship") {
            new_ship.reset(new Cruise_ship(name, initial_position));
            return new_ship;
        } else 
            throw Error("Trying to create ship of unknown type!");
            
}
