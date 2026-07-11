#pragma once
#include "GameObject.h"
#include "Constants.h"

namespace Rogalique {

    class Paddle : public GameObject {
    private:
        float speed;
        bool moveLeft;
        bool moveRight;
        sf::Texture texture;

    public:
        Paddle();
        ~Paddle() = default;

        void update(float dt) override;
        void draw(sf::RenderWindow& window) const override;
        void handleInput(const sf::Event& event) override;

        void setSpeed(float newSpeed) { speed = newSpeed; }
        float getSpeed() const { return speed; }
    };

}