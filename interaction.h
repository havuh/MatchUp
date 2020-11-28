#ifndef INTERACTION_H
#define INTERACTION_H


#include <iostream>
#include "date.h"

using std::string;

class Interaction
{
private:
    Date date;
    bool shared;

public:
    Interaction() {}
    Interaction(Date date, bool shared) : date(date), shared(shared) {}
    ~Interaction() {}

    void setDate(Date date)     { this->date = date         ; }
    void setShared(bool shared) { this->shared = shared     ; }

    Date& getDate()              { return date       ; }
    bool getShared()            { return shared     ; }
};

#endif // INTERACTION_H
