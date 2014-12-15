#include "Bot.h"
#include "Model.h"
#include <cstdlib>
#include <memory>

using std::string;
using std::shared_ptr;

const int random_char_start = 97;
const int random_char_range = 26;

void Bot::init() 
{
    loading_island_name = random_string_gen();
    shared_ptr<Island> new_island(new Island(loading_island_name, Point(10, 10), 1000, 200));
    Model::get_instance().add_island(new_island);

    unloading_island_name = random_string_gen();
    new_island.reset(new Island(unloading_island_name, Point(50, 50), 1000, 200));
    Model::get_instance().add_island(new_island);
}

bool Bot::run() {
    if (Model::get_instance().get_time() == 0) {
        ship_name = random_string_gen();
        shared_ptr<Ship> new_ship = create_ship(ship_name, "Tanker", Point(40, 40));
        Model::get_instance().add_ship(new_ship);
        new_ship->set_load_destination()
    }
}

string Bot::random_string_gen()
{
    string name(5);
    do {
        name.clear();
        for (int i = 0; i < 5; ++i)
            name[i] = random_char_start + rand()%random_char_range;
    } while (Model::get_instance().is_name_in_use(name));
    return name;
}
