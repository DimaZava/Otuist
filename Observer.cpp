#include "Observer.h"

void Subject::Attach(Observer& o)
{
    observers.push_back(&o);
}

void Subject::Detach(Observer& o)
{
    observers.erase(std::remove(observers.begin(), observers.end(), &o));
}

void Subject::Notify()
{
    for (auto* o : observers)
    {
        o->Update(*this);
    }
}
