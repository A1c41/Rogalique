#pragma once

#include "framework.h"
#include <map>
#include <string>
#include <vector>
#include <SFML/Audio.hpp>

namespace GameEngine
{
    class ENGINE_API SoundSystem
    {
    public:
        static SoundSystem* Instance();

        void LoadMusic(const std::string& name, const std::string& sourcePath);
        void PlayMusic(const std::string& name, bool loop = false);
        void StopMusic(const std::string& name);
        void PauseMusic(const std::string& name);
        void SetMusicVolume(const std::string& name, float volume);
        void SetMasterVolume(float volume);

        void LoadSound(const std::string& name, const std::string& sourcePath);
        void PlaySound(const std::string& name);
        void PlaySound(const std::string& name, float volume);

        void Update();
        void Clear();

    private:
        std::map<std::string, sf::Music*> musicMap;
        std::map<std::string, sf::SoundBuffer*> soundBufferMap;
        std::vector<sf::Sound*> activeSounds;

        float masterVolume = 100.0f;

        SoundSystem() {}
        ~SoundSystem();

        SoundSystem(SoundSystem const&) = delete;
        SoundSystem& operator= (SoundSystem const&) = delete;

        void CleanupFinishedSounds();
        void UpdateMusicVolumes();
    };
}