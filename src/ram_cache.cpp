#include "ram_cache.hpp"

#include <rocksdb/iterator.h>

RamCache::RamCache(const int maxRamData)
    : m_maxRamData{maxRamData}, m_RamDataCounter{0}
{
    m_cache.reserve(m_maxRamData);
}

void RamCache::put(const std::string& key, const std::string& value)
{
    auto [it, inserted] = m_cache.try_emplace(key);
    it->second.value = value;
    if(inserted)
    {
        m_RamDataCounter++;
    }
    moveToMostRecentlyUsed(it, inserted);
}

std::optional<std::string> RamCache::get(const std::string& key)
{
    if(isKeyInCache(key))
    {
        auto it = m_cache.find(key);
        moveToMostRecentlyUsed(it, false);
        return it->second.value;
    }
    return std::nullopt;
}

void RamCache::remove(const std::string& key)
{
    auto it = m_cache.find(key);
    if (it != m_cache.end()) 
    {
        m_lru.erase(it->second.lruIt);
        m_cache.erase(it);
        m_RamDataCounter--;
    }
}

void RamCache::print() const
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

bool RamCache::isKeyInCache(const std::string& key)
{
    return m_cache.find(key) != m_cache.end();
}

bool RamCache::isCacheFull()
{
    return (m_RamDataCounter == m_maxRamData);
}

void RamCache::moveToMostRecentlyUsed(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted)
{
    if (!isInserted) {
        m_lru.erase(it->second.lruIt);        
    }

    m_lru.push_back(it->first);
    it->second.lruIt = std::prev(m_lru.end());
}

std::optional<std::pair<std::string, Entry>> RamCache::removeOldestData()
{
    if (m_lru.empty())
    {
        return std::nullopt;
    }

    auto it = getOldestData();
    auto result = *it;

    m_lru.pop_front();
    m_cache.erase(it);
    m_RamDataCounter--;

    return result;
}

std::unordered_map<std::string, Entry>::iterator RamCache::getOldestData()
{
    return m_cache.find(m_lru.front());
}