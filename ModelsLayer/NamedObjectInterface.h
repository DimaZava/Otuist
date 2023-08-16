#ifndef NAMEDOBJECTINTERFACE_H
#define NAMEDOBJECTINTERFACE_H

#include <string>

// Virtual interface with a name property
class NamedObjectInterface
{
public:
    virtual ~NamedObjectInterface() = default;

    virtual std::string getName() const = 0;
};

#endif // NAMEDOBJECTINTERFACE_H
