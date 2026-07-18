#ifndef lfu_eviction_policy_hpp
#define lfu_eviction_policy_hpp

#include "i_eviction_policy.hpp"

#include <optional>
#include <string>

class LfuEvictionPolicy : public IEvictionPolicy
{
public:
    ~LfuEvictionPolicy() = default;
    std::optional<std::string> getEvictionCandidate() override;
    void remove(std::unordered_map<std::string, Entry>::iterator it) override;
    void updateEvictionCandidate(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted) override;
    void print() const override;
private:
    // m_minFrequency is valid only when the cache is full.
    // When a new key is added the m_minFrequency will become 1
    // Eviction only happens when cache is full, so this is OK
    uint32_t  m_minFrequency{0};  
    std::unordered_map<uint32_t, std::list<std::string>> m_lfu;
};
#endif