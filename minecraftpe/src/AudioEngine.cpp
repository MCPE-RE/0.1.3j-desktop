#include "AudioEngine.h"
#include <cstdlib>
#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>
#include "GeneralDefs.h"
#include <iostream>
#include <fstream>
#include <list>

bool AudioEngine::getSource(ALuint *source) {
    if (this->idleSources.size() > 0) {
        *source = this->idleSources.back();
        this->idleSources.pop_back();
        return true;
    }
    alGenSources(1, source);
    if (*source && alIsSource(*source)) {
        return true;
    }
    return false;
}

bool AudioEngine::getBuffer(std::string resourceId, ALuint *buffer) {
    std::string resourceName;
    if (this->repo.get(resourceId, &resourceName)) {
        std::map<std::string, ALuint>::iterator it = this->buffers.find(resourceName);
        if (it == this->buffers.end()) {
            bool found = false;
            std::string path;
            for (size_t i = 0; i < this->searchPaths.size(); ++i) {
                path = this->searchPaths[i] + DIR_SEPARATOR + resourceName + ".ogg";
                std::ifstream file(path.c_str());
                if (file.good()) {
                    found = true;
                    break;
                }
            }
            if (found) {
                ALuint alBuffer;
                alGenBuffers(1, &alBuffer);
                if (buffer && alIsBuffer(alBuffer)) {
                    int16_t *bufferData = nullptr;
                    int32_t channels;
                    int32_t sampleRate;
                    int32_t samples = stb_vorbis_decode_filename(path.c_str(), &channels, &sampleRate, &bufferData);
                    ALenum format = AL_NONE;
                    if (channels == 1) {
                        format = AL_FORMAT_MONO16;
                    } else if (channels == 2) {
                        format = AL_FORMAT_STEREO16;
                    }
                    ALenum alErr;
                    alBufferData(alBuffer, format, bufferData, samples * channels * 2, sampleRate);
                    if (bufferData) {
                        free(bufferData);
                    }
                    alErr = alGetError();
                    if (alErr == AL_NO_ERROR) {
                        *buffer = alBuffer;
                        this->buffers.insert(std::pair<std::string, ALuint>(resourceName, alBuffer));
                        return true;
                    } else {
                        if (alBuffer && alIsBuffer(alBuffer)) {
                            alDeleteBuffers(1, &alBuffer);
                        }
                    }
                } else {
                    alDeleteBuffers(1, &alBuffer);
                }
            }
        } else {
            *buffer = it->second;
            return true;
        }
    }
    return false;
}

void AudioEngine::deleteAllbuffers() {
    std::map<std::string, ALuint>::iterator it = this->buffers.begin();
    while (it != this->buffers.end()) {
        if (it->second && alIsBuffer(it->second)) {
            alDeleteBuffers(1, &it->second);
        }
        ++it;
    }
    this->buffers.clear();
}

void AudioEngine::deleteAllSources() {
    for (size_t i = 0; i < this->idleSources.size(); ++i) {
        if (this->idleSources[i] && alIsSource(this->idleSources[i])) {
            alSourceStop(this->idleSources[i]);
            alDeleteSources(1, &this->idleSources[i]);
        }
    }
    this->idleSources.clear();

    for (size_t i = 0; i < this->activeSources.size(); ++i) {
        if (this->activeSources[i] && alIsSource(this->activeSources[i])) {
            alSourceStop(this->activeSources[i]);
            alDeleteSources(1, &this->activeSources[i]);
        }
    }
    this->activeSources.clear();
}

void AudioEngine::initDefaultSounds() {
    this->repo.add("step.cloth", "cloth1");
    this->repo.add("step.cloth", "cloth2");
    this->repo.add("step.cloth", "cloth3");
    this->repo.add("step.cloth", "cloth4");
    this->repo.add("step.grass", "grass1");
    this->repo.add("step.grass", "grass2");
    this->repo.add("step.grass", "grass3");
    this->repo.add("step.grass", "grass4");
    this->repo.add("step.gravel", "gravel1");
    this->repo.add("step.gravel", "gravel2");
    this->repo.add("step.gravel", "gravel3");
    this->repo.add("step.gravel", "gravel4");
    this->repo.add("step.sand", "sand1");
    this->repo.add("step.sand", "sand2");
    this->repo.add("step.sand", "sand3");
    this->repo.add("step.sand", "sand4");
    this->repo.add("step.stone", "stone1");
    this->repo.add("step.stone", "stone2");
    this->repo.add("step.stone", "stone3");
    this->repo.add("step.stone", "stone4");
    this->repo.add("step.wood", "wood1");
    this->repo.add("step.wood", "wood2");
    this->repo.add("step.wood", "wood3");
    this->repo.add("step.wood", "wood4");
    this->repo.add("random.splash", "splash");
    this->repo.add("random.explode", "explode");
    this->repo.add("random.click", "click");
}

AudioEngine::AudioEngine() {
    this->isInitialized = false;
    this->device = nullptr;
    this->context = nullptr;
}

