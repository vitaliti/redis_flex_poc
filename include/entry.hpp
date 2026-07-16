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
    std::list<std::string>::iterator lruIt;
};

#endif
