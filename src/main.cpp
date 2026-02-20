#include "kv_store.h" 
#include "command.h"
#include <iostream>
#include <cassert>

void repl();
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
    KVStore store;
    while (true) {
        std::cout << "kv> ";
        std::string input;
        std::getline(std::cin, input);
        execute(input, store);


    }
}



static int runTests() {
    KVStore store;

    // set/get
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

    //del
    assert(store.del("b") == true);
    assert(store.del("b") == false);

    //clear
    store.clear();
    assert(store.size() == 0);

    //keys
    store.set("x", "1");
    store.set("y", "2");
    std::vector<std::string> allKeys = store.keys();
    assert(allKeys.size() == 2);


    std::cout  << "Pass\n";
    return 0;
}