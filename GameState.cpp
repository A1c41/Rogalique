#include "GameState.h"
#include <cmath>
#include <cstdlib>
#include <sstream>

namespace Rogalique {

    GameState::GameState() : currentState(MENU), currentDifficulty(MEDIUM),
        menuSelection(0), difficultySelection(0), soundEnabled(true), musicEnabled(true) {

        window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Rogalique");
        window->setFramerateLimit(60);

        font = std::make_unique<sf::Font>();
        if (!font->loadFromFile(RESOURCES + "fonts\\Roboto-Black.ttf")) {
            font->loadFromFile("C:/Windows/Fonts/arial.ttf");
        }

        textPlug.setFont(*font);
        textPlug.setString("TO BE DONE... Press Esc to exit to menu");
        textPlug.setCharacterSize(24);
        textPlug.setFillColor(sf::Color::Red);
        textPlug.setStyle(sf::Text::Bold | sf::Text::Underlined);

        bufferClick.loadFromFile(RESOURCES + "click.wav");
        soundClick.setBuffer(bufferClick);        

        if (music.openFromFile(RESOURCES + "music.wav")) {
            music.setLoop(true);
            music.setVolume(50);
        }
    }

    void GameState::resetGame() {
        currentState = PLAYING;

        if (musicEnabled && music.getStatus() != sf::Music::Playing) {
            music.play();
        }
    }

    void GameState::update(float dt) {

        if (currentState != PLAYING) return;

        sf::Vector2u windowCurrentSize = window->getSize();
        sf::FloatRect textRect = textPlug.getLocalBounds();
        sf::Vector2f textPlugPosition = { windowCurrentSize.x / 2 - textRect.width / 2, windowCurrentSize.y / 2 - textRect.height / 2 };
        textPlug.setPosition(textPlugPosition);
    }

    void GameState::render() 
    {
        window->clear(sf::Color(20, 20, 40));

        switch (currentState) {
        case MENU: renderMenu(); break;
        case PLAYING: renderGame(); break;
        case SETTINGS: renderSettings(); break;
        case DIFFICULTY: renderDifficulty(); break;
        }

        window->display();
    }

    void GameState::renderGame() 
    {    
        window->draw(textPlug);
    }

    void GameState::renderMenu() {
        sf::Text title("ROGALIQUE", *font, 72);
        title.setFillColor(sf::Color::White);
        title.setPosition(WINDOW_WIDTH / 2.0f - 150.0f, 80.0f);
        window->draw(title);

        std::vector<std::string> items = { "Play", "Difficulty", "Settings", "Exit" };
        for (size_t i = 0; i < items.size(); ++i) {
            sf::Text item(items[i], *font, 34);
            item.setFillColor(i == menuSelection ? sf::Color::Yellow : sf::Color::White);
            item.setPosition(WINDOW_WIDTH / 2.0f - 80.0f, 200.0f + i * 60.0f);
            window->draw(item);
        }

        std::string diffText = "Current: ";
        switch (currentDifficulty) {
        case EASY: diffText += "Easy"; break;
        case MEDIUM: diffText += "Medium"; break;
        case HARD: diffText += "Hard"; break;
        }
        sf::Text diffInfo(diffText, *font, 22);
        diffInfo.setFillColor(sf::Color(200, 200, 200));
        diffInfo.setPosition(WINDOW_WIDTH / 2.0f - 70.0f, 500.0f);
        window->draw(diffInfo);
    }

    void GameState::renderSettings() {
        sf::Text title("SETTINGS", *font, 56);
        title.setFillColor(sf::Color::Cyan);
        title.setPosition(WINDOW_WIDTH / 2.0f - 130.0f, 100.0f);
        window->draw(title);

        std::vector<std::string> items = {
            "Sound: " + std::string(soundEnabled ? "ON" : "OFF"),
            "Music: " + std::string(musicEnabled ? "ON" : "OFF")
        };
        for (size_t i = 0; i < items.size(); ++i) {
            sf::Text item(items[i], *font, 34);
            item.setFillColor(i == menuSelection ? sf::Color::Yellow : sf::Color::White);
            item.setPosition(WINDOW_WIDTH / 2.0f - 120.0f, 250.0f + i * 70.0f);
            window->draw(item);
        }

        sf::Text hint("Use Up/Down to select, Left/Right to toggle", *font, 20);
        hint.setFillColor(sf::Color(200, 200, 200));
        hint.setPosition(WINDOW_WIDTH / 2.0f - 200.0f, WINDOW_HEIGHT - 70.0f);
        window->draw(hint);

        sf::Text backHint("Press Escape to return to menu", *font, 20);
        backHint.setFillColor(sf::Color(200, 200, 200));
        backHint.setPosition(WINDOW_WIDTH / 2.0f - 160.0f, WINDOW_HEIGHT - 40.0f);
        window->draw(backHint);
    }

