#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <vector>

template <typename T>
class Observer
{
public:
    virtual ~Observer() = default;
    virtual void notify(const T& value) = 0;
};

template <typename T>
class Subject
{
public:
    virtual ~Subject() = default;

    void addObserver(Observer<T>* observer)
    {
        observers.push_back(observer);
    }

    void removeObserver(Observer<T>* observer)
    {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify(const T& value)
    {
        for (Observer<T>* observer : observers)
        {
            observer->notify(value);
        }
    }

private:
    std::vector<Observer<T>*> observers;
};

#endif // OBSERVER_H
