// actor.cpp
#include "actor.hpp"

Actor::Actor(const std::string& name) : name_(name) {}

Actor::~Actor() {}

const std::string& Actor::name() const {
    return name_;
}
