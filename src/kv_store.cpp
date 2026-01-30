#include "kv_store.h"


KVStore :: KVStore() {}

KVStore :: ~KVStore() {}

bool KVStore :: set(const std::string& key, const std::string& value) {
    my_map[key] = value;
    return true;
}

std::optional<std::string> KVStore :: get(const std::string& key) const {
    auto it = my_map.find(key);
    if (it != my_map.end()) {
        return it->second;
    } 
    return {};
}

bool KVStore :: del(const std::string& key){
    if (my_map.count(key)) {
        my_map.erase(key);
        return true;
    }
    return false;
}

bool KVStore :: exists(const std::string& key) const{
    if (my_map.count(key)) {
        return true;
    }
    return false;
}

size_t KVStore :: size() const{
    return my_map.size();
}

std::vector<std::string> KVStore :: keys() const{

    std::vector<std::string> keys;
    for (const auto& pair : my_map) {
        keys.push_back(pair.first);  
    }
    return keys;
}

void KVStore :: clear(){
    my_map.clear();
}




