#include "Record.h"
#include "String.h"
#include "Utility.h"
#include <iostream>
#include <fstream>

int Record::ID_counter = 0;
int Record::ID_counter_backup = 0;

Record::Record(const String& medium_, const String& title_)
        :title(title_), ID(++ID_counter), medium(medium_), rating(0)
{}

Record::Record(const String& title_)
        :title(title_), ID(0), medium(""), rating(0)
{}

Record::Record(int ID_)
        :title(""), ID(ID_), medium(""), rating(0)
{}

Record::Record(std::ifstream& is)
{
    is >> ID >> medium >> rating;
    if (!is) {
        throw Error("Invalid data found in file!");
    }
    is.get();
    getline(is, title);
    if (ID > ID_counter)
        ID_counter = ID;
}

void Record::set_rating(int rating_)
{
    if (rating_ < 1 || rating_ > 5) {
        throw Error("Rating is out of range!");
    }
    rating = rating_;
}

void Record::save(std::ostream& os) const
{
    os << ID << ' ' << medium << ' ' << rating << ' ' << title << std::endl;
}

std::ostream& operator<< (std::ostream& os, const Record& record)
{
    if (record.rating)
        os << record.ID << ": " << record.medium << ' ' << record.rating << ' ' << record.title << std::endl;
    else 
        os << record.ID << ": " << record.medium << ' ' << 'u' << ' ' << record.title << std::endl;
    return os;
}
