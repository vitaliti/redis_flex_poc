#include "ram_cache.hpp"
#include <cinttypes>

RamCache::RamCache(IEvictionPolicy& evictionPolicy, const uint32_t ramSizeInBytes)
    : m_evictionPolicy{evictionPolicy}, m_freeRamInBytes{ramSizeInBytes}, m_maxRamInBytes{ramSizeInBytes}
{
}

void RamCache::put(const std::string& key, const std::string& value)
{
    auto [it, inserted] = m_cache.try_emplace(key, value);
    if(inserted)
    {
        int64_t keySize = key.size() + sizeof(std::string);
        int64_t entrySize = value.size() + sizeof(Entry);
        m_freeRamInBytes = m_freeRamInBytes - (keySize + entrySize);
        printf("Free Memory:%lld Added:%lld \n", m_freeRamInBytes, (keySize + entrySize));
    }
    else
    {
        int64_t newValueSize = value.size();
        int64_t oldValueSize =  it->second.m_value->size();
        m_freeRamInBytes = m_freeRamInBytes - (newValueSize - oldValueSize);
        it->second.m_value = value;
        printf("Free Memory:%lld Added:%lld \n", m_freeRamInBytes, (newValueSize - oldValueSize));
    }
    m_evictionPolicy.updateEvictionCandidate(it, inserted);
}

std::optional<std::string> RamCache::get(const std::string& key)
{
    auto it = m_cache.find(key);
    if(it != m_cache.end())
    {
        m_evictionPolicy.updateEvictionCandidate(it, false);
        printf("Free Memory:%lld Item:%lld \n", m_freeRamInBytes, (int64_t)(it->second.getSize() + it->first.size() + sizeof(std::string)));
        return it->second.m_value;
    }
    return std::nullopt;
}

void RamCache::remove(const std::string& key)
{
    auto it = m_cache.find(key);
    if (it != m_cache.end()) 
    {
        int64_t keySize = it->first.size() + sizeof(std::string);
        int64_t entrySize = it->second.getSize();
        m_freeRamInBytes = m_freeRamInBytes + (keySize + entrySize);
        printf("Free Memory:%lld Removed:%lld \n", m_freeRamInBytes, (keySize + entrySize));

        m_evictionPolicy.remove(it);
        m_cache.erase(it);
    }
}

void RamCache::print() const
{
    printf("Free Memory:%lld Max Memory:%lld \n", m_freeRamInBytes, m_maxRamInBytes);
    m_evictionPolicy.print();
}

bool RamCache::isWithinMaxCapacity(const std::string& key, const std::string& value)
{
    int64_t newKeyPairSize = getKeyValuePredictedSizeInCache(key, value);
    return ((m_maxRamInBytes - newKeyPairSize) >= 0);
}
bool RamCache::hasCapacityFor(const std::string& key, const std::string& value)
{
    int64_t newKeyPairSize = getKeyValuePredictedSizeInCache(key, value);
    return ((m_freeRamInBytes - newKeyPairSize) >= 0);
}

bool RamCache::isEmpty()
{
    return m_cache.empty();
}

std::optional<KeyValuePair> RamCache::getEvictionCandidate()
{
    std::optional<std::string> key = m_evictionPolicy.getEvictionCandidate();
    if(key)
    {
        auto it = m_cache.find(*key);
        KeyValuePair result{it->first, *it->second.m_value};
        return result;
    }
    return std::nullopt;
}

int64_t RamCache::getKeyValuePredictedSizeInCache(const std::string& key, const std::string& value)
{
    int64_t result = 0;
    auto it = m_cache.find(key);
    if(it != m_cache.end())
    {
        int64_t oldValueSize = it->second.m_value ? it->second.m_value->size() : 0;
        int64_t newValueSize = value.size();
        int64_t byteDelta = newValueSize - oldValueSize;
        result = byteDelta;
    }
    else
    {
        int64_t keySize = key.size() + sizeof(std::string);
        int64_t entrySize = value.size() + sizeof(Entry);
        int64_t newKeyPairSize = keySize + entrySize;
        result = newKeyPairSize;
    }
    return result;
}