#include <rocksdb/iterator.h>

#include "db_controller.hpp"

DBController::DBController(const std::string dbName)
{
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::DB* raw_db = nullptr;

    rocksdb::Status status = rocksdb::DB::Open(
        options,
        dbName,
        &raw_db
    );

    if (!status.ok()) {
    throw std::runtime_error(status.ToString());
    }

    m_db.reset(raw_db);
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
        std::string value;
        rocksdb::Status status = m_db->Get(rocksdb::ReadOptions(), key, &value);
        if (status.ok()) 
        {
            //Call Put here
            Put(key, value);
            printf(" Taken from DB and put in cache");
            return value;
        }
        else
        {
            printf("StatusError: %s \n", status.ToString().c_str());
            return std::nullopt;
        }
    }
}

void DBController::Delete(const std::string& key)
{
    rocksdb::Status status = m_db->Delete(rocksdb::WriteOptions(), key);

    if (!status.ok()) 
    {
        printf("StatusError: %s \n", status.ToString().c_str());
    }

    auto it = m_cache.find(key);
    if (it != m_cache.end()) 
    {
        m_lru.erase(it->second.lruIt);
        m_cache.erase(it);
        m_hotRAMDataCounter--;
    }
}

void DBController::PrintDB() const
{
    printf("Data in FLASH/DB \n");
    rocksdb::ReadOptions read_options;
    std::unique_ptr<rocksdb::Iterator> it(m_db->NewIterator(read_options));

    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        printf("Key:%s Value:%s \n", it->key().ToString().c_str(), it->value().ToString().c_str());
    }

    if (!it->status().ok()) {
        throw std::runtime_error(it->status().ToString());
    }
    printf("___________________________\n");  
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
    printf("___________________________\n");
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
        m_db->Put(rocksdb::WriteOptions(), coldestKey, *it->second.value);
        m_lru.pop_front();
        m_cache.erase(it);
    }
}