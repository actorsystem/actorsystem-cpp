
// actor.hpp
#pragma once

#include <string>

class Actor {
public:
    Actor(const std::string& name);

    virtual ~Actor();

    const std::string& name() const;

private:
    std::string name_;
};
