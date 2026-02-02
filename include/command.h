#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "kv_store.h"


void execute(std::string input, KVStore& store);

struct command {
    std::vector<std::string> args;
    std::string name;
};

void help();

std::vector<std::string> split(const std::string& input);
