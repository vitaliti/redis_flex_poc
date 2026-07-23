#include "memory_controller.hpp"
#include "speedb_flash_storage.hpp"
#include "ram_cache.hpp"
#include "lru_eviction_policy.hpp"
#include "lfu_eviction_policy.hpp"

#include <iostream>
#include <unordered_map>
int GetIntInput(const std::string& prompt);

int main()
{    
    int userChoice = GetIntInput("Press 1 for LRU anything else for LFU \n");
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

    userChoice = GetIntInput("Write RAM size in bytes(default is 500) max is 1000 \n");
    uint32_t ramSizeInBytes = 500;
    if(userChoice > 0 && userChoice <= 1000)
    {
        ramSizeInBytes = static_cast<uint32_t>(userChoice);
    }
    else
    {
        printf("Illeageal Choice, 500 bytes will be chosen as RAM! \n");
    }

    RamCache ramCache{*policy, ramSizeInBytes};
    SpeedbFlashStorage dbStorage("mydb");
    MemoryController mc(dbStorage, ramCache);

    std::string menu =
        "1) Put new Key/value into Cache/DB \n"
        "2) Get Data from Cache/DB \n"
        "3) Delete a Key/value from Cache/DB \n"
        "4) Print Cache \n"
        "5) Print DB \n"
        // printf("6) Save all Cached Key/userInputs into DB \n");
        // printf("7) Read DB and put it into RAM \n");
        "8) EXIT \n";
    
    while (userChoice != 8)
    {
        userChoice = GetIntInput(menu);
        if(userChoice == 1)
        {
            printf("Write a key/data pair:\n");
            printf("Key:\n");
            std::string key;
            std::cin >> key;
            printf("Data:\n");
            std::string userInput;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, userInput);
            mc.put(key, userInput);
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
                printf("value:%s \n", value->c_str());
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
    return 0;
}

int GetIntInput(const std::string& prompt)
{
    int userInput = 0;
    while (true)
    {
        printf("___________________________\n");
        printf("%s", prompt.c_str());
        printf("___________________________\n");
        std::cin >> userInput;

        if (std::cin)
        {
            // Remove anything left on the line
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return userInput;
        }

        // Bad input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad input!!! Please enter an integer.\n";
    }
}