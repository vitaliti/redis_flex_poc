#ifndef lru_eviction_policy_hpp
#define lru_eviction_policy_hpp

#include "i_eviction_policy.hpp"

#include <optional>
#include <string>

class LruEvictionPolicy : public IEvictionPolicy
{
public:
    ~LruEvictionPolicy() = default;
    std::optional<std::string> getEvictionCandidate() override;
    void remove(const Entry& entry) override;
    void updateEvictionCandidate(const std::string& key, Entry& entry, const bool isInserted) override;
    void print() const override;
private:
    std::list<std::string> m_lru;
};
#endif