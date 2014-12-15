#include "Controller.h"
#include "Model.h"
#include "Views.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>
#include <functional>

using std::string;
using std::cout; using std::endl; using std::cin;
using std::map;
using std::shared_ptr;
using std::for_each; using std::mem_fn;

Controller::Controller()
{
    ship_command_map["course"] = &Controller::set_course;
    ship_command_map["position"] = &Controller::set_position;
    ship_command_map["destination"] = &Controller::set_destination;
    ship_command_map["load_at"] = &Controller::set_load;
    ship_command_map["unload_at"] = &Controller::set_unload;
    ship_command_map["dock_at"] = &Controller::set_dock;
    ship_command_map["attack"] = &Controller::set_attack;
    ship_command_map["refuel"] = &Controller::set_refuel;
    ship_command_map["stop"] = &Controller::set_stop;
    ship_command_map["stop_attack"] = &Controller::set_stopattack;
    
    general_command_map["default"] = &Controller::set_default;    
    general_command_map["size"] = &Controller::set_size;    
    general_command_map["zoom"] = &Controller::set_zoom;    
    general_command_map["pan"] = &Controller::set_pan;    
    general_command_map["show"] = &Controller::imple_show;    
    general_command_map["status"] = &Controller::get_status;    
    general_command_map["go"] = &Controller::imple_go;    
    general_command_map["create"] = &Controller::imple_create;
    general_command_map["open_map_view"] = &Controller::open_map_view;
    general_command_map["close_map_view"] = &Controller::close_map_view;   
    general_command_map["open_sailing_view"] = &Controller::open_sailing_view;
    general_command_map["close_sailing_view"] = &Controller::close_sailing_view;
    general_command_map["open_bridge_view"] = &Controller::open_bridge_view;
    general_command_map["close_bridge_view"] = &Controller::close_bridge_view;
}
void Controller::run()
{
    while (true) {
        cout << "\nTime " << Model::get_instance()->get_time() << ": Enter command: ";
        string command;
        cin >> command;
        if (command == "quit") {
            clear_all_view();
            cout << "Done" << endl;
            return;
        }
        try {
            if (Model::get_instance()->is_ship_present(command)) {
                shared_ptr<Ship> ship_ptr = Model::get_instance()->get_ship_ptr(command);
                cin >> command;
                auto ship_command_it = ship_command_map.find(command);
                if (ship_command_it == ship_command_map.end())
                    throw Error("Unrecognized command!");
                else
                  (this->*(ship_command_it->second))(ship_ptr);
            } else {
                auto general_command_it = general_command_map.find(command);
                if (general_command_it == general_command_map.end())
                    throw Error("Unrecognized command!");
                else
                   (this->*(general_command_it->second))();
            }
        } catch (Error &except) {
            cin.clear();
            read_to_newline();
            cout << except.what() << endl;
        } catch(std::exception &except) {
            cout << except.what() << endl;
            clear_all_view();
            return;
        }
    }
}


void Controller::set_course(shared_ptr<Ship> ship_ptr)
{
    double compass = read_double();
    if (compass < 0.0 || compass >= 360.0)  
        throw Error("Invalid heading entered!");
    double speed = read_speed();
    ship_ptr->set_course_and_speed(compass, speed);
}

void Controller::set_position(shared_ptr<Ship> ship_ptr)
{
    double x = read_double();
    double y = read_double();
    double speed = read_speed();
    Point destination(x, y);
    ship_ptr->set_destination_position_and_speed(destination, speed);
}

void Controller::set_destination(shared_ptr<Ship> ship_ptr)
{
    shared_ptr<Island> island_ptr = read_island();
    double speed = read_speed();
    ship_ptr->set_destination_position_and_speed(island_ptr->get_location(), speed);
}

void Controller::set_load(shared_ptr<Ship> ship_ptr)
{
    shared_ptr<Island> island_ptr = read_island();
    ship_ptr->set_load_destination(island_ptr);
}

void Controller::set_unload(shared_ptr<Ship> ship_ptr)
{
    shared_ptr<Island> island_ptr = read_island();
    ship_ptr->set_unload_destination(island_ptr);
}

void Controller::set_dock(shared_ptr<Ship> ship_ptr)
{
    shared_ptr<Island> island_ptr = read_island();
    ship_ptr->dock(island_ptr);
}

void Controller::set_attack(shared_ptr<Ship> ship_ptr) 
{
    string ship_name;
    cin >> ship_name;
    shared_ptr<Ship> target_ptr = Model::get_instance()->get_ship_ptr(ship_name);
    ship_ptr->attack(target_ptr);
}

