#include "ram_cache.hpp"

#include <rocksdb/iterator.h>

RamCache::RamCache(IEvictionPolicy& evictionPolicy, const int maxRamData)
    : m_evictionPolicy{evictionPolicy}, m_maxRamData{maxRamData}, m_RamDataCounter{0}
{
    m_cache.reserve(m_maxRamData);
}

void RamCache::put(const std::string& key, const std::string& value)
{
    auto [it, inserted] = m_cache.try_emplace(key, value);
    if(inserted)
    {
        m_RamDataCounter++;
    }
    else
    {
        it->second.value = value;
    }
    m_evictionPolicy.updateEvictionCandidate(it, inserted);
}

std::optional<std::string> RamCache::get(const std::string& key)
{
    if(isKeyInCache(key))
    {
        auto it = m_cache.find(key);
        m_evictionPolicy.updateEvictionCandidate(it, false);
        return it->second.value;
    }
    return std::nullopt;
}

void RamCache::remove(const std::string& key)
{
    auto it = m_cache.find(key);
    if (it != m_cache.end()) 
    {
        m_evictionPolicy.remove(it);
        m_cache.erase(it);
        m_RamDataCounter--;
    }
}

void RamCache::print() const
{
    m_evictionPolicy.print();
}

bool RamCache::isKeyInCache(const std::string& key)
{
    return m_cache.find(key) != m_cache.end();
}

bool RamCache::isCacheFull()
{
    return (m_RamDataCounter == m_maxRamData);
}

std::optional<KeyValuePair> RamCache::getEvictionCandidate()
{
    std::optional<std::string> key = m_evictionPolicy.getEvictionCandidate();
    if(key)
    {
        auto it = m_cache.find(*key);
        KeyValuePair result{it->first, *it->second.value};

        return result;
    }
    return std::nullopt;
}
