#ifndef i_cache_hpp
#define i_cache_hpp

#include "i_key_value_storage.hpp"

#include <optional>
#include <string>
#include <list>

using KeyValuePair = std::pair<std::string, std::string>;

class ICache : public IKeyValueStorage
{
public:
    ~ICache() = default;
    virtual bool isWithinMaxCapacity(const std::string& key, const std::string& value) = 0;
    virtual bool hasCapacityFor(const std::string& key, const std::string& value) = 0;
    virtual bool isEmpty() = 0;
    virtual std::optional<KeyValuePair> getEvictionCandidate() = 0;
};
#endif