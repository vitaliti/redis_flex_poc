// #include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lru_eviction_policy.hpp"

using namespace testing;

class LruEvictionPolicyTest : public Test {
protected:
    LruEvictionPolicy policy{};
};

TEST_F(LruEvictionPolicyTest, getEmptyEvictionCandidate) {
    auto result = policy.getEvictionCandidate();
    EXPECT_FALSE(result);
}

TEST_F(LruEvictionPolicyTest, getOnlyInsertedEvictionCandidate) {
    std::unordered_map<std::string, Entry> map;

    const std::string key = "testttKEYYY";
    auto it = map.emplace(key, Entry{""}).first;
    bool isInserted = true;

    policy.updateEvictionCandidate(it->first, it->second, isInserted);
    auto result = policy.getEvictionCandidate();
    EXPECT_EQ(key, *result);
}

TEST_F(LruEvictionPolicyTest, getCorrectLRUEvictionCandidate) {
    std::unordered_map<std::string, Entry> map;

    const std::string key1 = "KEY1";
    auto it = map.emplace(key1, Entry{""}).first;
    const std::string key2 = "KEY2";
    auto it2 = map.emplace(key2, Entry{""}).first;

    bool isInserted = true;
    policy.updateEvictionCandidate(it->first, it->second, isInserted);
    policy.updateEvictionCandidate(it2->first, it2->second, isInserted);

    isInserted = false;
    policy.updateEvictionCandidate(it2->first, it2->second, isInserted);
    policy.updateEvictionCandidate(it2->first, it2->second, isInserted);
    auto result = policy.getEvictionCandidate();
    EXPECT_EQ(key1, *result);
}

TEST_F(LruEvictionPolicyTest, removeOnlyEvictionCandidate) {
    std::unordered_map<std::string, Entry> map;

    const std::string key1 = "KEY1";
    auto it = map.emplace(key1, Entry{""}).first;
    bool isInserted = true;
    policy.updateEvictionCandidate(it->first, it->second, isInserted);
    auto result = policy.getEvictionCandidate();
    EXPECT_EQ(key1, *result);

    policy.remove(it->second);
    result = policy.getEvictionCandidate();
    EXPECT_FALSE(result);
}

TEST_F(LruEvictionPolicyTest, removeCorrectLRUEvictionCandidate) {
    std::unordered_map<std::string, Entry> map;

    const std::string key1 = "KEY1";
    auto it = map.emplace(key1, Entry{""}).first;
    const std::string key2 = "KEY2";
    auto it2 = map.emplace(key2, Entry{""}).first;

    bool isInserted = true;
    policy.updateEvictionCandidate(it->first, it->second, isInserted);
    policy.updateEvictionCandidate(it2->first, it2->second, isInserted);
    auto result = policy.getEvictionCandidate();
    EXPECT_EQ(key1, *result);

    policy.remove(it->second);
    result = policy.getEvictionCandidate();
    EXPECT_EQ(key2, *result);
}

TEST_F(LruEvictionPolicyTest, callPrintForCoverage) {
    std::unordered_map<std::string, Entry> map;
    const std::string key1 = "KEY1";
    auto it = map.emplace(key1, Entry{""}).first;
    bool isInserted = true;
    policy.updateEvictionCandidate(it->first, it->second, isInserted);

    policy.print();
}
