#ifndef CRUISE_SHIP_H
#define CRUISE_SHIP_H

#include "Ship.h"
#include <map>

enum CruiseShipState { NO_CRUISE, REFUEL, SIGHT, SET_COURSE, MOVING };

class Island;

class Cruise_ship : public Ship {
public:
    // constructor
    Cruise_ship(const std::string& name_, Point position_);
    // destructor
    ~Cruise_ship() {}
    
    // override set_destination_position_and_speed
    // if has cruise, cancel current cruise
    // start a new cruise
    void set_destination_position_and_speed(Point destination, double speed) override;
    // override set_course_and_speed
    // if has cruise, cancel current cruise
    void set_course_and_speed(double course, double speed) override;
    // override stop
    // if has cruise, cancel current cruise
    void stop() override;
    // override update
    void update() override;
    // override describe
    void describe() const override;

private:
    // the current state of cruise ship
    CruiseShipState cruise_ship_state;
    // the speed obtained when starting a new cruise
    double cruise_speed;
    // record the start island of the cruise
    std::shared_ptr<Island> start_island;
    // the next destination on the cruise
    std::shared_ptr<Island> next_destination;
    // the remaining islands that will be visited
    std::map<std::string, std::shared_ptr<Island> > remain_island;

    // find the nearest island from remain_island and set it as the next destination
    std::shared_ptr<Island> find_next();
    // start a new cruise, set start_island, and obtain island map from Model
    void start_cruise(std::shared_ptr<Island> island_ptr);
    // cancel the current cruise. reset variables
    void cancel_cruise();
};

#endif 
