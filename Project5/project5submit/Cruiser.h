/*
A Cruiser is a Warship with moderate speed, firepower, and resistance.
When told to attack it will start firing at the target if it is in range.
At each update, it will stop attacking if the target is either no longer afloat
(i.e. is sinking or sunk), or is out of range. As long as the target is both afloat
and in range, it will keep firing at it.

Initial values:
fuel capacity and initial amount: 1000, maximum speed 20., fuel consumption 10.tons/nm, 
resistance 6, firepower 3, maximum attacking range 15
*/

#ifndef CRUISER_H
#define CRUISER_H

#include "Warship.h"
#include <memory>

class Cruiser : public Warship {
public:
	Cruiser(const std::string& name_, Point position_);
	~Cruiser() {}

    // override update()
	void update() override;
    // override describe()
	void describe() const override;
    // override receive_hit for Cruiser
    void receive_hit(int hit_force, std::shared_ptr<Ship> attacker_ptr) override;
};

#endif
