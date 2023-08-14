#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <vector>

class Subject;

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void update(Subject&) = 0;
};

class Subject
{
public:
    virtual ~Subject() = default;
    void attach(Observer& o);
    void detach(Observer& o);
    void notify();

private:
    std::vector<Observer*> observers;
};

#endif // OBSERVER_H
