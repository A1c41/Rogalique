#include "Paddle.h"

namespace Rogalique {

    Paddle::Paddle() : speed(350.0f), moveLeft(false), moveRight(false) {
        size = sf::Vector2f(120.0f, 22.0f);
        position = sf::Vector2f(WINDOW_WIDTH / 2.0f - size.x / 2, WINDOW_HEIGHT - 50.0f);

        sf::Image image;
        image.create(static_cast<unsigned>(size.x), static_cast<unsigned>(size.y), sf::Color::White);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void Paddle::update(float dt) {
        if (moveLeft) {
            position.x -= speed * dt;
            if (position.x < 0) position.x = 0;
        }
        if (moveRight) {
            position.x += speed * dt;
            if (position.x + size.x > WINDOW_WIDTH) position.x = WINDOW_WIDTH - size.x;
        }
        sprite.setPosition(position);
    }

    void Paddle::draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }

    void Paddle::handleInput(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
                moveLeft = true;
                moveRight = false;
            }
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {
                moveRight = true;
                moveLeft = false;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
                moveLeft = false;
            }
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {
                moveRight = false;
            }
        }
    }

}