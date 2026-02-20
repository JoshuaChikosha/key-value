#include "command.h"

std::vector<std::string> split(const std::string& input){
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (ss >> token){
        tokens.push_back(token);
    }
    return tokens;
}

void help() {
    std::cout << "Available commands: " << std::endl;
    std::cout << "SET <key> <value>        -> prints OK" << std::endl;
    std::cout << "GET <key>                -> prints value or (nil)" << std::endl;
    std::cout << "DEL <key>                -> prints 1 if deleted else 0" << std::endl;
    std::cout << "EXISTS <key>             -> prints 1 or 0" << std::endl;
    std::cout << "SIZE                     -> number of keys" << std::endl;
    std::cout << "KEYS                     -> list all keys (any order ok)" << std::endl;
    std::cout << "CLEAR                    -> remove everything" << std::endl;
    std::cout << "HELP                     -> print commands" << std::endl;
    std::cout << "EXIT                     -> quit program" << std::endl;
}
void execute(std::string input,  KVStore& store) {

    std::vector<std::string> processedInput  = split(input);

    if (processedInput.empty()) return;

    if (processedInput.size() == 3 && processedInput[0] == "SET") {
        store.set(processedInput[1],  processedInput[2]);
        std::cout << "OK" << std::endl;

    }

    else if (processedInput.size() == 2 && processedInput[0] == "GET") {
        auto value = store.get(processedInput[1]);
        if (value) {
            std::cout << *value << std::endl;
        } else {
            std::cout << "(nil)" << std::endl;
        }
    }
    else if (processedInput.size() == 2 && processedInput[0] == "DEL") {
        if ( store.del(processedInput[1]))
        {
            std::cout << "1" << std::endl;
        }
        else{
            std::cout << "0" << std::endl;
        }
        
    }
    else if (processedInput.size() == 2 && processedInput[0] == "EXISTS")
    {
        if (store.exists(processedInput[1]))
        {
            std::cout << "1" << std::endl;
        }
        else{
            std::cout << "0" << std::endl;
        }
    }
    else if (processedInput.size() == 1 && processedInput[0] == "SIZE")
    {
        std::cout << store.size() << std::endl;
    }
    else if (processedInput.size() == 1 && processedInput[0] == "KEYS")
    {
        std::vector<std::string> keys  =store.keys();
        for (size_t i = 0; i < keys.size(); i++){
            std::cout << keys[i] << std::endl;
        }

    }
    else if (processedInput.size() == 1 && processedInput[0] == "CLEAR")
    {
        store.clear();
        std::cout << "OK" << std::endl;
    }
    else if (processedInput[0] == "HELP") {
        help();
    }

    else if (processedInput[0] == "EXIT") {
        exit(0);
    }
    else {
        std::cout << "ERR unknown command" << std::endl;
    }



}