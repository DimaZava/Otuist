#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <vector>

class Subject;

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void Update(Subject&) = 0;
};

class Subject
{
public:
    virtual ~Subject() = default;
    void Attach(Observer& o);
    void Detach(Observer& o);
    void Notify();

private:
    std::vector<Observer*> observers;
};

#endif // OBSERVER_H
