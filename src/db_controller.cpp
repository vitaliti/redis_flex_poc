#include <rocksdb/iterator.h>

#include "db_controller.hpp"

DBController::DBController(SpeedbFlashStorage& flash_db)
: m_db{flash_db}
{
    m_cache.reserve(m_maxRAMdata);
}

void DBController::Put(const std::string& key, const std::string& value)
{
    if(!isKeyInCache(key))
    {
        if((m_hotRAMDataCounter + 1) > m_maxHotRAMData)
        {
            //Move oldest RAM data to FLASH
            removeOldestData();
        }
        else
        {
            m_hotRAMDataCounter++;
        }
    }
    auto [it, inserted] = m_cache.try_emplace(key);
    it->second.value = value;
    updateKeyHotness(it, inserted);
}

std::optional<std::string> DBController::Get(const std::string& key)
{
    if(isKeyInCache(key))
    {
        auto it = m_cache.find(key);
        return it->second.value;
    }
    else
    {
        std::optional<std::string> value = m_db.Get(key);
        if (value) 
        {
            //Call Put here
            Put(key, *value);
            printf("Taken from DB and put in cache");
            return value;
        }
    }
}

void DBController::Delete(const std::string& key)
{
    m_db.Delete(key);

    auto it = m_cache.find(key);
    if (it != m_cache.end()) 
    {
        m_lru.erase(it->second.lruIt);
        m_cache.erase(it);
        m_hotRAMDataCounter--;
    }
}

void DBController::PrintCache() const
{
    printf("Data in RAM/CACHE \n");
    for (const auto & [ key, value ] : m_cache) {
        printf("Key: %s, ", key.c_str());
        if(value.value)
        {
            printf("Value:%s ", value.value->c_str());
        }
        printf("\n");
    }
}

void DBController::PrintDB() const
{
    m_db.PrintDB();
}

bool DBController::isKeyInCache(const std::string& key)
{
    return m_cache.find(key) != m_cache.end();
}

void DBController::updateKeyHotness(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted)
{
    if (!isInserted) {
        m_lru.erase(it->second.lruIt);        
    }

    m_lru.push_back(it->first);
    it->second.lruIt = std::prev(m_lru.end());
}

void DBController::removeOldestData()
{
    std::string coldestKey = m_lru.front();
    auto it = m_cache.find(coldestKey);
    if (it != m_cache.end()) {
        m_db.Put(coldestKey, *it->second.value);
        m_lru.pop_front();
        m_cache.erase(it);
    }
}