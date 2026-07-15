#include <iostream>
#include <unordered_map>

#include "rocksdb/db.h"

#include "db_controller.hpp"
#include "speedb_flash_storage.hpp"

int main()
{
    SpeedbFlashStorage dbStorage("mydb");
    DBController db(dbStorage);
    int userChoice = 0;
    while (true && userChoice != 8)
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
                printf("Write a key/data pari:\n");
                printf("Key:\n");
                std::string key;
                std::cin >> key;
                printf("Data:\n");
                std::string value;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, value);
                db.Put(key, value);
            }
            else if(userChoice == 2)
            {
                printf("Get data from a key:");
                printf("Key:\n");
                std::string key;
                std::cin >> key;
                auto value = db.Get(key);
                if(value)
                {
                    printf("Value:%s \n", value->c_str());
                }
                else
                {
                    printf("NO SUCH DATA! \n");
                }
                
            }
            else if(userChoice == 3)
            {
                printf("Delete data with a key:");
                std::string key;
                std::cin >> key;
                db.Delete(key);
            }
            else if(userChoice == 4)
            {
                db.PrintCache();
            }
            else if(userChoice == 5)
            {
                db.PrintDB();
            }
        }
    }
    return 0;
}