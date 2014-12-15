#include "Island.h"
#include "Model.h"
#include <iostream>

using std::cout; using std::endl;

Island::Island(const std::string& name_, Point position_, double fuel_, double production_rate_)
        :Sim_object(name_), position(position_), fuel(fuel_), production_rate(production_rate_)
{}

double Island::provide_fuel(double amount)
{
    double provide_amount = (amount < fuel) ? amount : fuel;
    fuel -= provide_amount;
    cout << "Island " << get_name() << " supplied " << 
            provide_amount << " tons of fuel" << endl;
    return provide_amount;
}

void Island::accept_fuel(double amount)
{
    fuel += amount;
    cout << "Island " << get_name() << " now has " << fuel << " tons" << endl;
}

void Island::update()
{
    if (production_rate > 0) {
        fuel += production_rate;
        cout << "Island " << get_name() << " now has " << fuel << " tons" << endl;
    }       
}

void Island::describe() const 
{
    cout << "\nIsland " << get_name() << " at position " << position << endl;
    cout << "Fuel available: " << fuel << " tons" << endl;
}

void Island::broadcast_current_state()
{
    Model::get_instance()->notify_location(get_name(), get_location());    
}
