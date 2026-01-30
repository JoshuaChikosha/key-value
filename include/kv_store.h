#pragma once 

#include <unordered_map>
#include <string>
#include <optional>
#include <vector>

class KVStore {

    public:

    KVStore();
    ~KVStore();

    bool set(const std::string& key, const std::string& value);
    std::optional<std::string>  get(const std::string& key) const;
    bool del(const std::string& key);
    bool exists(const std::string& key) const;
    size_t size() const; 
    std::vector<std::string> keys() const;
    void clear();

    private:
    std::unordered_map<std::string, std::string> my_map;
};