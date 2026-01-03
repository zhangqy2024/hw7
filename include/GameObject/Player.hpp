#ifndef PLAYER_HPP__
#define PLAYER_HPP__

#include "GameObject/GameObject.hpp"
#include <memory>

class Player : public GameObject {
public:
    Player(pGameWorld world);
    ~Player();

    void UpdateActions() override;
    GameObject::Type GetType() const override { return GameObject::Type::Player; }

private:
    static const int INITIAL_X = 200;
    static const int INITIAL_Y = 120;
    static const int INITIAL_HP = 10;
    static const int JUMP_VELOCITY = 23;
    static const int GRAVITY = 2;
    static const int SHOOT_COOLDOWN = 10;
    static const int GROUND_Y = 120;

    int m_velocityY;
    bool m_isJumping;
    int m_jumpCount;
    int m_shootCooldown;
};

#endif // !PLAYER_HPP__
