#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <vector>

template <typename T>
class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void didChange(const T& value) = 0;
};

template <typename T>
class ISubject
{
public:
    virtual ~ISubject() = default;

    void addObserver(IObserver<T>* observer)
    {
        observers.push_back(observer);
    }

    void removeObserver(IObserver<T>* observer)
    {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify(const T& value)
    {
        for (IObserver<T>* observer : observers)
        {
            observer->didChange(value);
        }
    }

private:
    std::vector<IObserver<T>*> observers;
};

#endif // OBSERVER_H
