#ifndef i_eviction_policy_hpp
#define i_eviction_policy_hpp

#include <optional>
#include <string>
#include <unordered_map>

#include "entry.hpp"

class IEvictionPolicy
{
public:
    virtual std::optional<std::string> getEvictionCandidate() = 0;
    virtual void remove(std::unordered_map<std::string, Entry>::iterator it) = 0;
    virtual void updateEvictionCandidate(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted) = 0;
    virtual void print() const = 0;
};

#endif
