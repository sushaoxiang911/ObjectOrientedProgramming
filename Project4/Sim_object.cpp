#include "Sim_object.h"
#include <iostream>
#include <string>

using std::cout; using std::endl;
Sim_object::Sim_object(const std::string& name_)
                        :name(name_)
{
    cout << "Sim_object " << name << " constructed" << endl;
}

Sim_object::~Sim_object()
{
    cout << "Sim_object " << name << " destructed" << endl;
}

