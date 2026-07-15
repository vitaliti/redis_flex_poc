#ifndef db_controller_hpp
#define db_controller_hpp

#include "rocksdb/db.h"
#include "speedb_flash_storage.hpp"
#include "ram_cache.hpp"

#include <optional>
#include <string>

class DBController
{
public:
    DBController(SpeedbFlashStorage& flashDb, RamCache& ramCache);
    ~DBController() {};

    void Put(const std::string& key, const std::string& value);
    std::optional<std::string> Get(const std::string& key);
    void Delete(const std::string& key);
    void PrintDB() const;
    void PrintCache() const;
private:
    void removeOldestData();

    SpeedbFlashStorage& m_db;
    RamCache& m_ramCache;
};
#endif