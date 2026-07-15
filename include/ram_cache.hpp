#ifndef ram_cache_hpp
#define ram_cache_hpp

#include "rocksdb/db.h"
#include "speedb_flash_storage.hpp"

#include <optional>
#include <string>

struct Entry
{
    std::optional<std::string> value;
    std::list<std::string>::iterator lruIt;
};

class RamCache
{
public:
    RamCache();
    ~RamCache() {};

    void Put(const std::string& key, const std::string& value);
    std::optional<std::string> Get(const std::string& key);
    void Delete(const std::string& key);
    void PrintCache() const;
    bool isKeyInCache(const std::string& key);
    bool isCacheFull();
    void updateKeyHotness(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted);
    std::optional<std::pair<std::string, Entry>> removeOldestData();
private:
    std::unordered_map<std::string, Entry>::iterator getOldestData();

    const int m_maxRAMdata = 10;
    const int m_maxHotRAMData = 5;
    int m_hotRAMDataCounter = 0;
    std::unordered_map<std::string, Entry> m_cache;
    std::list<std::string> m_lru;
};
#endif