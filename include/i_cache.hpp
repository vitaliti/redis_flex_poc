#ifndef i_cache_hpp
#define i_cache_hpp

#include <optional>
#include <string>
#include "i_key_value_storage.hpp"

struct Entry
{
    std::optional<std::string> value;
    std::list<std::string>::iterator lruIt;
};

class ICache : public IKeyValueStorage
{
public:
    ~ICache() = default;

    virtual bool isKeyInCache(const std::string& key) = 0;
    virtual bool isCacheFull() = 0;
    virtual std::optional<std::pair<std::string, Entry>> removeOldestData() = 0;
};
#endif