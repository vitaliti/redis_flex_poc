#ifndef entry_hpp
#define entry_hpp

#include <optional>
#include <string>
#include <list>

struct Entry
{
    Entry(std::string value)
        : m_value{std::move(value)}
    {
    }

    std::optional<std::string> m_value;

    // TRADEOFF for having metadata of policies here instead of inside their classes:
    // + less Key dupliation = less RAM
    // - coupling between Entry and all future algorithms
    // - not used metadata depending on the algo used

    // LRU
    std::list<std::string>::iterator m_lruIt;

    // LFU
    uint32_t m_frequency{0};
    std::list<std::string>::iterator m_lfuIt;
};

#endif
