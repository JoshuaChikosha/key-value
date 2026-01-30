#include "kv_store.h" 
#include <iostream>
#include <cassert>

void repl();
void help();
void execute(std::string input);
static int runTests();

int main(int argc, char* argv[]) {

    if (argc > 1 && std::string(argv[1]) == "--help") {
        help();     //replace with actual help message later
        return 0;
    }
    if (argc > 1 && std::string(argv[1]) == "--run-tests") {
        return runTests(); 
    }
    repl();

}
void repl() {
    while (true) {
        std::cout << "kv> ";
        std::string input;
        std::getline(std::cin, input);
        execute(input);
        std::cout << input << std::endl;
    }
}
void execute(std::string input) {
    if (input == "help") {
        help();
    }
    else if (input == "exit") {
        exit(0);
    }
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

static int runTests() {
    KVStore store;

    // set/goal
    store.set("a", "10");
    auto v = store.get("a");
    assert(v.has_value());
    assert(*v == "10");

    // overwrite
    store.set("a", "20");
    v = store.get("a");
    assert(v && *v == "20");

    //exits
    assert(store.exists("a") == true);
    assert(store.exists("missing") == false);

    //size
    assert(store.size() == 1);
    store.set("b", "x");
    assert(store.size() == 2);


    std::cout  << "Pass\n";
    return 0;
}