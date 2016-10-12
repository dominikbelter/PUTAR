
#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "Defs/defs.h"
#include <vector>
#include <list>
class Observer
{
public:
    virtual void update(const putar::Mat34& objectState) = 0;
};

class Subject
{
    //Lets keep a track of all the shops we have observing
    std::vector<Observer*> list;

public:
    void attach(Observer *observer);
    void detach(Observer *observer);
    void notify(const putar::Mat34& objectState);
};

#endif // OBSERVER_H_
