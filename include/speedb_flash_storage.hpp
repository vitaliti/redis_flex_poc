#ifndef speedb_flash_storage_hpp
#define speedb_flash_storage_hpp

#include "rocksdb/db.h"
#include <optional>
#include <string>

class SpeedbFlashStorage
{
public:
    SpeedbFlashStorage(const std::string dbName);
    ~SpeedbFlashStorage() {};

    void Put(const std::string& key, const std::string& value);
    std::optional<std::string> Get(const std::string& key);
    void Delete(const std::string& key);
    void PrintDB() const;
private:
    std::unique_ptr<rocksdb::DB> m_db;
};
#endif