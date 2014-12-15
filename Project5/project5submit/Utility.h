#ifndef UTILITIES_H
#define UTILITIES_H
#include <exception>

// error class
class Error : public std::exception {
public:
	Error(const char* msg_) : msg(msg_) {}
    const char* what() const noexcept override
        {return msg;}
private:
	const char* msg;
};

// this structure is used in sailing view
struct ship_sailing {
	double fuel;
	double course;
	double speed;
};

#endif
