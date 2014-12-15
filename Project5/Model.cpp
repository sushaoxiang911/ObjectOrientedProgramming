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
using std::shared_ptr;
/* create some islands and ships using the following code in the Model constructor.
Do not change the execution order of these code fragments. You should delete this comment. */

static Model_destroyer destroyer;

Model* Model::model_ptr = 0;

Model::Model()
        :time(0)
{
    shared_ptr<Island> temp_island(new Island("Exxon", Point(10, 10), 1000, 200));
    insert_island(temp_island);
	temp_island.reset(new Island("Shell", Point(0, 30), 1000, 200));
    insert_island(temp_island);
	temp_island.reset(new Island("Bermuda", Point(20, 20)));
    insert_island(temp_island);
    temp_island.reset(new Island("Treasure_Island", Point(50, 5), 100, 5));
    insert_island(temp_island);
	
    shared_ptr<Ship> temp_ship;
	temp_ship = create_ship("Ajax", "Cruiser", Point (15, 15));
    insert_ship(temp_ship);
	temp_ship = create_ship("Xerxes", "Cruiser", Point (25, 25));
    insert_ship(temp_ship);
	temp_ship = create_ship("Valdez", "Tanker", Point (30, 30));
    insert_ship(temp_ship);
}

Model::~Model()
{
    // no need to delete here since container has the ownership
    // And clear means the contained pointers are destroyed
   object_container.clear();
    island_container.clear();
    ship_container.clear();
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

void Model::add_island(shared_ptr<Island> island_ptr)
{
    insert_island(island_ptr);
    island_ptr->broadcast_current_state();
}

void Model::insert_island(shared_ptr<Island> island_ptr)
{
    string name = island_ptr->get_name();
    object_container.insert(pair<string, shared_ptr<Sim_object> >(name, island_ptr));
    island_container.insert(pair<string, shared_ptr<Island> >(name, island_ptr));

}

shared_ptr<Island> Model::get_island_ptr(const string& name) const
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

void Model::add_ship(shared_ptr<Ship> ship_ptr)
{
    insert_ship(ship_ptr); 
    ship_ptr->broadcast_current_state();
}

void Model::insert_ship(shared_ptr<Ship> ship_ptr)
{
    string name = ship_ptr->get_name();
    object_container.insert(pair<string, shared_ptr<Sim_object> >(name, ship_ptr));
    ship_container.insert(pair<string, shared_ptr<Ship> >(name, ship_ptr));
}

shared_ptr<Ship> Model::get_ship_ptr(const string& name) const
{
    auto it = ship_container.find(name);
    if (it == ship_container.end())
        throw Error("Ship not found!");
    return it->second;
}

void Model::describe() const
{
    for_each(object_container.begin(), object_container.end(), 
                [](const pair<const string, shared_ptr<Sim_object> >& elem)
                    {(elem.second)->describe();});
}

void Model::update()
{
    ++time;
    for_each(object_container.begin(), object_container.end(), 
                [](pair<const string, shared_ptr<Sim_object> >& elem)
                    {(elem.second)->update();});
}

void Model::attach(shared_ptr<View> view_ptr)
{
    view_container.push_back(view_ptr);
    for_each(object_container.begin(), object_container.end(), 
                [](pair<const string, shared_ptr<Sim_object> >& elem)
                    {(elem.second)->broadcast_current_state();});
}

void Model::detach(shared_ptr<View> view_ptr)
{
    for(auto it = view_container.begin(); it != view_container.end(); ++it) {
        if (*it == view_ptr) {
            view_container.erase(it);
            break;
        }
    }
}

void Model::notify_location(const string& name, Point location)
{
    for_each(view_container.begin(), view_container.end(), 
                [name, location](shared_ptr<View> elem)
                    {elem->update_location(name, location);});
}

void Model::notify_gone(const string& name)
{
    for_each(view_container.begin(), view_container.end(),
                [name](shared_ptr<View> elem)
                    {elem->update_remove(name);});
}

void Model::notify_speed(const string& name, double speed)
{
    for_each(view_container.begin(), view_container.end(),
                [name, speed](shared_ptr<View> elem)
                    {elem->update_speed(name, speed);});
}
void Model::notify_course(const string& name, double course)
{
    for_each(view_container.begin(), view_container.end(),
                [name, course](shared_ptr<View> elem)
                    {elem->update_course(name, course);});
}

void Model::notify_fuel(const string& name, double fuel)
{
    for_each(view_container.begin(), view_container.end(),
                [name, fuel](shared_ptr<View> elem)
                    {elem->update_fuel(name, fuel);});
}

void Model::remove_ship(shared_ptr<Ship> ship_ptr)
{
    object_container.erase(ship_ptr->get_name());
    ship_container.erase(ship_ptr->get_name());
}

shared_ptr<Island> Model::is_island_at_position(Point position)
{
    for(auto it = island_container.begin(); it != island_container.end(); ++it) {
        if ((it->second)->get_location() == position)
            return it->second;    
    }
    return nullptr;
}
