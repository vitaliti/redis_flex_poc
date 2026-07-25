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
    virtual void remove(const Entry& entry) = 0;
    virtual void updateEvictionCandidate(const std::string& key, Entry& entry, const bool isInserted) = 0;
    virtual void print() const = 0;
};

#endif
