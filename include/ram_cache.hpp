#ifndef ram_cache_hpp
#define ram_cache_hpp

#include "i_cache.hpp"

#include "rocksdb/db.h"

#include <optional>
#include <string>
class RamCache : public ICache
{
public:
    RamCache(const int maxRamData);
    ~RamCache() = default;

    void put(const std::string& key, const std::string& value) override;
    std::optional<std::string> get(const std::string& key) override;
    void remove(const std::string& key) override;
    void print() const override;

    bool isKeyInCache(const std::string& key) override;
    bool isCacheFull() override;
    std::optional<KeyValuePair> removeOldestData() override;
private:
    struct Entry
    {
        Entry(std::string value)
            : value(std::move(value))
        {
        }

        std::optional<std::string> value;
        std::list<std::string>::iterator lruIt;
    };

    void moveToMostRecentlyUsed(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted);
    std::unordered_map<std::string, Entry>::iterator getOldestData();

    int m_maxRamData;
    int m_RamDataCounter;
    std::unordered_map<std::string, Entry> m_cache;
    std::list<std::string> m_lru;
};
#endif