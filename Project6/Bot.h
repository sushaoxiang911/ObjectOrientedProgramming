#ifndef BOT_H
#define BOT_H

#include "Participant.h"

class Bot : public Participant {
public:
    Bot(std::string name_) : Participant(name_) {}
    
    void init() override;

    bool run() override;
    
private:
    std::string random_string_gen();
};

#endif
