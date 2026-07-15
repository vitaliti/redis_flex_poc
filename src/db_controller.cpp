#include <rocksdb/iterator.h>

#include "db_controller.hpp"

DBController::DBController(SpeedbFlashStorage& flashDb, RamCache& ramCache)
    : m_db{flashDb}, m_ramCache{ramCache}
{
    // m_ramCache.reserve(m_maxRAMdata);
}

void DBController::Put(const std::string& key, const std::string& value)
{
    if(!m_ramCache.isKeyInCache(key) && m_ramCache.isCacheFull())
    {
        //Move oldest RAM data to FLASH
        removeOldestData();
    }
    m_ramCache.Put(key, value);
}

std::optional<std::string> DBController::Get(const std::string& key)
{
    auto ramValue = m_ramCache.Get(key);
    if(ramValue)
    {
        return ramValue;
    }
    else
    {
        std::optional<std::string> value = m_db.Get(key);
        if (value) 
        {
            Put(key, *value);
            printf("Taken from DB and put in cache");
            return value;
        }
    }
    return std::nullopt;
}

void DBController::Delete(const std::string& key)
{
    m_db.Delete(key);
    m_ramCache.Delete(key);
}

void DBController::PrintCache() const
{
    m_ramCache.PrintCache();
}

void DBController::PrintDB() const
{
    m_db.PrintDB();
}

void DBController::removeOldestData()
{
    auto keyValuePair = m_ramCache.removeOldestData();
    if(keyValuePair)
    {
        m_db.Put(keyValuePair->first, *keyValuePair->second.value);
    }
}