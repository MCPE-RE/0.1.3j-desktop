#include "SoundRepo.h"


void SoundRepo::add(std::string resourceId, std::string resourceName) {
    std::map<std::string, std::vector<std::string> >::iterator it = this->repository.find(resourceId);
    if (it == this->repository.end()) {
        std::vector<std::string> resourceNames;
        resourceNames.push_back(resourceName);
        this->repository.insert(std::pair<std::string, std::vector<std::string> >(resourceId, resourceNames));
    } else {
        it->second.push_back(resourceName);
    }
}

bool SoundRepo::get(std::string resourceId, std::string *resourceName) {
    std::map<std::string, std::vector<std::string> >::iterator it = this->repository.find(resourceId);
    if (it != this->repository.end()) {
        size_t resourceNamesCount = it->second.size();
        if (resourceNamesCount > 0) {
            int32_t soundIndex = this->random.nextInt() & (resourceNamesCount - 1);
            *resourceName = it->second[soundIndex];
            return true;
        }
    }
    return false;
}

void SoundRepo::removeAll() {
    this->repository.clear();
}