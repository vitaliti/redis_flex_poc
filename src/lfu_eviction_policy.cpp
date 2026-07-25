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

void LfuEvictionPolicy::updateEvictionCandidate(const std::string& key, Entry& entry, const bool isInserted)
{
    if (isInserted) 
    {
        m_minFrequency = 1;
    }
    else
    {
        remove(entry);
    }

    entry.m_frequency += 1;
    auto [bucketIt, inserted] = m_lfu.try_emplace(entry.m_frequency);
    bucketIt->second.push_back(key);
    entry.m_lfuIt = std::prev(bucketIt->second.end());
    updateMinFrequency();
}

void LfuEvictionPolicy::remove(const Entry& entry)
{
    auto bucketIt = m_lfu.find(entry.m_frequency);
    if(bucketIt != m_lfu.end())
    {
        bucketIt->second.erase(entry.m_lfuIt);
        if (bucketIt->second.empty())
        {
            uint32_t removedFrequency = entry.m_frequency;
            m_lfu.erase(bucketIt);
            if (removedFrequency == m_minFrequency)
            {
                updateMinFrequency();
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

void LfuEvictionPolicy::updateMinFrequency()
{
    if (m_lfu.empty())
    {
        m_minFrequency = 0;
    }
    else
    {
        m_minFrequency = m_lfu.begin()->first;
    }
}
