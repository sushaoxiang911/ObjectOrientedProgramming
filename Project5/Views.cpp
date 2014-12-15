#include "Views.h"
#include "Utility.h"
#include "Navigation.h"
#include "Geometry.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

using std::for_each;
using std::pair; using std::map; using std::vector;
using std::cout; using std::endl;
using std::setw; using std::setprecision; using std::ios;
using std::string;

map_view::map_view()
    :size(25), scale(2.0), origin(-10, -10)
{}

void map_view::draw()
{
    vector<vector<string> > view_map(size);
    for (int i = 0; i < size; ++i) {
        view_map[i].resize(size);
        for (int j = 0; j < size; ++j)
            view_map[i][j] = ". ";
    }
    cout << "Display size: " << size << ", scale: " << scale << ", origin: " << origin << endl;
    vector<string>  outsider;
    for (auto it = location_record.begin(); it != location_record.end(); ++it) {
        int ix; int iy;
        if (get_subscripts(ix, iy, it->second)) {
            int real_x = convert(iy);
            int real_y = ix;
            if (view_map[real_x][real_y] != ". ")
                view_map[real_x][real_y] = "* ";
            else
                view_map[real_x][real_y] = (it->first).substr(0, 2);
        } else {
            outsider.push_back(it->first);
        }
    }
    if (!outsider.empty()) {
        cout << outsider[0];
        for_each(outsider.begin()+1, outsider.end(), [](string& elem){cout << ", " << elem;});
        cout << " outside the map" << endl;
    }
    for (int i = 0; i < size; ++i) {
        if (convert(i) % 3 == 0) {
            double label = convert(i)*scale + origin.y;
            cout << setw(4) << setprecision(0) << label;
        } else 
            cout << "    ";
        cout << " ";
        for_each(view_map[i].begin(), view_map[i].end(), 
                    [](string& elem){cout << elem;});
        cout << endl;
    }
    for (int i = 0; i < size; ++i) {
        if (i % 3 == 0) {
            double label = i*scale + origin.x;
            cout << setw(6) << setprecision(0) << label;
        }
    }
    cout << endl;

    // set the cout setting back
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
}

void map_view::clear()
{
    location_record.clear();
}
void map_view::update_location(const string& name, Point location)
{
    location_record[name] = location;
}

void map_view::update_remove(const string& name)
{
    location_record.erase(name);
}

void map_view::set_size(int size_)
{
    if (size_ > 30)
        throw Error("New map size is too big!");
    if (size_ <= 6)
        throw Error("New map size is too small!");
    size = size_;
}

void map_view::set_scale(double scale_)
{
    if (scale_ <= 0.0)
        throw Error("New map scale must be positive!");
    scale = scale_;
}

void map_view::set_origin(Point origin_)
{
    origin = origin_;
}

void map_view::set_defaults()
{
    set_size(25);
    set_scale(2.0);
    Point point(-10, -10);
    set_origin(point);
}
/* *** Use this function to calculate the subscripts for the cell. */

/* *** This code assumes the specified private member variables. */


// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool map_view::get_subscripts(int &ix, int &iy, Point location)
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will produce integer smaller than even for negative values, 
	// so - 0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

void sailing_view::draw()
{
    cout << "----- Sailing Data -----" << endl;
    cout << setw(10) << "Ship" << setw(10) << "Fuel" 
         << setw(10) << "Course" << setw(10) << "Speed" << endl;;
    for_each(sailing_record.begin(), sailing_record.end(), 
                [](pair<const string, ship_sailing> &elem)
                {
                    cout << setw(10) << elem.first << setw(10) << elem.second.fuel
                         << setw(10) << elem.second.course << setw(10) << elem.second.speed
                         << endl;
                });
     // set the cout setting back
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
}

void sailing_view::clear()
{
    sailing_record.clear();
}

void sailing_view::update_remove(const string& name)
{
    sailing_record.erase(name);
}

void sailing_view::update_speed(const string& name, double speed)
{
    sailing_record[name].speed = speed;
}

void sailing_view::update_course(const string& name, double course)
{
    sailing_record[name].course = course;
}

void sailing_view::update_fuel(const string& name, double fuel)
{
    sailing_record[name].fuel = fuel;
}

bridge_view::bridge_view(string name_)
            :sunk(false), ship_course(0.), ship_name(name_)
{}

void bridge_view::draw()
{
    if (sunk) {
        cout << "Bridge view from " << get_name() << " sunk at " 
             << location_record[get_name()] << endl;
        for (int i = 0; i < 3; ++i) {
            cout << "     ";
            for (int j = 0; j < 19; ++j) {
                cout << "w-";
            }
            cout << endl;
        }
    } else {
        vector<string> bridge_sight(19, ". ");
        Point self_position = location_record[get_name()];
        cout << "Bridge view from " << get_name() << " position "
             << self_position << " heading " << ship_course << endl;
        for (auto it = location_record.begin(); it != location_record.end(); ++it) {
            Compass_position bearing_range(self_position, it->second);
            if (bearing_range.range < 0.05 || bearing_range.range > 20)
                continue;
            double angle = bearing_range.bearing - ship_course;
            if (angle < -180)
                angle += 360.0;
            else if (angle > 180)
                angle -= 360.0;
            int subscript = (int)floor((angle+90.0)/10.0);
            if (subscript < 0 || subscript >= 19)
                continue;
            if (bridge_sight[subscript] == ". ")
                bridge_sight[subscript] = (it->first).substr(0, 2);
            else 
                bridge_sight[subscript] = "**";
        }
        for (int i = 0; i < 2; ++i) {
            cout << "     ";
            for (int j = 0; j < 19; ++j) {
                cout << ". ";
            }
            cout << endl;
        }
        cout << "     ";
        for (int i = 0; i < 19; ++i)
            cout << bridge_sight[i];
        cout << endl; 
    }

    for (int i = -90; i <= 90; i = i+30)
        cout << setw(6) << i;
    cout << endl;
    // set the cout setting back
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
}

void bridge_view::clear()
{
    location_record.clear();
}

void bridge_view::update_location(const string& name, Point location)
{
    location_record[name] = location;
}

void bridge_view::update_remove(const string& name)
{
    if (name == get_name())
        sunk = true;
    else
        location_record.erase(name);
}

void bridge_view::update_course (const string& name, double course)
{
    if (name == get_name())
        ship_course = course;
}
