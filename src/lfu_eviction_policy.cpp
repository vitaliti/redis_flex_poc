#include "lfu_eviction_policy.hpp"

void LfuEvictionPolicy::print() const
{
    printf("Keys in RAM/CACHE \n");
    for (const auto& [frequency, keys] : m_lfu)
    {
        printf("Frequency:%d \n" , frequency);
        printf("Keys:");
        for (const auto& key : keys)
        {
            printf(" %s", key.c_str());
        }
        printf("\n");
    }
    printf("Min frequency = %u\n", m_minFrequency);
}

void LfuEvictionPolicy::updateEvictionCandidate(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted)
{
    if (isInserted) 
    {
        m_minFrequency = 1;
    }
    else
    {
        remove(it);
    }

    it->second.m_frequency += 1;
    auto [bucketIt, inserted] = m_lfu.try_emplace(it->second.m_frequency);
    bucketIt->second.push_back(it->first);
    it->second.m_lfuIt = std::prev(bucketIt->second.end());
}

void LfuEvictionPolicy::remove(std::unordered_map<std::string, Entry>::iterator it)
{
    auto bucketIt = m_lfu.find(it->second.m_frequency);
    if(bucketIt != m_lfu.end())
    {
        bucketIt->second.erase(it->second.m_lfuIt);
        if (bucketIt->second.empty())
        {
            uint32_t removedFrequency = it->second.m_frequency;
            m_lfu.erase(bucketIt);
            if (removedFrequency == m_minFrequency)
            {
                m_minFrequency++;
            }
        }
    }
}

std::optional<std::string> LfuEvictionPolicy::getEvictionCandidate()
{
    if (!m_lfu.empty())
    {
        auto bucketIt = m_lfu.find(m_minFrequency);
        const auto& bucket = bucketIt->second;
        return bucket.front();
    }

    return std::nullopt;
}