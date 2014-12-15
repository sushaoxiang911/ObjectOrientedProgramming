#ifndef UTILITY_H
#define UTILITY_H
/* Utility functions, constants, and classes used by more than one other modules */

#include "Record.h"

// a simple class for error exceptions - msg points to a C-string error message
struct Error {
	Error(const char* msg_ = "") :
		msg(msg_)
		{}
	const char* msg;
};
// a simpel class for error exceptions after read a title - msg points to a C-string error message
struct Error_title {
    Error_title(const char* msg_ = "") :
        msg(msg_)
        {}
    const char* msg;
};

// a struct that compare two record pointers based on their title
struct comp_record_ptr_title {
    bool operator()(const Record* record1, const Record* record2) const
        {return *record1 < *record2;}
};

// extern const to declare the minimum rating number
extern const int rating_min;
// extern const to declare the maximum rating number
extern const int rating_max;

#endif
