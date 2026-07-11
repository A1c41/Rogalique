#include "SaveSystem.h"
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
#include "StrongBlock.h"
#include "GlassBlock.h"
#include "IndestructibleBlock.h"
#include <fstream>

namespace Arcanoid {

    GameMemento::GameMemento() : score(0), lives(3) {}

    void GameMemento::saveState(int score_, int lives_,
        const std::vector<std::unique_ptr<GameObject>>& blocks_,
        const GameObject* ball_,
        const GameObject* paddle_,
        const std::vector<std::unique_ptr<Bonus>>& bonuses_,
        const std::vector<std::pair<std::unique_ptr<IBonusEffect>, float>>& activeEffects_) {
        score = score_;
        lives = lives_;

        blocks.clear();
        for (const auto& block : blocks_) {
            BlockState state;
            state.x = block->getPosition().x;
            state.y = block->getPosition().y;
            state.isActive = true;
            state.hitsRemaining = 0;
            state.isGlass = false;
            state.isIndestructible = false;
            state.colorR = 255;
            state.colorG = 255;
            state.colorB = 255;

            auto* blockPtr = dynamic_cast<Block*>(block.get());
            if (blockPtr) {
                state.isActive = blockPtr->isAlive();
                sf::Color color = blockPtr->getColor();
                state.colorR = color.r;
                state.colorG = color.g;
                state.colorB = color.b;
            }

            auto* strongBlock = dynamic_cast<StrongBlock*>(block.get());
            if (strongBlock) {
                state.hitsRemaining = strongBlock->getHitsRemaining();
                sf::Color color = strongBlock->getColor();
                state.colorR = color.r;
                state.colorG = color.g;
                state.colorB = color.b;
            }

            auto* glassBlock = dynamic_cast<GlassBlock*>(block.get());
            if (glassBlock) {
                state.isGlass = true;
                state.isActive = glassBlock->isAlive();
                sf::Color color = glassBlock->getColor();
                state.colorR = color.r;
                state.colorG = color.g;
                state.colorB = color.b;
            }

            auto* indestructibleBlock = dynamic_cast<IndestructibleBlock*>(block.get());
            if (indestructibleBlock) {
                state.isIndestructible = true;
                state.isActive = true;
                sf::Color color = indestructibleBlock->getColor();
                state.colorR = color.r;
                state.colorG = color.g;
                state.colorB = color.b;
            }

            blocks.push_back(state);
        }

        if (ball_) {
            auto* ball = dynamic_cast<const Ball*>(ball_);
            if (ball) {
                ballState.x = ball->getPosition().x;
                ballState.y = ball->getPosition().y;
                ballState.vx = ball->getVelocity().x;
                ballState.vy = ball->getVelocity().y;
                ballState.speed = 400.0f;
            }
        }

        if (paddle_) {
            paddleState.x = paddle_->getPosition().x;
            paddleState.y = paddle_->getPosition().y;
            paddleState.sizeX = paddle_->getSize().x;
            paddleState.sizeY = paddle_->getSize().y;
            auto* paddle = dynamic_cast<const Paddle*>(paddle_);
            if (paddle) {
                paddleState.speed = paddle->getSpeed();
            }
        }

        bonuses.clear();
        for (const auto& bonus : bonuses_) {
            if (bonus->isAlive()) {
                BonusState state;
                state.x = bonus->getPosition().x;
                state.y = bonus->getPosition().y;
                state.type = static_cast<int>(bonus->getType());
                state.isActive = true;
                state.remainingTime = 5.0f;
                bonuses.push_back(state);
            }
        }

        activeEffects.clear();
        for (const auto& effect : activeEffects_) {
            ActiveEffectState state;
            if (dynamic_cast<FireBallEffect*>(effect.first.get())) {
                state.type = 0;
            }
            else if (dynamic_cast<FragileBlocksEffect*>(effect.first.get())) {
                state.type = 1;
            }
            else if (dynamic_cast<PaddleSizeEffect*>(effect.first.get())) {
                auto* sizeEffect = dynamic_cast<PaddleSizeEffect*>(effect.first.get());
                state.type = 2;
            }
            else if (dynamic_cast<PaddleSpeedEffect*>(effect.first.get())) {
                state.type = 4;
            }
            state.elapsedTime = effect.second;
            activeEffects.push_back(state);
        }
    }

    int GameMemento::getScore() const { return score; }
    int GameMemento::getLives() const { return lives; }
    const std::vector<GameMemento::BlockState>& GameMemento::getBlocks() const { return blocks; }
    const GameMemento::BallState& GameMemento::getBall() const { return ballState; }
    const GameMemento::PaddleState& GameMemento::getPaddle() const { return paddleState; }
    const std::vector<GameMemento::BonusState>& GameMemento::getBonuses() const { return bonuses; }
    const std::vector<GameMemento::ActiveEffectState>& GameMemento::getActiveEffects() const { return activeEffects; }

    SaveSystem::SaveSystem() {
        saveFile = RESOURCES + "save.dat";
        save = nullptr;
    }

