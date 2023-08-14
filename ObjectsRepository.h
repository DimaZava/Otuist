#ifndef OBJECTSREPOSITORY_H
#define OBJECTSREPOSITORY_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <type_traits>

#include "NamedObjectInterface.h"

template <
    typename T,
    typename = typename std::enable_if_t<std::is_base_of_v<NamedObjectInterface, typename T::element_type>>>
class ObjectsRepository
{
protected:
    // Create
    void addObject(const T& obj)
    {
        objects.insert(obj);
    }

    // Read all
    std::set<T> readObjects() const
    {
        return objects;
    }

    // Read
    std::optional<T> readObject(const std::string& name) const
    {
        auto findClosure = [&name](const T& obj) {
            return obj->getName() == name;
        };
        auto it = std::find_if(objects.begin(), objects.end(), findClosure);

        if (it != objects.end())
        {
            return *it;
        }
        return std::nullopt;
    }

    // Update if shared_ptr
    template <typename U = T, typename std::enable_if_t<std::is_same_v<U, std::shared_ptr<typename U::element_type>>>>
    void updateObject(const std::string& name, const U& newValue)
    {
        // auto it = std::find_if(objects.begin(), objects.end(), [&name](const T& obj) {
        //     return obj->getName() == name;
        // });

        // if (it != objects.end())
        // {
        //     *it = newValue;
        // }
    }

    // Update for plain types
    template <typename U = T, typename std::enable_if_t<!std::is_same_v<U, std::shared_ptr<typename U::element_type>>>>
    void updateObject(const std::string& name, const U& newValue)
    {
        auto it = std::find_if(objects.begin(), objects.end(), [&name](const T& obj) {
            return obj->getName() == name;
        });

        if (it != objects.end())
        {
            *it = newValue;
        }
    }

    // Delete
    void deleteObject(const std::string& name)
    {
        auto it = std::find_if(objects.begin(), objects.end(), [&name](const T& obj) {
            return obj->getName() == name;
        });

        if (it != objects.end())
        {
            objects.erase(it);
        }
    }

private:
    std::set<T> objects;
};

#endif // OBJECTSREPOSITORY_H
