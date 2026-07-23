// #include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lfu_eviction_policy.hpp"

using namespace testing;

class LfuEvictionPolicyTest : public Test {
protected:
    LfuEvictionPolicy policy{};
};

TEST_F(LfuEvictionPolicyTest, getEmptyEvictionCandidate) {
    auto result = policy.getEvictionCandidate();
    EXPECT_FALSE(result);
}

TEST_F(LfuEvictionPolicyTest, getOnlyInsertedEvictionCandidate) {
    std::unordered_map<std::string, Entry> map;

    const std::string expectedResult = "testttKEYYY";
    auto it = map.emplace(expectedResult, Entry{""}).first;
    bool isInserted = true;

    policy.updateEvictionCandidate(it, isInserted);
    auto result = policy.getEvictionCandidate();
    EXPECT_EQ(expectedResult, *result);
}

TEST_F(LfuEvictionPolicyTest, getLFUEvictionCandidate) {
    std::unordered_map<std::string, Entry> map;

    const std::string expectedResultKey1 = "KEY1";
    auto it = map.emplace(expectedResultKey1, Entry{""}).first;
    const std::string key2 = "KEY2";
    auto it2 = map.emplace(key2, Entry{""}).first;

    bool isInserted = true;
    policy.updateEvictionCandidate(it, isInserted);
    policy.updateEvictionCandidate(it2, isInserted);

    isInserted = false;
    policy.updateEvictionCandidate(it2, isInserted);
    policy.updateEvictionCandidate(it2, isInserted);
    auto result = policy.getEvictionCandidate();
    EXPECT_EQ(expectedResultKey1, *result);
}