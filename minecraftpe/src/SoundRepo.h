#pragma once
 
#include <map>
#include <vector>
#include <iostream>
#include "../0.1.3j-core/src/math/Random.h"

class SoundRepo {
private:
    std::map<std::string, std::vector<std::string> > repository;
    Random random;
public:
    void add(std::string resourceId, std::string resourceName);

    bool get(std::string resourceId, std::string *resourceName);

    void removeAll();
};
