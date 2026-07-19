#include "pch.h"
#include "SoundSystem.h"

namespace GameEngine
{
    SoundSystem* SoundSystem::Instance()
    {
        static SoundSystem instance;
        return &instance;
    }

    SoundSystem::~SoundSystem()
    {
        Clear();
    }

    void SoundSystem::LoadMusic(const std::string& name, const std::string& sourcePath)
    {
        if (musicMap.find(name) != musicMap.end())
            return;

        sf::Music* music = new sf::Music();
        if (music->openFromFile(sourcePath))
        {
            music->setVolume(masterVolume);
            musicMap[name] = music;
            std::cout << "Loaded music: " << name << std::endl;
        }
        else
        {
            delete music;
            std::cout << "Failed to load music: " << sourcePath << std::endl;
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
            std::cout << "Playing music: " << name << std::endl;
        }
    }

    void SoundSystem::StopMusic(const std::string& name)
    {
        auto it = musicMap.find(name);
        if (it != musicMap.end())
        {
            it->second->stop();
            std::cout << "Stopped music: " << name << std::endl;
        }
    }

    void SoundSystem::PauseMusic(const std::string& name)
    {
        auto it = musicMap.find(name);
        if (it != musicMap.end())
        {
            it->second->pause();
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
    }

    void SoundSystem::SetMasterVolume(float volume)
    {
        if (volume < 0) volume = 0;
        if (volume > 100) volume = 100;

        masterVolume = volume;
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
        if (soundBufferMap.find(name) != soundBufferMap.end())
            return;

        sf::SoundBuffer* buffer = new sf::SoundBuffer();
        if (buffer->loadFromFile(sourcePath))
        {
            soundBufferMap[name] = buffer;
            std::cout << "Loaded sound: " << name << std::endl;
        }
        else
        {
            delete buffer;
            std::cout << "Failed to load sound: " << sourcePath << std::endl;
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
            sound->setBuffer(*it->second);

            float normalizedVolume = (volume / 100.0f) * masterVolume;
            sound->setVolume(normalizedVolume);

            sound->play();
            activeSounds.push_back(sound);
        }
    }

    void SoundSystem::Update()
    {
        CleanupFinishedSounds();
    }

    void SoundSystem::CleanupFinishedSounds()
    {
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
    }

    void SoundSystem::Clear()
    {
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
    }
}