    SaveSystem::~SaveSystem() {
        if (save) {
            saveToFile();
        }
    }

    void SaveSystem::saveGame(int score, int lives,
        const std::vector<std::unique_ptr<GameObject>>& blocks,
        const GameObject* ball,
        const GameObject* paddle,
        const std::vector<std::unique_ptr<Bonus>>& bonuses,
        const std::vector<std::pair<std::unique_ptr<IBonusEffect>, float>>& activeEffects) {
        save = std::make_unique<GameMemento>();
        save->saveState(score, lives, blocks, ball, paddle, bonuses, activeEffects);
        saveToFile();
    }

    bool SaveSystem::loadGame() {
        loadFromFile();
        return save != nullptr;
    }

    const GameMemento* SaveSystem::getCurrentSave() const {
        return save.get();
    }

    bool SaveSystem::hasSave() const {
        return save != nullptr;
    }

    void SaveSystem::saveToFile() {
        if (!save) return;

        std::ofstream file(saveFile, std::ios::binary);
        if (!file.is_open()) return;

        int score = save->getScore();
        int lives = save->getLives();
        file.write(reinterpret_cast<const char*>(&score), sizeof(int));
        file.write(reinterpret_cast<const char*>(&lives), sizeof(int));

        GameMemento::BallState ballState = save->getBall();
        file.write(reinterpret_cast<const char*>(&ballState), sizeof(GameMemento::BallState));

        GameMemento::PaddleState paddleState = save->getPaddle();
        file.write(reinterpret_cast<const char*>(&paddleState), sizeof(GameMemento::PaddleState));

        std::vector<GameMemento::BlockState> blocks = save->getBlocks();
        size_t blockCount = blocks.size();
        file.write(reinterpret_cast<const char*>(&blockCount), sizeof(size_t));
        for (const auto& block : blocks) {
            file.write(reinterpret_cast<const char*>(&block), sizeof(GameMemento::BlockState));
        }

        std::vector<GameMemento::BonusState> bonuses = save->getBonuses();
        size_t bonusCount = bonuses.size();
        file.write(reinterpret_cast<const char*>(&bonusCount), sizeof(size_t));
        for (const auto& bonus : bonuses) {
            file.write(reinterpret_cast<const char*>(&bonus), sizeof(GameMemento::BonusState));
        }

        std::vector<GameMemento::ActiveEffectState> activeEffects = save->getActiveEffects();
        size_t effectCount = activeEffects.size();
        file.write(reinterpret_cast<const char*>(&effectCount), sizeof(size_t));
        for (const auto& effect : activeEffects) {
            file.write(reinterpret_cast<const char*>(&effect), sizeof(GameMemento::ActiveEffectState));
        }

        file.close();
    }

    void SaveSystem::loadFromFile() {
        std::ifstream file(saveFile, std::ios::binary);
        if (!file.is_open()) return;

        try {
            save = std::make_unique<GameMemento>();

            int score, lives;
            file.read(reinterpret_cast<char*>(&score), sizeof(int));
            file.read(reinterpret_cast<char*>(&lives), sizeof(int));
            save->setScore(score);
            save->setLives(lives);

            GameMemento::BallState ballState;
            file.read(reinterpret_cast<char*>(&ballState), sizeof(GameMemento::BallState));
            save->setBall(ballState);

            GameMemento::PaddleState paddleState;
            file.read(reinterpret_cast<char*>(&paddleState), sizeof(GameMemento::PaddleState));
            save->setPaddle(paddleState);

            size_t blockCount;
            file.read(reinterpret_cast<char*>(&blockCount), sizeof(size_t));
            std::vector<GameMemento::BlockState> blocks;
            for (size_t i = 0; i < blockCount; ++i) {
                GameMemento::BlockState block;
                file.read(reinterpret_cast<char*>(&block), sizeof(GameMemento::BlockState));
                blocks.push_back(block);
            }
            save->setBlocks(blocks);

            size_t bonusCount;
            file.read(reinterpret_cast<char*>(&bonusCount), sizeof(size_t));
            std::vector<GameMemento::BonusState> bonuses;
            for (size_t i = 0; i < bonusCount; ++i) {
                GameMemento::BonusState bonus;
                file.read(reinterpret_cast<char*>(&bonus), sizeof(GameMemento::BonusState));
                bonuses.push_back(bonus);
            }
            save->setBonuses(bonuses);

            size_t effectCount;
            file.read(reinterpret_cast<char*>(&effectCount), sizeof(size_t));
            std::vector<GameMemento::ActiveEffectState> activeEffects;
            for (size_t i = 0; i < effectCount; ++i) {
                GameMemento::ActiveEffectState effect;
                file.read(reinterpret_cast<char*>(&effect), sizeof(GameMemento::ActiveEffectState));
                activeEffects.push_back(effect);
            }
            save->setActiveEffects(activeEffects);

            file.close();
        }
        catch (...) {
            save = nullptr;
            file.close();
        }
    }

}