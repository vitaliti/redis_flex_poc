#include "memory_controller.hpp"
#include "speedb_flash_storage.hpp"
#include "ram_cache.hpp"
#include "lru_eviction_policy.hpp"
#include "lfu_eviction_policy.hpp"

#include <iostream>
#include <unordered_map>

int main()
{
    printf("___________________________\n");
    printf("1) Press 1 for LRU anything else for LFU \n");
    printf("___________________________\n");
    int userChoice = 0;
    std::cin >> userChoice;
    std::unique_ptr<IEvictionPolicy> policy;
    if(userChoice == 1)
    {
        policy = std::make_unique<LruEvictionPolicy>();
        printf("LRU shall be used \n");
    }
    else
    {
        policy = std::make_unique<LfuEvictionPolicy>();
        printf("LFU shall be used \n");
    }

    RamCache ramCache{*policy};
    SpeedbFlashStorage dbStorage("mydb");
    MemoryController mc(dbStorage, ramCache);
    
    while (userChoice != 8)
    {
        printf("___________________________\n");
        printf("1) Put new Key/Value into Cache/DB \n");
        printf("2) Get Data from Cache/DB \n");
        printf("3) Delete a Key/Value from Cache/DB \n");
        printf("4) Print values in Cache \n");
        printf("5) Print values in DB \n");
        // printf("6) Save all Cached Key/Values into DB \n");
        // printf("7) Read DB and put it into RAM \n");
        printf("8) EXIT \n");
        printf("___________________________\n");
        
        std::cin >> userChoice;
        if(!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printf("Bad Input !!!\n");
        }
        else
        {
            if(userChoice == 1)
            {
                printf("Write a key/data pair:\n");
                printf("Key:\n");
                std::string key;
                std::cin >> key;
                printf("Data:\n");
                std::string value;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, value);
                mc.put(key, value);
            }
            else if(userChoice == 2)
            {
                printf("Get data from a key:\n");
                printf("Key:");
                std::string key;
                std::cin >> key;
                auto value = mc.get(key);
                if(value)
                {
                    printf("Value:%s \n", value->c_str());
                }
                else
                {
                    printf("\n NO SUCH DATA! \n");
                }
                
            }
            else if(userChoice == 3)
            {
                printf("Delete data with a key:");
                std::string key;
                std::cin >> key;
                mc.remove(key);
            }
            else if(userChoice == 4)
            {
                mc.printCache();
            }
            else if(userChoice == 5)
            {
                mc.printDB();
            }
        }
    }
    return 0;
}