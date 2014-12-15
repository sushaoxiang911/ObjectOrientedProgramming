#ifndef VIEW_H
#define VIEW_H

#include <string>

struct Point;
/* An iterface of all the views. It contains all the functions of all the views concrete
   class. It is a flat interface
*/
class View {
public:
    // destructor
    virtual ~View() {}
    // draw the view
    virtual void draw() = 0;
    // clear the view
    virtual void clear() = 0;

    // update the location of an Sim_object 
    virtual void update_location(const std::string& name, Point location) = 0;
    // update if the ship is removed
    virtual void update_remove(const std::string& name) = 0;
    // update the ship speed of ship_sailing
    virtual void update_speed(const std::string& name, double speed) = 0;
    // update the ship course of ship_sailing
    virtual void update_course(const std::string& name, double course) = 0;
    // update the ship fuel of ship_sailing
    virtual void update_fuel(const std::string& name, double fuel) = 0;
    // get the name that the view is belonged to
    virtual std::string get_name() = 0;
};
#endif