bool AudioEngine::init() {
    if (!this->isInitialized) {
        this->device = alcOpenDevice(NULL);
        if (!this->device) {
            std::cout << "[AudioEngine] Cannot open the device" << std::endl;
            return false;
        }
        this->context = alcCreateContext(this->device, NULL);
        ALCenum alc_err = alcGetError(this->device);
        if (alc_err != ALC_NO_ERROR) {
            std::cout << "[AudioEngine] Cannot create the context" << std::endl;
            return false;
        }
        alcMakeContextCurrent(this->context);
        alc_err = alcGetError(this->device);
        if (alc_err != ALC_NO_ERROR) {
            std::cout << "[AudioEngine] Cannot set the current context" << std::endl;
            return false;
        }
        
        this->initDefaultSounds();
        this->isInitialized = true;
        return true;
    }
    return false;
}

void AudioEngine::deinit() {
    if (this->isInitialized) {
        this->isInitialized = false;

        this->repo.removeAll();
        this->searchPaths.clear();
        this->deleteAllSources();
        this->deleteAllbuffers();

        alcMakeContextCurrent(nullptr);

        ALCenum alc_err = alcGetError(this->device);
        if (alc_err != ALC_NO_ERROR) {
            std::cout << "[AudioEngine] Cannot set the context to NULL" << std::endl;
        }
        alcDestroyContext(this->context);
        alc_err = alcGetError(this->device);
        if (alc_err != ALC_NO_ERROR) {
            std::cout << "[AudioEngine] Cannot destroy the context" << std::endl;
        }
        alcCloseDevice(this->device);
        alc_err = alcGetError(this->device);
        if (alc_err != ALC_NO_ERROR) {
            std::cout << "[AudioEngine] Cannot close the device" << std::endl;
        }
    }
}

void AudioEngine::addPath(std::string path) {
    if (this->isInitialized) {
        this->searchPaths.push_back(path);
    }
}

void AudioEngine::tick() {
    if (this->isInitialized) {
        std::list<size_t> indexesToDelete;
        for (size_t i = 0; i < this->activeSources.size(); ++i) {
            ALuint currentSource = this->activeSources[i];
            if (currentSource && alIsSource(currentSource)) {
                ALint state;
                alGetSourcei(currentSource, AL_SOURCE_STATE, &state);
                if (state != AL_PLAYING) {
                    indexesToDelete.push_front(i);
                    if (this->idleSources.size() < 50) {
                        this->idleSources.push_back(currentSource);
                    } else {
                        alDeleteSources(1, &currentSource);
                    }
                }
            } else {
                indexesToDelete.push_front(i);
            }
        }
        std::list<size_t>::iterator it = indexesToDelete.begin();
        while (it != indexesToDelete.end()) {
            this->activeSources.erase(this->activeSources.begin() + *it);
            ++it;
        }
    }
}

bool AudioEngine::play(std::string resourceId, float volume, float pitch) {
    if (this->isInitialized) {
        ALuint buffer;
        if (this->getBuffer(resourceId, &buffer)) {
            ALuint source;
            if (this->getSource(&source)) {
                alSourcei(source, AL_LOOPING, AL_FALSE);
                ALenum err = alGetError();
                if (err != AL_NO_ERROR) {
                    if (source && alIsSource(source)) {
                        alDeleteSources(1, &source);
                        return false;
                    }
                }
                alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
                err = alGetError();
                if (err != AL_NO_ERROR) {
                    if (source && alIsSource(source)) {
                        alDeleteSources(1, &source);
                        return false;
                    }
                }
                alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
                err = alGetError();
                if (err != AL_NO_ERROR) {
                    if (source && alIsSource(source)) {
                        alDeleteSources(1, &source);
                        return false;
                    }
                }
                alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
                err = alGetError();
                if (err != AL_NO_ERROR) {
                    if (source && alIsSource(source)) {
                        alDeleteSources(1, &source);
                        return false;
                    }
                }
                alSourcef(source, AL_GAIN, volume);
                err = alGetError();
                if (err != AL_NO_ERROR) {
                    if (source && alIsSource(source)) {
                        alDeleteSources(1, &source);
                        return false;
                    }
                }
                alSourcef(source, AL_PITCH, pitch);
                err = alGetError();
                if (err != AL_NO_ERROR) {
                    if (source && alIsSource(source)) {
                        alDeleteSources(1, &source);
                        return false;
                    }
                }
                alSourcei(source, AL_BUFFER, buffer);
                err = alGetError();
                if (err != AL_NO_ERROR) {
                    if (source && alIsSource(source)) {
                        alDeleteSources(1, &source);
                        return false;
                    }
                }
                alSourcePlay(source);
                err = alGetError();
                if (err != AL_NO_ERROR) {
                    if (source && alIsSource(source)) {
                        alDeleteSources(1, &source);
                        return false;
                    }
                }
                this->activeSources.push_back(source);
                return true;
            }
        }
    }
    return false;
}