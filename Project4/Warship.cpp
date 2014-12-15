#include "Warship.h"
#include "Utility.h"
#include <iostream>

using std::cout; using std::endl;

Warship::Warship(const std::string& name_, Point position_, double fuel_capacity_, 
            double maximum_speed_, double fuel_consumption_, int resistance_,
            int firepower_, double maximum_range_)
            : Ship(name_, position_, fuel_capacity_, maximum_speed_, fuel_consumption_, 
                resistance_), 
              firepower(firepower_), maximum_range(maximum_range_), attacking(false),
              target_ptr(NULL)
{
    cout << "Warship " << get_name() << " constructed" << endl;
}

Warship::~Warship()
{
    cout << "Warship " << get_name() << " destructed" << endl;
}

void Warship::update()
{
    Ship::update();
    if (!is_attacking())
        return;
    if (!is_afloat() || !get_target()->is_afloat())
        stop_attack();
    else 
        cout << get_name() << " is attacking " << endl;
}

void Warship::attack(Ship* target_ptr_)
{
    if (!is_afloat())
        throw Error("Cannot attack!");
    if (target_ptr_ == this)
        throw Error("Warship may not attack itself!");
    if (get_target() == target_ptr_)
        throw Error("Already attacking this target!");
    target_ptr = target_ptr_;
    attacking = true;
    cout << get_name() << " will attack " << get_target()->get_name() << endl;
}

void Warship::stop_attack()
{
    if (!attacking)
        throw Error("Was not attacking!");
    cout << get_name() << " stopping attack" << endl;
    attacking = false;
    target_ptr = NULL;
}

void Warship::describe() const
{
    Ship::describe();
    if (attacking)
        cout << "Attacking " << get_target()->get_name() << endl;
}

bool Warship::is_attacking() const
{
    return attacking;
}

void Warship::fire_at_target()
{
    cout << get_name() << " fires" << endl;
    get_target()->receive_hit(firepower, this);
}

bool Warship::target_in_range() const
{
    return (cartesian_distance(get_location(), get_target()->get_location()) < maximum_range);
}

Ship* Warship::get_target() const
{
    return target_ptr;
}

