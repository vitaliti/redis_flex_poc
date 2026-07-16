#include "lru_eviction_policy.hpp"

void LruEvictionPolicy::print() const
{
    printf("Keys in RAM/CACHE \n");
    for (const auto& key : m_lru)
    {
        printf("Key:%s \n", key.c_str());
    }
}

void LruEvictionPolicy::updateEvictionCandidate(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted)
{
    if (!isInserted) {
        m_lru.erase(it->second.lruIt);        
    }

    m_lru.push_back(it->first);
    it->second.lruIt = std::prev(m_lru.end());
}

void LruEvictionPolicy::remove(std::unordered_map<std::string, Entry>::iterator it)
{
    m_lru.erase(it->second.lruIt);
}

std::optional<std::string> LruEvictionPolicy::getEvictionCandidate()
{
    if (m_lru.empty())
    {
        return std::nullopt;
    }

    return m_lru.front();
}