#include "Bonus.h"
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "StrongBlock.h"
#include "GlassBlock.h"
#include "IndestructibleBlock.h"
#include <cstdlib>
#include <cmath>

namespace Arcanoid {

    FireBallEffect::FireBallEffect() : duration(8.0f), originalSpeed(0), isActive(false) {}

    void FireBallEffect::apply(Paddle* paddle, Ball* ball, std::vector<std::unique_ptr<GameObject>>& blocks) {
        if (ball) {
            sf::Vector2f vel = ball->getVelocity();
            originalSpeed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
            ball->setSpeed(originalSpeed * 1.5f);
            isActive = true;
        }
    }

    void FireBallEffect::remove(Paddle* paddle, Ball* ball, std::vector<std::unique_ptr<GameObject>>& blocks) {
        if (ball && isActive) {
            ball->setSpeed(originalSpeed);
            isActive = false;
        }
    }

    float FireBallEffect::getDuration() const { return duration; }
    sf::Color FireBallEffect::getColor() const { return sf::Color(255, 100, 0); }
    bool FireBallEffect::isFireBallActive() const { return isActive; }

    FragileBlocksEffect::FragileBlocksEffect() : duration(10.0f) {}

    void FragileBlocksEffect::apply(Paddle* paddle, Ball* ball, std::vector<std::unique_ptr<GameObject>>& blocks) {
        /*for (auto& block : blocks) {
            auto* strongBlock = dynamic_cast<StrongBlock*>(block.get());
            if (strongBlock) {
                while (strongBlock->getHitsRemaining() > 1) {
                    strongBlock->hit();
                }
            }
        }*/
    }

    void FragileBlocksEffect::remove(Paddle* paddle, Ball* ball, std::vector<std::unique_ptr<GameObject>>& blocks) {}

    float FragileBlocksEffect::getDuration() const { return duration; }
    sf::Color FragileBlocksEffect::getColor() const { return sf::Color(100, 255, 100); }

    PaddleSizeEffect::PaddleSizeEffect(float multiplier)
        : duration(10.0f), sizeMultiplier(multiplier) {
    }

    void PaddleSizeEffect::apply(Paddle* paddle, Ball* ball, std::vector<std::unique_ptr<GameObject>>& blocks) {
        if (paddle) {
            originalSize = paddle->getSize();
            sf::Vector2f newSize = originalSize;
            newSize.x *= sizeMultiplier;
            paddle->setSize(newSize);
        }
    }

    void PaddleSizeEffect::remove(Paddle* paddle, Ball* ball, std::vector<std::unique_ptr<GameObject>>& blocks) {
        if (paddle) {
            paddle->setSize(originalSize);
        }
    }

    float PaddleSizeEffect::getDuration() const { return duration; }
    sf::Color PaddleSizeEffect::getColor() const {
        return sizeMultiplier > 1.0f ? sf::Color(100, 255, 255) : sf::Color(255, 100, 255);
    }

    PaddleSpeedEffect::PaddleSpeedEffect(float multiplier)
        : duration(10.0f), speedMultiplier(multiplier), originalSpeed(0) {
    }

    void PaddleSpeedEffect::apply(Paddle* paddle, Ball* ball, std::vector<std::unique_ptr<GameObject>>& blocks) {
        if (paddle) {
            originalSpeed = paddle->getSpeed();
            paddle->setSpeed(originalSpeed * speedMultiplier);
        }
    }

    void PaddleSpeedEffect::remove(Paddle* paddle, Ball* ball, std::vector<std::unique_ptr<GameObject>>& blocks) {
        if (paddle) {
            paddle->setSpeed(originalSpeed);
        }
    }

    float PaddleSpeedEffect::getDuration() const { return duration; }
    sf::Color PaddleSpeedEffect::getColor() const {
        return speedMultiplier > 1.0f ? sf::Color(255, 255, 100) : sf::Color(255, 100, 100);
    }

    Bonus::Bonus(float x, float y, Type type)
        : type(type), fallSpeed(150.0f), isActive(true), effect(nullptr) {

        position = sf::Vector2f(x, y);
        size = sf::Vector2f(20.0f, 20.0f);

        createShape();

        switch (type) {
        case Type::FIRE_BALL:
            effect = std::make_unique<FireBallEffect>();
            break;
        case Type::FRAGILE_BLOCKS:
            effect = std::make_unique<FragileBlocksEffect>();
            break;
        case Type::SIZE_UP:
            effect = std::make_unique<PaddleSizeEffect>(1.5f);
            break;
        case Type::SIZE_DOWN:
            effect = std::make_unique<PaddleSizeEffect>(0.7f);
            break;
        case Type::SPEED_UP:
            effect = std::make_unique<PaddleSpeedEffect>(1.5f);
            break;
        case Type::SPEED_DOWN:
            effect = std::make_unique<PaddleSpeedEffect>(0.7f);
            break;
        }
    }

    void Bonus::createShape() {
        shape.setRadius(10.0f);
        shape.setPosition(position);

        sf::Color color;
        switch (type) {
        case Type::FIRE_BALL: color = sf::Color(255, 100, 0); break;
        case Type::FRAGILE_BLOCKS: color = sf::Color(100, 255, 100); break;
        case Type::SIZE_UP: color = sf::Color(100, 255, 255); break;
        case Type::SIZE_DOWN: color = sf::Color(255, 100, 255); break;
        case Type::SPEED_UP: color = sf::Color(255, 255, 100); break;
        case Type::SPEED_DOWN: color = sf::Color(255, 100, 100); break;
        }
        shape.setFillColor(color);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.0f);
    }

    void Bonus::update(float dt) {
        if (!isActive) return;

        position.y += fallSpeed * dt;
        shape.setPosition(position);

        if (position.y > WINDOW_HEIGHT) {
            isActive = false;
        }
    }

    void Bonus::draw(sf::RenderWindow& window) const {
        if (isActive) {
            window.draw(shape);
        }
    }

    sf::FloatRect Bonus::getBounds() const {
        return shape.getGlobalBounds();
    }

    std::unique_ptr<Bonus> BonusFactory::createRandomBonus(float x, float y) {
        int type = rand() % 6;
        return std::make_unique<Bonus>(x, y, static_cast<Bonus::Type>(type));
    }

}