#include "speedb_flash_storage.hpp"

SpeedbFlashStorage::SpeedbFlashStorage(const std::string dbName)
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
        printf("StatusError: %s \n", status.ToString().c_str());
    }

    m_db.reset(raw_db);
}

void SpeedbFlashStorage::put(const std::string& key, const std::string& value)
{
    rocksdb::Status status = m_db->Put(rocksdb::WriteOptions(), key, value);
    if (!status.ok()) 
    {
        printf("StatusError: %s \n", status.ToString().c_str());
    }
}

std::optional<std::string> SpeedbFlashStorage::get(const std::string& key)
{
    std::string value;
    rocksdb::Status status = m_db->Get(rocksdb::ReadOptions(), key, &value);
    if (status.ok()) 
    {
        return value;
    }
    else
    {
        printf("StatusError: %s \n", status.ToString().c_str());
        return std::nullopt;
    }
}

void SpeedbFlashStorage::remove(const std::string& key)
{
    rocksdb::Status status = m_db->Delete(rocksdb::WriteOptions(), key);

    if (!status.ok()) 
    {
        printf("StatusError: %s \n", status.ToString().c_str());
    }
}

void SpeedbFlashStorage::print() const
{
    printf("Data in FLASH/DB \n");
    rocksdb::ReadOptions read_options;
    std::unique_ptr<rocksdb::Iterator> it(m_db->NewIterator(read_options));

    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        printf("Key:%s Value:%s \n", it->key().ToString().c_str(), it->value().ToString().c_str());
    }

    if (!it->status().ok()) {
        printf("StatusError: %s \n", it->status().ToString().c_str());
    }
}
