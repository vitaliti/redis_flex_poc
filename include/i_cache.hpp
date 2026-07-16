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

    virtual bool isKeyInCache(const std::string& key) = 0;
    virtual bool isCacheFull() = 0;
    virtual std::optional<KeyValuePair> removeOldestData() = 0;
};
#endif