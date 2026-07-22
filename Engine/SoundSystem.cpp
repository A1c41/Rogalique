#include "pch.h"
#include "SoundSystem.h"
#include "Logger.h"

namespace GameEngine
{
    SoundSystem* SoundSystem::Instance()
    {
        static SoundSystem instance;
        return &instance;
    }

    SoundSystem::~SoundSystem()
    {
        LOG_INFO("SoundSystem destroyed, clearing all sounds");
        Clear();
    }

    void SoundSystem::LoadMusic(const std::string& name, const std::string& sourcePath)
    {
        if (musicMap.find(name) != musicMap.end()) {
            LOG_WARN("Music already loaded: " + name);
            return;
        }

        sf::Music* music = new sf::Music();
        if (!music) {
            LOG_ERROR("Failed to allocate music: " + name);
            throw std::bad_alloc();
        }

        if (music->openFromFile(sourcePath))
        {
            music->setVolume(masterVolume);
            musicMap[name] = music;
            LOG_INFO("Loaded music: " + name + " from " + sourcePath);
        }
        else
        {
            delete music;
            LOG_ERROR("Failed to load music from file: " + sourcePath);
            throw std::runtime_error("Failed to load music: " + sourcePath);
        }
    }

    void SoundSystem::PlayMusic(const std::string& name, bool loop)
    {
        auto it = musicMap.find(name);
        if (it != musicMap.end())
        {
            it->second->setLoop(loop);
            it->second->setVolume(masterVolume);
            it->second->play();
            LOG_INFO("Playing music: " + name + " (loop: " + (loop ? "true" : "false") + ")");
        }
        else
        {
            LOG_WARN("Attempted to play non-existent music: " + name);
        }
    }

    void SoundSystem::StopMusic(const std::string& name)
    {
        auto it = musicMap.find(name);
        if (it != musicMap.end())
        {
            it->second->stop();
            LOG_INFO("Stopped music: " + name);
        }
        else
        {
            LOG_WARN("Attempted to stop non-existent music: " + name);
        }
    }

    void SoundSystem::PauseMusic(const std::string& name)
    {
        auto it = musicMap.find(name);
        if (it != musicMap.end())
        {
            it->second->pause();
            LOG_INFO("Paused music: " + name);
        }
        else
        {
            LOG_WARN("Attempted to pause non-existent music: " + name);
        }
    }

    void SoundSystem::SetMusicVolume(const std::string& name, float volume)
    {
        auto it = musicMap.find(name);
        if (it != musicMap.end())
        {
            float normalizedVolume = (volume / 100.0f) * masterVolume;
            it->second->setVolume(normalizedVolume);
        }
        else
        {
            LOG_WARN("Attempted to set volume for non-existent music: " + name);
        }
    }

    void SoundSystem::SetMasterVolume(float volume)
    {
        if (volume < 0) volume = 0;
        if (volume > 100) volume = 100;

        masterVolume = volume;
        LOG_INFO("Master volume set to: " + std::to_string(volume));
        UpdateMusicVolumes();
    }

    void SoundSystem::UpdateMusicVolumes()
    {
        for (auto& pair : musicMap)
        {
            if (pair.second->getStatus() == sf::Music::Playing)
            {
                float currentVolume = pair.second->getVolume();
                float relativeVolume = currentVolume / 100.0f;
                pair.second->setVolume(masterVolume * relativeVolume);
            }
        }
    }

    void SoundSystem::LoadSound(const std::string& name, const std::string& sourcePath)
    {
        if (soundBufferMap.find(name) != soundBufferMap.end()) {
            LOG_WARN("Sound buffer already loaded: " + name);
            return;
        }

        sf::SoundBuffer* buffer = new sf::SoundBuffer();
        if (!buffer) {
            LOG_ERROR("Failed to allocate sound buffer: " + name);
            throw std::bad_alloc();
        }

        if (buffer->loadFromFile(sourcePath))
        {
            soundBufferMap[name] = buffer;
            LOG_INFO("Loaded sound: " + name + " from " + sourcePath);
        }
        else
        {
            delete buffer;
            LOG_ERROR("Failed to load sound from file: " + sourcePath);
            throw std::runtime_error("Failed to load sound: " + sourcePath);
        }
    }

    void SoundSystem::PlaySound(const std::string& name)
    {
        PlaySound(name, 100.0f);
    }

    void SoundSystem::PlaySound(const std::string& name, float volume)
    {
        CleanupFinishedSounds();

        auto it = soundBufferMap.find(name);
        if (it != soundBufferMap.end())
        {
            sf::Sound* sound = new sf::Sound();
            if (!sound) {
                LOG_ERROR("Failed to allocate sound instance for: " + name);
                throw std::bad_alloc();
            }

            sound->setBuffer(*it->second);
            float normalizedVolume = (volume / 100.0f) * masterVolume;
            sound->setVolume(normalizedVolume);
            sound->play();
            activeSounds.push_back(sound);
        }
        else
        {
            LOG_WARN("Attempted to play non-existent sound: " + name);
        }
    }

    void SoundSystem::Update()
    {
        CleanupFinishedSounds();
    }

    void SoundSystem::CleanupFinishedSounds()
    {
        auto initialSize = activeSounds.size();
        activeSounds.erase(
            std::remove_if(activeSounds.begin(), activeSounds.end(),
                [](sf::Sound* sound) {
                    if (sound->getStatus() == sf::Sound::Stopped)
                    {
                        delete sound;
                        return true;
                    }
                    return false;
                }),
            activeSounds.end()
        );

        if (activeSounds.size() != initialSize) {
            LOG_INFO("Cleaned up " + std::to_string(initialSize - activeSounds.size()) + " finished sounds");
        }
    }

    void SoundSystem::Clear()
    {
        LOG_INFO("Clearing all sounds and music");

        for (auto& pair : musicMap)
        {
            pair.second->stop();
            delete pair.second;
        }
        musicMap.clear();

        for (auto& pair : soundBufferMap)
        {
            delete pair.second;
        }
        soundBufferMap.clear();

        for (auto& sound : activeSounds)
        {
            sound->stop();
            delete sound;
        }
        activeSounds.clear();

        LOG_INFO("All sounds and music cleared");
    }
}