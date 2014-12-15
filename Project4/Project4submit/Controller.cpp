#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <iostream>

using std::string;
using std::cout; using std::endl; using std::cin;
using std::map;

Controller::Controller()
{
    cout << "Controller constructed" << endl;

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
}

Controller::~Controller()
{
    cout << "Controller destructed" << endl;
}

void Controller::run()
{
    view_ptr = new View();
    g_Model_ptr->attach(view_ptr);
    while (true) {
        cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
        string command;
        cin >> command;
        if (command == "quit") {
            g_Model_ptr->detach(view_ptr);
            delete view_ptr;
            cout << "Done" << endl;
            return;
        }
        try {
            if (g_Model_ptr->is_ship_present(command)) {
                Ship* ship_ptr = g_Model_ptr->get_ship_ptr(command);
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
            g_Model_ptr->detach(view_ptr);
            delete view_ptr;
            return;
        }
    }
}


void Controller::set_course(Ship* ship_ptr)
{
    double compass = read_double();
    if (compass < 0.0 || compass >= 360.0)  
        throw Error("Invalid heading entered!");
    double speed = read_speed();
    ship_ptr->set_course_and_speed(compass, speed);
}

void Controller::set_position(Ship* ship_ptr)
{
    double x = read_double();
    double y = read_double();
    double speed = read_speed();
    Point destination(x, y);
    ship_ptr->set_destination_position_and_speed(destination, speed);
}

void Controller::set_destination(Ship* ship_ptr)
{
    Island* island_ptr = read_island();
    double speed = read_speed();
    ship_ptr->set_destination_position_and_speed(island_ptr->get_location(), speed);
}

void Controller::set_load(Ship* ship_ptr)
{
    Island* island_ptr = read_island();
    ship_ptr->set_load_destination(island_ptr);
}

void Controller::set_unload(Ship* ship_ptr)
{
    Island* island_ptr = read_island();
    ship_ptr->set_unload_destination(island_ptr);
}

void Controller::set_dock(Ship* ship_ptr)
{
    Island* island_ptr = read_island();
    ship_ptr->dock(island_ptr);
}

void Controller::set_attack(Ship* ship_ptr) 
{
    string ship_name;
    cin >> ship_name;
    Ship* target_ptr = g_Model_ptr->get_ship_ptr(ship_name);
    ship_ptr->attack(target_ptr);
}

void Controller::set_refuel(Ship* ship_ptr) 
{
    ship_ptr->refuel();    
}

void Controller::set_stop(Ship* ship_ptr)
{
    ship_ptr-> stop();
}

void Controller::set_stopattack(Ship* ship_ptr)
{
    ship_ptr->stop_attack();
}

void Controller::set_default()
{
    view_ptr->set_defaults();
}

void Controller::set_size()
{   
    int size = read_int();
    view_ptr->set_size(size);
}

void Controller::set_zoom()
{
    double scale = read_double();
    view_ptr->set_scale(scale);
}

void Controller::set_pan()
{
    double x = read_double();
    double y = read_double();
    Point origin(x, y);
    view_ptr->set_origin(origin);
}

void Controller::imple_show()
{
    view_ptr->draw();
}

void Controller::get_status()
{
    g_Model_ptr->describe();
}

void Controller::imple_go()
{
    g_Model_ptr->update();
}

void Controller::imple_create()
{
    string ship_name;
    cin >> ship_name;
    if (ship_name.length() < 2)
        throw Error("Name is too short!");
    if (g_Model_ptr->is_name_in_use(ship_name))
        throw Error("Name is already in use!");
    string type_name;
    cin >> type_name;
    double x = read_double();
    double y = read_double();
    Point position(x, y);
    Ship* new_ship_ptr = create_ship(ship_name, type_name, position);
    g_Model_ptr->add_ship(new_ship_ptr);
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

Island* Controller::read_island()
{
    string island_name;
    cin >> island_name;
    return g_Model_ptr->get_island_ptr(island_name);
}