void Controller::set_refuel(shared_ptr<Ship> ship_ptr) 
{
    ship_ptr->refuel();    
}

void Controller::set_stop(shared_ptr<Ship> ship_ptr)
{
    ship_ptr-> stop();
}

void Controller::set_stopattack(shared_ptr<Ship> ship_ptr)
{
    ship_ptr->stop_attack();
}

void Controller::set_default()
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");
    map_view_ptr->set_defaults();
}

void Controller::set_size()
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");
    int size = read_int();
    map_view_ptr->set_size(size);
}

void Controller::set_zoom()
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");
    double scale = read_double();
    map_view_ptr->set_scale(scale);
}

void Controller::set_pan()
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");
    double x = read_double();
    double y = read_double();
    Point origin(x, y);
    map_view_ptr->set_origin(origin);
}

void Controller::imple_show()
{
    for_each(view_order.begin(), view_order.end(), mem_fn(&View::draw));
}

void Controller::get_status()
{
    Model::get_instance()->describe();
}

void Controller::imple_go()
{
    Model::get_instance()->update();
}

void Controller::imple_create()
{
    string ship_name;
    cin >> ship_name;
    if (ship_name.length() < 2)
        throw Error("Name is too short!");
    if (Model::get_instance()->is_name_in_use(ship_name))
        throw Error("Name is already in use!");
    string type_name;
    cin >> type_name;
    double x = read_double();
    double y = read_double();
    Point position(x, y);
    shared_ptr<Ship> new_ship_ptr = create_ship(ship_name, type_name, position);
    Model::get_instance()->add_ship(new_ship_ptr);
}

void Controller::open_map_view()
{
    if (map_view_ptr)
        throw Error("Map view is already open!");
    map_view_ptr.reset(new map_view());
    view_order.push_back(map_view_ptr);
    Model::get_instance()->attach(map_view_ptr);
}

void Controller::close_map_view()
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");
    for (auto it = view_order.begin(); it != view_order.end(); ++it) {
        if (*it == map_view_ptr) 
            view_order.erase(it);
            break;
    }
    Model::get_instance()->detach(map_view_ptr);
    map_view_ptr.reset();
}

void Controller::open_sailing_view()
{
    if (sailing_view_ptr)
        throw Error("Sailing data view is already open!");
    sailing_view_ptr.reset(new sailing_view());
    view_order.push_back(sailing_view_ptr);
    Model::get_instance()->attach(sailing_view_ptr);
}

void Controller::close_sailing_view()
{
    if (!sailing_view_ptr)
        throw Error("Sailing data view is not open!");
    for (auto it = view_order.begin(); it != view_order.end(); ++it) {
        if (*it == sailing_view_ptr) 
            view_order.erase(it);
            break;
    }
    Model::get_instance()->detach(sailing_view_ptr);
    sailing_view_ptr.reset();
}

void Controller::open_bridge_view()
{
    string name;
    cin >> name;
    if (!Model::get_instance()->is_ship_present(name))
        throw Error("Ship not found!");
    for (auto it = view_order.begin(); it != view_order.end(); ++it) {
        if ((*it)->get_name() == name) {
            throw Error("Bridge view is already open for that ship!");
        }
    }
    shared_ptr<View> ptr(new bridge_view(name));
    view_order.push_back(ptr);
    Model::get_instance()->attach(ptr);
}

void Controller::close_bridge_view()
{
    string name;
    cin >> name;
    for (auto it = view_order.begin(); it != view_order.end(); ++it) {
        if ((*it)->get_name() == name) {
            Model::get_instance()->detach(*it);
            view_order.erase(it);
            return;   
        }
    }
    throw Error("Bridge view for that ship is not open!");
}

void Controller::read_to_newline()
{
    while(cin.get() != '\n');
}

double Controller::read_double()
{
    double result;
    if (!(cin >> result))
        throw Error("Expected a double!");
    return result;
}

int Controller::read_int()
{
    int result;
    if (!(cin >> result))
        throw Error("Expected an integer!");
    return result;
}

double Controller::read_speed()
{
    double result = read_double();
    if (result < 0.0)
        throw Error("Negative speed entered!");
    return result;
}

shared_ptr<Island> Controller::read_island()
{
    string island_name;
    cin >> island_name;
    return Model::get_instance()->get_island_ptr(island_name);
}

void Controller::clear_all_view()
{
    for_each(view_order.begin(), view_order.end(), 
        [](shared_ptr<View> &elem)
        {
            Model::get_instance()->detach(elem);
        });
    view_order.clear();
    map_view_ptr.reset();
    sailing_view_ptr.reset();
}

