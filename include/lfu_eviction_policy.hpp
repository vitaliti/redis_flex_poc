#ifndef lfu_eviction_policy_hpp
#define lfu_eviction_policy_hpp

#include "i_eviction_policy.hpp"

#include <optional>
#include <string>
#include <map>

class LfuEvictionPolicy : public IEvictionPolicy
{
public:
    ~LfuEvictionPolicy() = default;
    std::optional<std::string> getEvictionCandidate() override;
    void remove(const Entry& entry) override;
    void updateEvictionCandidate(const std::string& key, Entry& entry, const bool isInserted) override;
    void print() const override;
private:
    void LfuEvictionPolicy::updateMinFrequency();
    uint32_t  m_minFrequency{0};
    std::map<uint32_t, std::list<std::string>> m_lfu;
};
#endif