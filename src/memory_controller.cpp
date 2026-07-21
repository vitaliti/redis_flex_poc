#include "memory_controller.hpp"

MemoryController::MemoryController(IKeyValueStorage& flashStorage, ICache& ramCache)
    : m_flashStorage{flashStorage}, m_ramCache{ramCache}
{
}

void MemoryController::put(const std::string& key, const std::string& value)
{
    if(!m_ramCache.isWithinMaxCapacity(key,value))
    {
        printf("NO ROOM IN CACHE! \n");
        return;
    }

    while(!m_ramCache.hasCapacityFor(key, value) && !m_ramCache.isEmpty())
    {
        transferOldestDataToFlash();
    }

    m_ramCache.put(key, value);
}

std::optional<std::string> MemoryController::get(const std::string& key)
{
    auto ramValue = m_ramCache.get(key);
    if(ramValue)
    {
        return ramValue;
    }
    else
    {
        std::optional<std::string> value = m_flashStorage.get(key);
        if (value) 
        {
            put(key, *value);
            printf("Taken from DB and put in cache \n");
            return value;
        }
    }
    return std::nullopt;
}

void MemoryController::remove(const std::string& key)
{
    m_flashStorage.remove(key);
    m_ramCache.remove(key);
}

void MemoryController::printCache() const
{
    m_ramCache.print();
}

void MemoryController::printDB() const
{
    m_flashStorage.print();
}

void MemoryController::transferOldestDataToFlash()
{
    auto keyValuePair = m_ramCache.getEvictionCandidate();
    if(keyValuePair)
    {
        m_ramCache.remove(keyValuePair->first);
        m_flashStorage.put(keyValuePair->first, keyValuePair->second);
    }
}
