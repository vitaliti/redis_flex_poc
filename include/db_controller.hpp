#ifndef db_controller_hpp
#define db_controller_hpp

#include "rocksdb/db.h"
#include "speedb_flash_storage.hpp"

#include <optional>
#include <string>

struct Entry
{
    std::optional<std::string> value;
    std::list<std::string>::iterator lruIt;
};

class DBController
{
public:
    DBController(SpeedbFlashStorage& flash_db);
    ~DBController() {};

    void Put(const std::string& key, const std::string& value);
    std::optional<std::string> Get(const std::string& key);
    void Delete(const std::string& key);
    // void LoadAll(std::unordered_map<std::string, std::string>& cache);
    void PrintDB() const;
    void PrintCache() const;
private:
    bool isKeyInCache(const std::string& key);
    void updateKeyHotness(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted);
    void removeOldestData();

    SpeedbFlashStorage& m_db;
    const int m_maxRAMdata = 10;
    const int m_maxHotRAMData = 5;
    int m_hotRAMDataCounter = 0;
    std::unordered_map<std::string, Entry> m_cache;
    std::list<std::string> m_lru;
};
#endif