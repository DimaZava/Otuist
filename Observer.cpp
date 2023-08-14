#include "Observer.h"

void Subject::attach(Observer& observer)
{
    observers.push_back(&observer);
}

void Subject::detach(Observer& observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), &observer), observers.end());
}

void Subject::notify()
{
    for (const auto& observer : observers)
    {
        observer->update(*this);
    }
}