    void GameState::renderDifficulty() {
        sf::Text title("DIFFICULTY", *font, 56);
        title.setFillColor(sf::Color::Cyan);
        title.setPosition(WINDOW_WIDTH / 2.0f - 150.0f, 100.0f);
        window->draw(title);

        std::vector<std::string> items = { "Easy", "Medium", "Hard" };
        for (size_t i = 0; i < items.size(); ++i) {
            sf::Text item(items[i], *font, 34);
            bool selected = (i == difficultySelection);
            bool current = (i == static_cast<int>(currentDifficulty));
            if (selected) item.setFillColor(sf::Color::Yellow);
            else if (current) item.setFillColor(sf::Color::Green);
            else item.setFillColor(sf::Color::White);
            item.setPosition(WINDOW_WIDTH / 2.0f - 70.0f, 250.0f + i * 70.0f);
            window->draw(item);

            if (current) {
                sf::Text currentText("(current)", *font, 20);
                currentText.setFillColor(sf::Color::Green);
                currentText.setPosition(WINDOW_WIDTH / 2.0f + 100.0f, 255.0f + i * 70.0f);
                window->draw(currentText);
            }
        }

        sf::Text hint("Use Up/Down to select, Enter to confirm, Escape to return", *font, 20);
        hint.setFillColor(sf::Color(200, 200, 200));
        hint.setPosition(WINDOW_WIDTH / 2.0f - 260.0f, WINDOW_HEIGHT - 60.0f);
        window->draw(hint);
    }

    void GameState::handleInput(const sf::Event& event) {
        if (event.type == sf::Event::Closed) {
            window->close();
            return;
        }

        if (event.type != sf::Event::KeyPressed) {
            return;
        }

        switch (currentState) {
        case MENU: handleMenuInput(event); break;
        case PLAYING: {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    currentState = State::MENU;
                    if (musicEnabled && music.getStatus() == sf::Music::Playing) {
                        music.stop();
                    }
                }
            }
            break;
        }
        case SETTINGS: handleSettingsInput(event); break;
        case DIFFICULTY: handleDifficultyInput(event); break;
        }
    }

    void GameState::handleMenuInput(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
            menuSelection = (menuSelection - 1 + 4) % 4;
        }
        else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
            menuSelection = (menuSelection + 1) % 4;
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            switch (menuSelection) {
            case 0: resetGame(); break;
            case 1: currentState = DIFFICULTY; difficultySelection = static_cast<int>(currentDifficulty); break;
            case 2: currentState = SETTINGS; menuSelection = 0; break;
            case 3: window->close(); break;
            }
        }
    }

    void GameState::handleSettingsInput(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
            menuSelection = (menuSelection - 1 + 2) % 2;
        }
        else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
            menuSelection = (menuSelection + 1) % 2;
        }
        else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
            if (menuSelection == 0) {
                soundEnabled = !soundEnabled;
            }
            else {
                musicEnabled = !musicEnabled;
                if (musicEnabled) music.setVolume(50);
                else music.setVolume(0);
            }
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            currentState = MENU;
            menuSelection = 0;
            if (musicEnabled) music.setVolume(50);
        }
    }

    void GameState::handleDifficultyInput(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
            difficultySelection = (difficultySelection - 1 + 3) % 3;
        }
        else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
            difficultySelection = (difficultySelection + 1) % 3;
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            currentDifficulty = static_cast<Difficulty>(difficultySelection);
            currentState = MENU;
            menuSelection = 0;
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            currentState = MENU;
            menuSelection = 0;
        }
    }

    bool GameState::isRunning() const {
        return window->isOpen();
    }

    sf::RenderWindow& GameState::getWindow() {
        return *window;
    }

}