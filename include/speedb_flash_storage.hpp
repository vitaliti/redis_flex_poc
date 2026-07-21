#ifndef speedb_flash_storage_hpp
#define speedb_flash_storage_hpp

#include "i_key_value_storage.hpp"

#include "rocksdb/db.h"

#include <optional>
#include <string>

class SpeedbFlashStorage : public IKeyValueStorage
{
public:
    SpeedbFlashStorage(const std::string dbName);
    ~SpeedbFlashStorage() = default;

    void put(const std::string& key, const std::string& value) override;
    std::optional<std::string> get(const std::string& key) override;
    void remove(const std::string& key) override;
    void print() const override;
private:
    std::unique_ptr<rocksdb::DB> m_db;
};
#endif
