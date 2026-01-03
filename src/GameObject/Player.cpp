#include "GameObject/Player.hpp"
#include "GameObject/Projectiles.hpp"
#include "GameWorld/GameWorld.hpp"

Player::Player(pGameWorld world)
    : GameObject(ImageID::PLAYER, INITIAL_X, INITIAL_Y, LayerID::PLAYER, 20, 48, world,
                 INITIAL_HP, 1, AnimID::IDLE),
      m_velocityY(0), m_isJumping(false), m_jumpCount(0), m_shootCooldown(0) {
    EnableHpText(60);
}

Player::~Player() {
}

void Player::UpdateActions() {
    if (!IsAlive()) {
        return;
    }

    // Update shoot cooldown
    if (m_shootCooldown > 0) {
        m_shootCooldown--;
    }

    // Handle shooting (J key or mouse left button)
    if ((GetWorld()->GetKey(KeyCode::JUMP) || GetWorld()->GetKey(KeyCode::FIRE1)) &&
        m_shootCooldown == 0) {
        // Create bullet at player's position + 30 pixels to the right
        GetWorld()->Instantiate(std::make_shared<Bullet>(GetWorld(), GetX() + 30, GetY()));
        m_shootCooldown = SHOOT_COOLDOWN;
    }

    // Handle jumping (K key or Space)
    if (GetWorld()->GetKeyDown(KeyCode::JUMP)) {
        if (!m_isJumping) {
            // First jump
            m_velocityY = JUMP_VELOCITY;
            m_isJumping = true;
            m_jumpCount = 1;
            PlayAnimation(AnimID::JUMP);
        } else if (m_jumpCount == 1) {
            // Second jump
            m_velocityY = JUMP_VELOCITY;
            m_jumpCount = 2;
        }
    }

    // Apply gravity and update position
    if (m_isJumping) {
        m_velocityY -= GRAVITY;
        MoveTo(GetX(), GetY() + m_velocityY);

        // Check if landed
        if (GetY() <= GROUND_Y) {
            MoveTo(GetX(), GROUND_Y);
            m_velocityY = 0;
            m_isJumping = false;
            m_jumpCount = 0;
            PlayAnimation(AnimID::IDLE);
        }
    }

    // HP text is managed by GameObject.
}
