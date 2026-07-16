#ifndef lru_eviction_policy_hpp
#define lru_eviction_policy_hpp

#include "i_eviction_policy.hpp"

#include "rocksdb/db.h"

#include <optional>
#include <string>

class LruEvictionPolicy : public IEvictionPolicy
{
public:
    ~LruEvictionPolicy() = default;
    std::optional<std::string> getEvictionCandidate() override;
    void remove(std::unordered_map<std::string, Entry>::iterator it) override;
    void updateEvictionCandidate(std::unordered_map<std::string, Entry>::iterator it, const bool isInserted) override;
    void print() const override;
private:
    std::list<std::string> m_lru;
};
#endif