#ifndef ram_cache_hpp
#define ram_cache_hpp

#include "i_cache.hpp"
#include "entry.hpp"
#include "i_eviction_policy.hpp"

#include <optional>
#include <string>
class RamCache : public ICache
{
public:
    RamCache(IEvictionPolicy& evictionPolicy);
    ~RamCache() = default;

    void put(const std::string& key, const std::string& value) override;
    std::optional<std::string> get(const std::string& key) override;
    void remove(const std::string& key) override;
    void print() const override;

    bool isWithinMaxCapacity(const std::string& key, const std::string& value) override;
    bool hasCapacityFor(const std::string& key, const std::string& value) override;
    bool isEmpty() override;
    std::optional<KeyValuePair> getEvictionCandidate() override;
private:
    int64_t getKeyValuePredictedSizeInCache(const std::string& key, const std::string& value);

    IEvictionPolicy& m_evictionPolicy;
    int64_t m_freeRamInBytes;
    int64_t m_maxRamInBytes;
    std::unordered_map<std::string, Entry> m_cache;
};
#endif
