#include "View.h"
#include "Utility.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

using std::for_each;
using std::map; using std::vector;
using std::cout; using std::endl;
using std::setw; using std::setprecision; using std::ios;
using std::string;

View::View()
    :size(25), scale(2.0), origin(-10, -10)
{
    cout << "View constructed" << endl;
}

View::~View()
{
    cout << "View destructed" << endl;
}

void View::update_location(const string& name, Point location)
{
    location_record[name] = location;
}

void View::update_remove(const string& name)
{
    location_record.erase(name);
}

void View::draw()
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

void View::clear()
{
    location_record.clear();
}

void View::set_size(int size_)
{
    if (size_ > 30)
        throw Error("New map size is too big!");
    if (size_ <= 6)
        throw Error("New map size is too small!");
    size = size_;
}

void View::set_scale(double scale_)
{
    if (scale_ <= 0.0)
        throw Error("New map scale must be positive!");
    scale = scale_;
}

void View::set_origin(Point origin_)
{
    origin = origin_;
}

void View::set_defaults()
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
bool View::get_subscripts(int &ix, int &iy, Point location)
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

