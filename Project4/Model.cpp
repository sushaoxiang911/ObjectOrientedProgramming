#include "Model.h"
#include "Sim_object.h"
#include "Island.h"
#include "Ship.h"
#include "View.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <algorithm>
#include <functional>
#include <iostream>
using std::pair;
using std::map;
using std::vector;
using std::string;
using std::for_each;
using std::cout; using std::endl;
/* create some islands and ships using the following code in the Model constructor.
Do not change the execution order of these code fragments. You should delete this comment. */

Model* g_Model_ptr = NULL;

Model::Model()
        :time(0)
{
    Island* temp_island;
	temp_island = new Island("Exxon", Point(10, 10), 1000, 200);
    insert_island(temp_island);
	temp_island = new Island("Shell", Point(0, 30), 1000, 200);
    insert_island(temp_island);
	temp_island = new Island("Bermuda", Point(20, 20));
    insert_island(temp_island);
	
    Ship* temp_ship;
	temp_ship = create_ship("Ajax", "Cruiser", Point (15, 15));
    insert_ship(temp_ship);
	temp_ship = create_ship("Xerxes", "Cruiser", Point (25, 25));
    insert_ship(temp_ship);
	temp_ship = create_ship("Valdez", "Tanker", Point (30, 30));
    insert_ship(temp_ship);
	cout << "Model constructed" << endl;
}

Model::~Model()
{
    for_each(object_container.begin(), object_container.end(),
                [](pair<const string, Sim_object*>& elem){delete elem.second;});
    object_container.clear();
    island_container.clear();
    ship_container.clear();
    cout << "Model destructed" << endl;
}

bool Model::is_name_in_use(const string& name) const
{
    for (auto it = object_container.begin(); it != object_container.end(); ++it) {
        if (name.substr(0, 2) == (it->first).substr(0, 2))
            return true;
    }
    return false;
}

bool Model::is_island_present(const string& name) const
{
    auto it = island_container.find(name);
    return (it != island_container.end());
}

void Model::add_island(Island* island_ptr)
{
    insert_island(island_ptr);
    island_ptr->broadcast_current_state();
}

void Model::insert_island(Island* island_ptr)
{
    string name = island_ptr->get_name();
    object_container.insert(pair<string, Sim_object*>(name, island_ptr));
    island_container.insert(pair<string, Island*>(name, island_ptr));

}

Island* Model::get_island_ptr(const string& name) const
{
    auto it = island_container.find(name);
    if (it == island_container.end())
        throw Error("Island not found!");
    return it->second;
}

bool Model::is_ship_present(const string& name) const
{
    auto it = ship_container.find(name);
    return (it != ship_container.end());
}

void Model::add_ship(Ship* ship_ptr)
{
    insert_ship(ship_ptr); 
    ship_ptr->broadcast_current_state();
}

void Model::insert_ship(Ship* ship_ptr)
{
    string name = ship_ptr->get_name();
    object_container.insert(pair<string, Sim_object*>(name, ship_ptr));
    ship_container.insert(pair<string, Ship*>(name, ship_ptr));
}

Ship* Model::get_ship_ptr(const string& name) const
{
    auto it = ship_container.find(name);
    if (it == ship_container.end())
        throw Error("Ship not found!");
    return it->second;
}

void Model::describe() const
{
    for_each(object_container.begin(), object_container.end(), 
                [](const pair<const string, Sim_object*>& elem){(elem.second)->describe();});
}

void Model::update()
{
    ++time;
    for_each(object_container.begin(), object_container.end(), 
                [](pair<const string, Sim_object*>& elem){(elem.second)->update();});
    for (auto it = ship_container.begin(); it != ship_container.end(); ++it) {
        if ((it->second)->is_on_the_bottom()) {
            delete it->second;
            object_container.erase(it->first);
            ship_container.erase(it);
        }
    }

}

void Model::attach(View* view_ptr)
{
    view_container.push_back(view_ptr);
    for_each(object_container.begin(), object_container.end(), 
                [](pair<const string, Sim_object*>& elem)
                    {(elem.second)->broadcast_current_state();});
}

void Model::detach(View* view_ptr)
{
    for(auto it = view_container.begin(); it != view_container.end(); ++it) {
        if (*it == view_ptr) {
            view_container.erase(it);
            break;
        }
    }
}

void Model::notify_location(const std::string& name, Point location)
{
    for_each(view_container.begin(), view_container.end(), 
                [name, location](View* elem){elem->update_location(name, location);});
}

void Model::notify_gone(const string& name)
{
    for_each(view_container.begin(), view_container.end(),
                [name](View* elem){elem->update_remove(name);});
}

