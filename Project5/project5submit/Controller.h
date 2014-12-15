 /* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <string>
#include <memory>
#include <vector>

class Model;
class View;
class Ship;
class Island;
class map_view;
class sailing_view;

class Controller;
typedef void (Controller::* ship_command_fun)(std::shared_ptr<Ship>);
typedef std::map<std::string, ship_command_fun> ship_command_map_t;
typedef void (Controller::* general_command_fun) ();
typedef std::map<std::string, general_command_fun>general_command_map_t;


class Controller {
public:
    // construct a controller with preset 
    Controller();
	// run the program by acccepting user commands, then destroy View object
	void run();

private:
    
    // the pointer of map_view
    std::shared_ptr<map_view> map_view_ptr; 
    // the pointer of sailing view
    std::shared_ptr<sailing_view> sailing_view_ptr;
    // the vector which keeps the order of the view objects
    std::vector<std::shared_ptr<View> > view_order;    
    // the ship_command_map_t that stores all the ship commands
    ship_command_map_t ship_command_map; 
    // the general_command_map_ that stores all other commands
    general_command_map_t general_command_map;
    
    // ship command functions
    void set_course(std::shared_ptr<Ship> ship_ptr);
    void set_position(std::shared_ptr<Ship> ship_position);
    void set_destination(std::shared_ptr<Ship> ship_ptr);
    void set_load(std::shared_ptr<Ship> ship_ptr);
    void set_unload(std::shared_ptr<Ship> ship_ptr);
    void set_dock(std::shared_ptr<Ship> ship_ptr);
    void set_attack(std::shared_ptr<Ship> ship_ptr);
    void set_refuel(std::shared_ptr<Ship> ship_ptr);
    void set_stop(std::shared_ptr<Ship> ship_ptr);
    void set_stopattack(std::shared_ptr<Ship> ship_ptr);

    // general command functions
    void set_default();
    void set_size();
    void set_zoom();
    void set_pan();
    void imple_show();
    void get_status();
    void imple_go();
    void imple_create();
    void open_map_view();
    void close_map_view();
    void open_sailing_view();
    void close_sailing_view();
    void open_bridge_view();
    void close_bridge_view();

    // helper functions
    void read_to_newline();
    double read_double();
    int read_int();
    double read_speed();
    std::shared_ptr<Island> read_island();
    void clear_all_view();
};

#endif
