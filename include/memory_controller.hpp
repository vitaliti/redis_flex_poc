#ifndef memory_controller_hpp
#define memory_controller_hpp

#include "speedb_flash_storage.hpp"
#include "ram_cache.hpp"

#include "rocksdb/db.h"

#include <optional>
#include <string>

class MemoryController
{
public:
    MemoryController(IKeyValueStorage& flashStorage, ICache& ramCache);
    ~MemoryController() = default;

    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    void remove(const std::string& key);
    void printDB() const;
    void printCache() const;
private:
    void transferOldestDataToFlash();

    IKeyValueStorage& m_flashStorage;
    ICache& m_ramCache;
};
#endif