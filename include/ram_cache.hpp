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
    RamCache(IEvictionPolicy& evictionPolicy, const int maxRamData);
    ~RamCache() = default;

    void put(const std::string& key, const std::string& value) override;
    std::optional<std::string> get(const std::string& key) override;
    void remove(const std::string& key) override;
    void print() const override;

    bool isKeyInCache(const std::string& key) override;
    bool isCacheFull() override;
    std::optional<KeyValuePair> getEvictionCandidate() override;
private:
    IEvictionPolicy& m_evictionPolicy;
    int m_maxRamData;
    int m_RamDataCounter;
    std::unordered_map<std::string, Entry> m_cache;
};
#endif