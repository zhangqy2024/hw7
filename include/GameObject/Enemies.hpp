#ifndef ENEMIES_HPP__

#define ENEMIES_HPP__

#include <memory>

#include "GameObject/GameObject.hpp"
#include "GameWorld/GameWorld.hpp"

class Goblin : public GameObject {
public:
    Goblin(pGameWorld world, int x, int y);

    void UpdateActions() override;
    GameObject::Type GetType() const override { return GameObject::Type::Enemy; }

private:
    static const int SPEED = 4;
    static const int THROW_INTERVAL = 100;
    static const int THROW_ANIMATION_DURATION = 20;

    int m_throwTimer;
    int m_throwAnimationTimer;
    bool m_isThrowing;
};

class Plant : public GameObject {
public:
    Plant(pGameWorld world, int x, int y);

    void UpdateActions() override;
    GameObject::Type GetType() const override { return GameObject::Type::Enemy; }

private:
    static const int SPEED = 4;
};

class Bird : public GameObject {
public:
    Bird(pGameWorld world, int x, int y);

    void UpdateActions() override;
    GameObject::Type GetType() const override { return GameObject::Type::Enemy; }

private:
    static const int SPEED = 14;
};

class Pig : public GameObject {
public:
    Pig(pGameWorld world, int x, int y);

    void UpdateActions() override;
    GameObject::Type GetType() const override { return GameObject::Type::Enemy; }

private:
    static const int INITIAL_SPEED = 4;
    static const int RUSH_SPEED = 7;
    static const int RUSH_DELAY = 15;

    int m_speed;
    int m_rushTimer;
    bool m_isRushing;
};

#endif // !ENEMIES_HPP__
