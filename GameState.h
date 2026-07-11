#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include "Constants.h"

namespace Rogalique {

    class GameState {
    private:
        enum State { MENU, PLAYING, SETTINGS, DIFFICULTY };
        enum Difficulty { EASY, MEDIUM, HARD };

        std::unique_ptr<sf::RenderWindow> window;
        std::unique_ptr<sf::Font> font;

        sf::SoundBuffer bufferClick;
        sf::Sound soundClick;
        sf::Music music;

        sf::Text textPlug;

        State currentState;
        Difficulty currentDifficulty;

        int menuSelection, difficultySelection;
        bool soundEnabled, musicEnabled;

        void resetGame();

        void renderGame();
        void renderMenu();
        void renderSettings();
        void renderDifficulty();

        void handleMenuInput(const sf::Event& event);
        void handleSettingsInput(const sf::Event& event);
        void handleDifficultyInput(const sf::Event& event);

    public:
        GameState();
        ~GameState() = default;

        void update(float dt);
        void render();
        void handleInput(const sf::Event& event);
        bool isRunning() const;
        sf::RenderWindow& getWindow();
    };

}