#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <map>
#include <vector>
#include <iostream>
#include "SoundRepo.h"

class AudioEngine {
private:
    std::map<std::string, ALuint> buffers;
    std::vector<ALuint> idleSources;
    std::vector<ALuint> activeSources;
    std::vector<std::string> searchPaths;
    SoundRepo repo;
    bool isInitialized;
    ALCdevice *device;
    ALCcontext *context;

    bool getSource(ALuint *source);

    bool getBuffer(std::string resourceId, ALuint *buffer);

    void initDefaultSounds();

    void deleteAllbuffers();

    void deleteAllSources();

public:
    AudioEngine();

    bool init();

    void deinit();

    void addPath(std::string path);

    void tick();

    bool play(std::string resourceId, float volume, float pitch);
};