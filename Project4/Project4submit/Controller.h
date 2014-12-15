 /* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <string>

class Model;
class View;
class Ship;
class Island;


class Controller;
typedef void (Controller::* ship_command_fun)(Ship*);
typedef std::map<std::string, ship_command_fun> ship_command_map_t;

typedef void (Controller::* general_command_fun) ();
typedef std::map<std::string, general_command_fun>general_command_map_t;

class Controller {
public:
	// output constructor message
	Controller();
	// output destructor message
	~Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();
	

private:
    View* view_ptr; // the View that the controller is controlling
    
    // the ship_command_map_t that stores all the ship commands
    ship_command_map_t ship_command_map; 
    // the general_command_map_ that stores all other commands
    general_command_map_t general_command_map;
    
    // ship command functions
    void set_course(Ship* ship_ptr);
    void set_position(Ship* ship_position);
    void set_destination(Ship* ship_ptr);
    void set_load(Ship* ship_ptr);
    void set_unload(Ship* ship_ptr);
    void set_dock(Ship* ship_ptr);
    void set_attack(Ship* ship_ptr);
    void set_refuel(Ship* ship_ptr);
    void set_stop(Ship* ship_ptr);
    void set_stopattack(Ship* ship_ptr);

    // general command functions
    void set_default();
    void set_size();
    void set_zoom();
    void set_pan();
    void imple_show();
    void get_status();
    void imple_go();
    void imple_create();

    // helper functions
    void read_to_newline();
    double read_double();
    int read_int();
    double read_speed();
    Island* read_island();
};

#endif
