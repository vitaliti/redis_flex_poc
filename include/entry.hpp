#ifndef entry_hpp
#define entry_hpp

#include <optional>
#include <string>
#include <list>

struct Entry
{
    Entry(std::string value)
        : value(std::move(value))
    {
    }

    std::optional<std::string> value;

    // TRADEOFF for having metadata of algos here instead of inside their classes:
    // + less Key dupliation = less RAM
    // - coupling between Entry and all future algorithms
    // - not used metadata depending on the algo used

    // Used by LRU eviction policy
    std::list<std::string>::iterator lruIt;

    // LFU
    // uint32_t frequency = 0;
};

#endif
